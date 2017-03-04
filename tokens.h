#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: plumber.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define ARRAY 2
#define TUBE 3
#define CONNECTOR 4
#define SPLIT 5
#define MERGE 6
#define LENGTH 7
#define DIAMETER 8
#define WHILE 9
#define ENDWHILE 10
#define NOT 11
#define AND 12
#define OR 13
#define LPAREN 14
#define RPAREN 15
#define COMMA 16
#define MINUS 17
#define ADD 18
#define MUL 19
#define PUSH 20
#define POP 21
#define FULL 22
#define EMPTY 23
#define ASSIG 24
#define GT 25
#define LT 26
#define EQ 27
#define NUM 28
#define ID 29
#define SPACE 30

#ifdef __USE_PROTOS
void plumber(AST**_root);
#else
extern void plumber();
#endif

#ifdef __USE_PROTOS
void listOp(AST**_root);
#else
extern void listOp();
#endif

#ifdef __USE_PROTOS
void op(AST**_root);
#else
extern void op();
#endif

#ifdef __USE_PROTOS
void funcSplit(AST**_root);
#else
extern void funcSplit();
#endif

#ifdef __USE_PROTOS
void funcArray(AST**_root);
#else
extern void funcArray();
#endif

#ifdef __USE_PROTOS
void funcConnector(AST**_root);
#else
extern void funcConnector();
#endif

#ifdef __USE_PROTOS
void funcTube(AST**_root);
#else
extern void funcTube();
#endif

#ifdef __USE_PROTOS
void boolExpr(AST**_root);
#else
extern void boolExpr();
#endif

#ifdef __USE_PROTOS
void andExpr(AST**_root);
#else
extern void andExpr();
#endif

#ifdef __USE_PROTOS
void notFunc(AST**_root);
#else
extern void notFunc();
#endif

#ifdef __USE_PROTOS
void boolFunc(AST**_root);
#else
extern void boolFunc();
#endif

#ifdef __USE_PROTOS
void aritmeticExpr(AST**_root);
#else
extern void aritmeticExpr();
#endif

#ifdef __USE_PROTOS
void mulExpr(AST**_root);
#else
extern void mulExpr();
#endif

#ifdef __USE_PROTOS
void numFunc(AST**_root);
#else
extern void numFunc();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType setwd1[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType setwd2[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType setwd3[];
