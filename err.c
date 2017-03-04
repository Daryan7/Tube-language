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
typedef struct {
  string kind;
  string text;
} Attrib;

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

ANTLRChar *zztokens[29]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"SPACE",
	/* 03 */	"TUNE",
	/* 04 */	"CONNECTOR",
	/* 05 */	"SPLIT",
	/* 06 */	"MERGE",
	/* 07 */	"LENGHT",
	/* 08 */	"DIAMETER",
	/* 09 */	"WHILE",
	/* 10 */	"ENDWHILE",
	/* 11 */	"NOT",
	/* 12 */	"AND",
	/* 13 */	"OR",
	/* 14 */	"LPAREN",
	/* 15 */	"RPAREN",
	/* 16 */	"COMMA",
	/* 17 */	"PUSH",
	/* 18 */	"POP",
	/* 19 */	"ARRAY",
	/* 20 */	"FULL",
	/* 21 */	"EMPTY",
	/* 22 */	"ASSIG",
	/* 23 */	"GT",
	/* 24 */	"LT",
	/* 25 */	"EQ",
	/* 26 */	"NUM",
	/* 27 */	"ID",
	/* 28 */	"TUBE"
};
SetWordType zzerr1[4] = {0x0,0x40,0x0,0x8};
SetWordType zzerr2[4] = {0x40,0x0,0x0,0x18};
SetWordType setwd1[29] = {0x0,0x36,0x0,0x0,0x30,0x0,0x28,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x35,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x20,0x3d,0x28};
