#header
<<
#include <string>
#include <iostream>
#include <vector>
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
};

class SimpleData : public Data {
protected:
    uint diameter;
public:
    SimpleData(uint diameter) : diameter(diameter) {

    }

    uint getDiameter() {
        return diameter;
    }
};

class Tube : public SimpleData {
    uint length;
public:
    Tube(uint length, uint diameter) : length(length), SimpleData(diameter) {
    }

    uint getLength() {
        return length;
    }

    void print() {
        cout << "Tube of length: " << length << " and diameter: " << diameter << endl;
    }

};

class Connector : public SimpleData {
public:
    Connector(uint diameter) : SimpleData(diameter) {
    }

    void print() {
        cout << "Connector of diameter: " << diameter << endl;
    }
};

class Vector : public Data {
    vector<Tube> vec;

public:
    Vector(uint size) {
        vec.reserve(size);
        cout << size << " " << vec.capacity() << endl;
    }

    bool full() {
        return vec.size() == vec.capacity();
    }
    bool empty() {
        return vec.size() > 0;
    }

    uint length() {
        return vec.size();
    }
    void push(const Tube& tube) {
        if (vec.size() < vec.capacity()) {
            vec.push_back(tube);
        }
        else {
            cerr << "Trying to push a full tube vector" << endl;
            exit(-1);
        }
    }
    Tube pop() {
        Tube last =  vec[vec.size()-1];
        vec.pop_back();
        return last;
    }

    void print() {
        cout << "Vector of tubes: " << endl;
        for (Tube& tube : vec) {
            cout << "\t";
            tube.print();
        }
    }
};

>>

<<
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

uint getLength(AST* child) {
    Iterator it = vars.find(child->text);
    if (it != vars.end()) {
        Tube* tube = dynamic_cast<Tube*>(it->second);
        if (tube) return tube->getLength();
        else {
            cout << "Wrong type, only tubes have length but " << child->text << " is a " << typeid(*it->second).name() << " instance" << endl;
            exit(-1);
        }
    }
    else {
        cout << "Var " << child->text << " does not exist" << endl;
        exit(-1);
    }
}

int evaluateExpresion(AST* root) {
    if (root->kind == "NUM") {
        return stoi(root->text);
    }
    else if (root->kind == "+") {
        return evaluateExpresion(root->down)+evaluateExpresion(root->down->right);
    }
    else if (root->kind == "-") {
        return evaluateExpresion(root->down)-evaluateExpresion(root->down->right);
    }
    else if (root->kind == "*") {
        return evaluateExpresion(root->down)*evaluateExpresion(root->down->right);
    }
    else if (root->kind == "LENGTH") {
        return getLength(root->down);
    }
    else if (root->kind == "DIAMETER") {

    }
}

void execute(AST* root) {
    if (root->kind == "=") {
        AST* childLeft = root->down;
        AST* childRight = childLeft->right;
        string& varName = childLeft->text;
        string& assignType = childRight->kind;
        if (assignType == "TUBE") {
            AST* lengthAST = childRight->down;
            AST* diameterAST = lengthAST->right;
            vars[varName] = new Tube(evaluateExpresion(lengthAST), evaluateExpresion(diameterAST));
        }
        else if (assignType == "CONNECTOR") {
            vars[varName] = new Connector(evaluateExpresion(childRight->down));
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
>>

#lexclass START
#token ARRAY "TUBEVECTOR\ OF"
#token TUBE "TUBE"
#token CONNECTOR "CONNECTOR"
#token SPLIT "SPLIT"
#token MERGE "MERGE"
#token LENGTH "LENGTH"
#token DIAMETER "DIAMETER"
#token WHILE "WHILE"
#token ENDWHILE "ENDWHILE"
#token NOT "NOT"
#token AND "AND"
#token OR "OR"
#token LPAREN "\("
#token RPAREN "\)"
#token COMMA ","
#token MINUS "\-"
#token ADD "\+"
#token MUL "\*"
#token PUSH "PUSH"
#token POP "POP"
#token FULL "FULL"
#token EMPTY "EMPTY"
#token ASSIG "="
#token GT ">"
#token LT "<"
#token EQ "=="
#token NUM "[0-9]+"
#token ID "[a-zA-Z][a-zA-Z0-9]*"
#token SPACE "[\ \n\t\r]" << zzskip();>>

plumber: listOp "@"!;

listOp: (op)* <<#0=createASTlist(_sibling);>>;

op : ID ASSIG^ (funcTube | funcConnector | funcArray)
    | LPAREN! ID COMMA! ID RPAREN! ASSIG^ funcSplit
    | WHILE^ LPAREN! boolExpr RPAREN! listOp ENDWHILE!
    | DIAMETER^ LPAREN! ID RPAREN!
    | LENGTH^ LPAREN! ID RPAREN!
    | PUSH^ ID funcTube
    | POP^ ID ID
    ;

funcSplit: SPLIT^ funcTube;

funcArray: ARRAY^ aritmeticExpr;

funcConnector: CONNECTOR^ aritmeticExpr;

funcTube : ID
    | MERGE^ funcTube (funcConnector | ID) funcTube
    | TUBE^ aritmeticExpr aritmeticExpr
    ;

boolExpr: andExpr (OR^ andExpr)*;
andExpr: notFunc (AND^ notFunc)*;
notFunc: (NOT^|) boolFunc;
boolFunc: aritmeticExpr ((GT^ | LT^ | EQ^) aritmeticExpr)*
    | FULL^ LPAREN! ID RPAREN!
    | EMPTY^ LPAREN! ID RPAREN!
    | LPAREN! boolExpr RPAREN!
    ;

aritmeticExpr: mulExpr ((MINUS^ | ADD^) mulExpr)*;
mulExpr: numFunc (MUL^ numFunc)*;
numFunc: NUM
    | LENGTH^ LPAREN! ID RPAREN!
    | DIAMETER^ LPAREN! ID RPAREN!
    ;
