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
#define SPACE 2
#define TUNE 3
#define CONNECTOR 4
#define SPLIT 5
#define MERGE 6
#define LENGHT 7
#define DIAMETER 8
#define WHILE 9
#define ENDWHILE 10
#define NOT 11
#define AND 12
#define OR 13
#define LPAREN 14
#define RPAREN 15
#define COMMA 16
#define PUSH 17
#define POP 18
#define ARRAY 19
#define FULL 20
#define EMPTY 21
#define ASSIG 22
#define GT 23
#define LT 24
#define EQ 25
#define NUM 26
#define ID 27
#define TUBE 28

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
void expr(AST**_root);
#else
extern void expr();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType setwd1[];
