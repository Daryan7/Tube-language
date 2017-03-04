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
#define TUBE 2
#define CONNECTOR 3
#define SPLIT 4
#define MERGE 5
#define LENGTH 6
#define DIAMETER 7
#define WHILE 8
#define ENDWHILE 9
#define NOT 10
#define AND 11
#define OR 12
#define LPAREN 13
#define RPAREN 14
#define COMMA 15
#define MINUS 16
#define ADD 17
#define MUL 18
#define PUSH 19
#define POP 20
#define ARRAY 21
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
void op(AST**_root);
#else
extern void op();
#endif

#ifdef __USE_PROTOS
void funcTubes(AST**_root);
#else
extern void funcTubes();
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
extern SetWordType setwd1[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType setwd2[];
extern SetWordType zzerr8[];
extern SetWordType setwd3[];
