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
using namespace std;

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
#define zzSET_SIZE 4
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[31]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"TUBE",
	/* 03 */	"CONNECTOR",
	/* 04 */	"SPLIT",
	/* 05 */	"MERGE",
	/* 06 */	"LENGTH",
	/* 07 */	"DIAMETER",
	/* 08 */	"WHILE",
	/* 09 */	"ENDWHILE",
	/* 10 */	"NOT",
	/* 11 */	"AND",
	/* 12 */	"OR",
	/* 13 */	"LPAREN",
	/* 14 */	"RPAREN",
	/* 15 */	"COMMA",
	/* 16 */	"MINUS",
	/* 17 */	"ADD",
	/* 18 */	"MUL",
	/* 19 */	"PUSH",
	/* 20 */	"POP",
	/* 21 */	"ARRAY",
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
SetWordType zzerr1[4] = {0x0,0x21,0x0,0x20};
SetWordType zzerr2[4] = {0x8,0x0,0x0,0x20};
SetWordType zzerr3[4] = {0x24,0x0,0x0,0x20};
SetWordType setwd1[31] = {0x0,0x5a,0x20,0x40,0x0,0x20,0x0,
	0x0,0x5d,0x58,0x0,0x0,0x0,0x5d,0x80,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x7d,0x0};
SetWordType zzerr4[4] = {0xc0,0x24,0xc0,0x10};
SetWordType zzerr5[4] = {0x0,0x0,0x0,0xe};
SetWordType zzerr6[4] = {0xc0,0x20,0xc0,0x10};
SetWordType zzerr7[4] = {0x0,0x0,0x3,0x0};
SetWordType setwd2[31] = {0x0,0x80,0x80,0x80,0x0,0x80,0x8a,
	0x8a,0x80,0x80,0x0,0xa4,0xa5,0x82,0xa5,
	0x0,0x40,0x40,0x0,0x0,0x0,0x0,0x2,
	0x2,0x0,0x90,0x90,0x90,0x8a,0x80,0x0};
SetWordType zzerr8[4] = {0xc0,0x0,0x0,0x10};
SetWordType setwd3[31] = {0x0,0x3,0x3,0x3,0x0,0x3,0x3,
	0x3,0x3,0x3,0x0,0x3,0x3,0x3,0x3,
	0x0,0x3,0x3,0x2,0x0,0x0,0x0,0x0,
	0x0,0x0,0x3,0x3,0x3,0x3,0x3,0x0};
