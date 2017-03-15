/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt plumber.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <cstring>
using namespace std;

typedef unsigned int uint;

// struct to store information about tokens
struct Attrib {
  string kind;
  string text;
};

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt);

class Data {
  public:
  virtual void print() = 0;
  virtual Data* clone() = 0;
  virtual ~Data() {}
};

class SimpleData : public Data {
  protected:
  uint diameter;
  public:
  SimpleData(uint diameter) : diameter(diameter) {}
  //SimpleData() : diameter(0) {}
  virtual ~SimpleData() {}
  uint getDiameter() {
    return diameter;
  }
};

class Tube : public SimpleData {
  uint length;
  public:
  Tube(uint length, uint diameter) : length(length), SimpleData(diameter) {}
  //Tube() : length(0) {}
  
  uint getLength() {
    return length;
  }
  Data* clone() {
    return new Tube(length, diameter);
  }
  
  void print() {
    cout << "Tube of length: " << length << " and diameter: " << diameter << endl;
  }
  
};

class Connector : public SimpleData {
  public:
  Connector(uint diameter) : SimpleData(diameter) {}
  
  Data* clone() {
    return new Connector(diameter);
  }
  
  void print() {
    cout << "Connector of diameter: " << diameter << endl;
  }
};

class Vector : public Data {
  Tube* vec;
  uint size;
  uint limit;
  
  public:
  Vector(uint limit) : limit(limit), size(0) {
    //vec = new Tube[limit];
    vec = (Tube*)malloc(limit*sizeof(Tube));
  }
  ~Vector() {
    free(vec);
    //delete vec;
  }
  
  bool full() {
    return size == limit;
  }
  bool empty() {
    return size == 0;;
  }
  uint length() {
    return size;
  }
  void push(const Tube& tube) {
    if (size < limit) {
      //vec[size] = tube;
      memcpy(&vec[size], &tube, sizeof(Tube));
      ++size;
    }
    else {
      cerr << "Trying to push a full tube vector" << endl;
      exit(-1);
    }
  }
  Tube* pop() {
    if (size > 0) {
      --size;
      return (Tube*)vec[size].clone();
    }
    cout << "Trying to pop an empty tube vector" << endl;
    exit(-1);
  }
  
  Data* clone() {
    Vector* auxVector = new Vector(limit);
    memcpy(auxVector->vec, vec, sizeof(Tube)*size);
    auxVector->size = size;
    return auxVector;
  }
  
  void print() {
    cout << "Vector of tubes: " << endl;
    for (uint i = 0; i < size; ++i) {
      cout << "\t";
      vec[i].print();
    }
  }
};
#define GENAST

#include "ast.h"

#define zzSET_SIZE 4
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>
#include <map>
#include <typeinfo>

map<string, Data*> vars;
typedef map<string, Data*>::iterator Iterator;

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  switch (type) {
    case ID:
    attr->text = text;
    attr->kind = "ID";
    break;
    case NUM:
    attr->text = text;
    attr->kind = "NUM";
    break;
    case ARRAY:
    attr->kind = "ARRAY";
    attr->text = "";
    break;
    default:
    attr->kind = text;
    attr->text = "";
    break;
  }
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind;
  as->text = attr->text;
  as->right = NULL;
  as->down = NULL;
  return as;
}

/// create a new "list" AST node with one element
AST* createASTlist(AST *child) {
  AST* as=new AST;
  as->kind="list";
  as->right=NULL;
  as->down=child;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a,int n) {
  AST *c=a->down;
  for (int i=0; c!=NULL && i<n; i++) c=c->right;
  return c;
}

/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s) {
  if (a==NULL) return;
  
  cout<<a->kind;
  if (a->text!="") cout<<"("<<a->text<<")";
  cout<<endl;
  
  AST *i = a->down;
  while (i!=NULL && i->right!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
  
  if (i!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
}

/// print AST 
void ASTPrint(AST *a) {
  while (a!=NULL) {
    cout << " ";
    ASTPrintIndent(a,"");
    a=a->right;
  }
}

inline Data* getData(const string& varName) {
  Iterator it = vars.find(varName);
  if (it == vars.end()) {
    cout << "Var " << varName << " does not exist" << endl;
    exit(-1);
  }
  return it->second;
}

inline void insertData(Data* data, const string& name) {
  Iterator it = vars.lower_bound(name);
  if (it != vars.end() and name == it->first) {
    delete it->second;
    it->second = data;
  }
  else vars.insert(it, make_pair(name, data));
}

inline uint getLength(AST* child) {
  Iterator it = vars.find(child->text);
  if (it != vars.end()) {
    Tube* tube = dynamic_cast<Tube*>(it->second);
    if (tube) return tube->getLength();
    cout << "Wrong type, only tubes have length but " << child->text << " is a " << typeid(*it->second).name() << " instance" << endl;
    exit(-1);
  }
  cout << "Var " << child->text << " does not exist" << endl;
  exit(-1);
}

inline uint getDiameter(AST* child) {
  Iterator it = vars.find(child->text);
  if (it != vars.end()) {
    SimpleData* data = dynamic_cast<SimpleData*>(it->second);
    if (data) return data->getDiameter();
    
    cout << "Wrong type, only tubes and connectors have diameter but " << child->text << " is a " << typeid(*it->second).name() << " instance" << endl;
    exit(-1);
  }
  cout << "Var " << child->text << " does not exist" << endl;
  exit(-1);
}

int evaluateExpresion(AST* root) {
  string& kind = root->kind;
  if (kind == "NUM") {
    return stoi(root->text);
  }
  else if (kind == "+") {
    return evaluateExpresion(root->down)+evaluateExpresion(root->down->right);
  }
  else if (kind == "-") {
    return evaluateExpresion(root->down)-evaluateExpresion(root->down->right);
  }
  else if (kind == "*") {
    return evaluateExpresion(root->down)*evaluateExpresion(root->down->right);
  }
  else if (kind == "LENGTH") {
    return getLength(root->down);
  }
  else { //DIAMETER
    return getDiameter(root->down);
  }
}

bool evaluateBoolean(AST* root) {
  string& kind = root->kind;
  if (kind == "AND") {
    return evaluateBoolean(root->down) and evaluateBoolean(root->down->right);
  }
  else if (kind == "OR") {
    return evaluateBoolean(root->down) or evaluateBoolean(root->down->right);
  }
  else if (kind == "NOT") {
    return not evaluateBoolean(root->down);
  }
  else if (kind == "FULL") {
    Iterator it = vars.find(root->down->text);
    if (it == vars.end()) {
      cout << "Var " << root->down->text << " does not exist" << endl;
      exit(-1);
    }
    Vector* vec = dynamic_cast<Vector*>(it->second);
    if (not vec) {
      cout << root->down->text << " is a " << typeid(*it->second).name() << ", so FULL can't be applied" << endl;
      exit(-1);
    }
    return vec->full();
  }
  else if (kind == "EMPTY") {
    Iterator it = vars.find(root->down->text);
    if (it == vars.end()) {
      cout << "Var " << root->down->text << "does not exist" << endl;
      exit(-1);
    }
    Vector* vec = dynamic_cast<Vector*>(it->second);
    if (not vec) {
      cout << root->down->text << " is a " << typeid(*it->second).name() << ", so EMPTY can't be applied" << endl;
      exit(-1);
    }
    return vec->empty();
  }
  else if (kind == "<") {
    return evaluateExpresion(root->down) < evaluateExpresion(root->down->right);
  }
  else if (kind == ">") {
    return evaluateExpresion(root->down) > evaluateExpresion(root->down->right);
  }
  else {//==
    return evaluateExpresion(root->down) == evaluateExpresion(root->down->right);
  }
}

Connector* getConnector(AST* root, Iterator* idIterator) {
  if (idIterator != NULL) *idIterator = vars.end();
  string& type = root->kind;
  
  if (type == "ID") {
    Iterator it = vars.find(root->text);
    if (it == vars.end()) {
      cout << "Var " << root->text << " does not exist" << endl;
      exit(-1);
    }
    Connector* connector = dynamic_cast<Connector*>(it->second);
    if (not connector) {
      cout << "Variable " << root->text << " is a " << typeid(*it->second).name() << " but a connector was requested" << endl;
      exit(-1);
    }
    if (idIterator != NULL) *idIterator = it;
    return connector;
  }
  else {//Connector
    return new Connector(evaluateExpresion(root->down));
  }
}

Tube* mergeTubes(AST *root);

Tube* getTube(AST* root, Iterator* idIterator) {
  if (idIterator != NULL) *idIterator = vars.end();
  string& type = root->kind;
  
  if (type == "TUBE") {
    AST* lengthAST = root->down;
    AST* diameterAST = lengthAST->right;
    return new Tube(evaluateExpresion(lengthAST), evaluateExpresion(diameterAST));
  }
  
  else if (type == "ID") {
    Iterator it = vars.find(root->text);
    if (it == vars.end()) {
      cout << "Var " << root->text << " does not exist" << endl;
      exit(-1);
    }
    Tube* tube = dynamic_cast<Tube*>(it->second);
    if (not tube) {
      cout << "Variable " << root->text << " is a " << typeid(it->second).name() << " but a tube was requested" << endl;
      exit(-1);
    }
    if (idIterator != NULL) *idIterator = it;
    return tube;
  }
  
  else { //MERGE
    return mergeTubes(root);
  }
}

Tube* mergeTubes(AST* root) {
  AST* tube1AST = root->down;
  AST* connectorAST = tube1AST->right;
  AST* tube2AST = connectorAST->right;
  
  Iterator itTube1, itTube2, itConnector;
  Tube* tube1 = getTube(tube1AST, &itTube1);
  Tube* tube2 = getTube(tube2AST, &itTube2);
  if (tube1 == tube2) {
    cout << "Can't merge a tube with itself" << endl;
    exit(-1);
  }
  Connector* connector = getConnector(connectorAST, &itConnector);
  
  uint diameterT1, diameterT2, diameterC;
  diameterT1 = tube1->getDiameter();
  diameterT2 = tube2->getDiameter();
  diameterC = connector->getDiameter();
  if (not (diameterT1 == diameterT2 and diameterT1 == diameterC)) {
    cout << "Merging tubes require equal diameters for all arguments, but this was provided: " << endl;
    cout << tube1AST->text << ", diameter " << diameterT1 << endl;
    cout << connectorAST->text << ", diameter " << diameterC << endl;
    cout << tube2AST->text << ", diameter " << diameterT2 << endl;
    exit(-1);
  }
  
  Tube* ret = new Tube(tube1->getLength()+tube2->getLength(), diameterC);
  delete tube1; delete tube2; delete connector;
  if (itTube1 != vars.end()) vars.erase(itTube1);
  if (itTube2 != vars.end()) vars.erase(itTube2);
  if (itConnector != vars.end()) vars.erase(itConnector);
  return ret;
}

void executeList(AST* root);

void execute(AST* root) {
  if (root->kind == "=") {
    AST* childLeft = root->down;
    AST* childRight = childLeft->right;
    string& varName = childLeft->text;
    string& assignType = childRight->kind;
    
    if (assignType == "TUBE") {
      AST* lengthAST = childRight->down;
      AST* diameterAST = lengthAST->right;
      insertData(new Tube(evaluateExpresion(lengthAST), evaluateExpresion(diameterAST)), varName);
    }
    
    else if (assignType == "CONNECTOR") {
      insertData(new Connector(evaluateExpresion(childRight->down)), varName);
    }
    
    else if (assignType == "ARRAY") {
      insertData(new Vector(evaluateExpresion(childRight->down)), varName);
    }
    
    else if (assignType == "MERGE") {
      insertData(mergeTubes(childRight), varName);
    }
    
    else if (childRight->right != NULL) { //SPLIT
      Iterator itTube;
      Tube* tube = getTube(childRight->right->down, &itTube);
      
      Tube* tube1 = new Tube(tube->getLength()/2, tube->getDiameter());
      Tube* tube2 = new Tube(tube->getLength() - tube->getLength()/2, tube->getDiameter());
      
      delete tube;
      if (itTube != vars.end()) vars.erase(itTube);
      
      insertData(tube1, childLeft->text);
      insertData(tube2, childRight->text);
    }
    
    else { //COPY
      Iterator it = vars.find(childRight->text);
      if (it == vars.end()) {
        cout << "Variable " << childRight->text << " does not exist" << endl;
        exit(-1);
      }
      insertData(it->second->clone(), varName);
    }
  }
  else if (root->kind == "LENGTH") {
    cout << getLength(root->down) << endl;
  }
  else if (root->kind == "DIAMETER") {
    cout << getDiameter(root->down) << endl;
  }
  else if (root->kind == "POP") {
    Iterator it = vars.find(root->down->text);
    if (it == vars.end()) {
      cout << "Var " << root->down->text << " does not exist";
      exit(-1);
    }
    Vector* vec = dynamic_cast<Vector*>(it->second);
    if (not vec) {
      cout << "Pop is only possible with vectors, but " << root->down->text << " is a " << typeid(*it->second).name() << endl;
      exit(-1);
    }
    insertData(vec->pop(), root->down->right->text);
  }
  else if (root->kind == "PUSH") {
    Iterator it = vars.find(root->down->text);
    if (it == vars.end()) {
      cout << "Var " << root->down->text << " does not exist";
      exit(-1);
    }
    Vector* vec = dynamic_cast<Vector*>(it->second);
    if (not vec) {
      cout << "Push is only possible with vectors, but " << root->down->text << " is a " << typeid(*it->second).name() << endl;
      exit(-1);
    }
    Data* data = getData(root->down->right->text);
    Tube* tube = dynamic_cast<Tube*>(data);
    if (not tube) {
      cout << "You can only push tubes to a vector, but " << root->down->right->text << " is a " << typeid(*data).name() << endl;
      exit(-1);
    }
    vec->push(*tube);
  }
  else {//WHILE
    AST* condition = root->down;
    AST* listInstructions = condition->right;
    while (evaluateBoolean(condition)) {
      executeList(listInstructions);
    }
  }
}

void executeList(AST* root) {
  AST* child = root->down;
  while (child != NULL) {
    execute(child);
    child = child->right;
  }
}

int main() {
  AST *root = NULL;
  ANTLR(plumber(&root), stdin);
  //ASTPrint(root);
  executeList(root);
  
  for (pair<const string, Data*>& var : vars) {
    cout << var.first << " is a ";
    var.second->print();
  }
}

void
#ifdef __USE_PROTOS
plumber(AST**_root)
#else
plumber(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  listOp(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(1);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x1);
  }
}

void
#ifdef __USE_PROTOS
listOp(AST**_root)
#else
listOp(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x2) ) {
      op(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x4);
  }
}

void
#ifdef __USE_PROTOS
op(AST**_root)
#else
op(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==ID) ) {
    zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    zzmatch(ASSIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      if ( (setwd1[LA(1)]&0x8) ) {
        funcTube(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==CONNECTOR) ) {
          funcConnector(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==ARRAY) ) {
            funcArray(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (LA(1)==LPAREN) ) {
      zzmatch(LPAREN);  zzCONSUME;
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzmatch(COMMA);  zzCONSUME;
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzmatch(RPAREN);  zzCONSUME;
      zzmatch(ASSIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      funcSplit(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==WHILE) ) {
        zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(LPAREN);  zzCONSUME;
        boolExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzmatch(RPAREN);  zzCONSUME;
        listOp(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzmatch(ENDWHILE);  zzCONSUME;
      }
      else {
        if ( (LA(1)==DIAMETER) ) {
          zzmatch(DIAMETER); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          zzmatch(LPAREN);  zzCONSUME;
          zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          zzmatch(RPAREN);  zzCONSUME;
        }
        else {
          if ( (LA(1)==LENGTH) ) {
            zzmatch(LENGTH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            zzmatch(LPAREN);  zzCONSUME;
            zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
            zzmatch(RPAREN);  zzCONSUME;
          }
          else {
            if ( (LA(1)==PUSH) ) {
              zzmatch(PUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
              zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
              funcTube(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {
              if ( (LA(1)==POP) ) {
                zzmatch(POP); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
                zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
                zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
              }
              else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
        }
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x10);
  }
}

void
#ifdef __USE_PROTOS
funcSplit(AST**_root)
#else
funcSplit(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(SPLIT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  funcTube(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
funcArray(AST**_root)
#else
funcArray(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ARRAY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  aritmeticExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
funcConnector(AST**_root)
#else
funcConnector(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(CONNECTOR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  aritmeticExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
funcTube(AST**_root)
#else
funcTube(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==ID) ) {
    zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  }
  else {
    if ( (LA(1)==MERGE) ) {
      zzmatch(MERGE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      funcTube(zzSTR); zzlink(_root, &_sibling, &_tail);
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        if ( (LA(1)==CONNECTOR) ) {
          funcConnector(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==ID) ) {
            zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp2);
        }
      }
      funcTube(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==TUBE) ) {
        zzmatch(TUBE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        aritmeticExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
        aritmeticExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
boolExpr(AST**_root)
#else
boolExpr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  andExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==OR) ) {
      zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      andExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
andExpr(AST**_root)
#else
andExpr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  notFunc(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==AND) ) {
      zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      notFunc(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
notFunc(AST**_root)
#else
notFunc(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NOT) ) {
      zzmatch(NOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd2[LA(1)]&0x8) ) {
      }
      else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  boolFunc(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
boolFunc(AST**_root)
#else
boolFunc(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (setwd2[LA(1)]&0x20) ) {
    aritmeticExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      while ( (setwd2[LA(1)]&0x40) ) {
        {
          zzBLOCK(zztasp3);
          zzMake0;
          {
          if ( (LA(1)==GT) ) {
            zzmatch(GT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==LT) ) {
              zzmatch(LT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {
              if ( (LA(1)==EQ) ) {
                zzmatch(EQ); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
              }
              else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
          zzEXIT(zztasp3);
          }
        }
        aritmeticExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
        zzLOOP(zztasp2);
      }
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (LA(1)==FULL) ) {
      zzmatch(FULL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      zzmatch(LPAREN);  zzCONSUME;
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzmatch(RPAREN);  zzCONSUME;
    }
    else {
      if ( (LA(1)==EMPTY) ) {
        zzmatch(EMPTY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(LPAREN);  zzCONSUME;
        zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(RPAREN);  zzCONSUME;
      }
      else {
        if ( (LA(1)==LPAREN) ) {
          zzmatch(LPAREN);  zzCONSUME;
          boolExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
          zzmatch(RPAREN);  zzCONSUME;
        }
        else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x80);
  }
}

void
#ifdef __USE_PROTOS
aritmeticExpr(AST**_root)
#else
aritmeticExpr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  mulExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd3[LA(1)]&0x1) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==MINUS) ) {
          zzmatch(MINUS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==ADD) ) {
            zzmatch(ADD); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      mulExpr(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

void
#ifdef __USE_PROTOS
mulExpr(AST**_root)
#else
mulExpr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  numFunc(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==MUL) ) {
      zzmatch(MUL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      numFunc(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x4);
  }
}

void
#ifdef __USE_PROTOS
numFunc(AST**_root)
#else
numFunc(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==NUM) ) {
    zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  }
  else {
    if ( (LA(1)==LENGTH) ) {
      zzmatch(LENGTH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      zzmatch(LPAREN);  zzCONSUME;
      zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      zzmatch(RPAREN);  zzCONSUME;
    }
    else {
      if ( (LA(1)==DIAMETER) ) {
        zzmatch(DIAMETER); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(LPAREN);  zzCONSUME;
        zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        zzmatch(RPAREN);  zzCONSUME;
      }
      else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}
