/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: plumber.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

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

#define zzSET_SIZE 4
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[31]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"ARRAY",
	/* 03 */	"TUBE",
	/* 04 */	"CONNECTOR",
	/* 05 */	"SPLIT",
	/* 06 */	"MERGE",
	/* 07 */	"LENGTH",
	/* 08 */	"DIAMETER",
	/* 09 */	"WHILE",
	/* 10 */	"ENDWHILE",
	/* 11 */	"NOT",
	/* 12 */	"AND",
	/* 13 */	"OR",
	/* 14 */	"LPAREN",
	/* 15 */	"RPAREN",
	/* 16 */	"COMMA",
	/* 17 */	"MINUS",
	/* 18 */	"ADD",
	/* 19 */	"MUL",
	/* 20 */	"PUSH",
	/* 21 */	"POP",
	/* 22 */	"FULL",
	/* 23 */	"EMPTY",
	/* 24 */	"ASSIG",
	/* 25 */	"GT",
	/* 26 */	"LT",
	/* 27 */	"EQ",
	/* 28 */	"NUM",
	/* 29 */	"ID",
	/* 30 */	"SPACE"
};
SetWordType zzerr1[4] = {0x5c,0x0,0x0,0x20};
SetWordType zzerr2[4] = {0x80,0x43,0x30,0x20};
SetWordType zzerr3[4] = {0x10,0x0,0x0,0x20};
SetWordType zzerr4[4] = {0x48,0x0,0x0,0x20};
SetWordType setwd1[31] = {0x0,0xf5,0x0,0x88,0x0,0x0,0x88,
	0xf2,0xf2,0xf2,0xf4,0x0,0x0,0x0,0xf2,
	0x0,0x0,0x0,0x0,0x0,0xf2,0xf2,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0xfa,0x0};
SetWordType zzerr5[4] = {0x80,0x49,0xc0,0x10};
SetWordType zzerr6[4] = {0x0,0x0,0x0,0xe};
SetWordType zzerr7[4] = {0x80,0x41,0xc0,0x10};
SetWordType setwd2[31] = {0x0,0x1,0x0,0x0,0x1,0x0,0x0,
	0x29,0x29,0x1,0x1,0x0,0x90,0x94,0x9,
	0x96,0x0,0x0,0x0,0x0,0x1,0x1,0x8,
	0x8,0x0,0x40,0x40,0x40,0x28,0x1,0x0};
SetWordType zzerr8[4] = {0x0,0x0,0x6,0x0};
SetWordType zzerr9[4] = {0x80,0x1,0x0,0x10};
SetWordType setwd3[31] = {0x0,0xe,0x0,0xe,0xe,0x0,0xe,
	0xe,0xe,0xe,0xe,0x0,0xe,0xe,0xe,
	0xe,0x0,0xd,0xd,0x8,0xe,0xe,0x0,
	0x0,0x0,0xe,0xe,0xe,0xe,0xe,0x0};
