#pragma once
#include "token.h"



// https://keleshev.com/abstract-syntax-tree-an-example-in-c/


/* "We want data members to reference other AST nodes, 
and since C does not allow recursive type definitions, 
we make a forward reference by defining AST as struct AST. 
We can do this because the namespace of structs and the namespace 
of types are separate in C." */
typedef struct node_expr node_expr; 
typedef struct node_stmt node_stmt;
typedef struct node_prog node_prog;
/* C does not have tagged unions, so this is the closest we get to it. */
/* Name conflict between tokenizer and parser, so we insert _ before each */
struct node_expr {
    enum {
        _INT_LITERAL,
        _IDENT,
        _ADD,
        _MULTI
    }   expr_t;

    union {
        struct _INT_LITERAL { int val; } _INT_LITERAL;
        struct _IDENT { char *name; } _IDENT;
        struct _ADD {node_expr *lhs; node_expr *rhs};
        struct _MULT {node_expr *lhs; node_expr *rhs};
    }   data;

};



struct node_stmt {
    enum {
        _EXIT,
        _LET
    }   stmt_t;
    union {
        struct _EXIT { node_expr *expr; } _EXIT;    
        struct _LET { char *name; node_expr *expr; } _LET;     /* Variable declaration. */
    }   data;
    struct node_stmt *next;
};


void add_stmt (struct node_stmt **head, struct node_stmt *_new);


struct node_prog {
    /* A program is a list of statements. */

    node_stmt *head;  // pointer to the first statement.

};


node_expr *expr_new (node_expr expr);
node_stmt *stmt_new (node_stmt stmt);

#define EXPR_NEW(type, ...) \
    expr_new((node_expr){type, {.type=(struct type){__VA_ARGS__}}})

#define STMT_NEW(type, ...) \
    stmt_new((node_stmt){type, {.type=(struct type){__VA_ARGS__}}})


/* Token *t will be changed in these "helper"functions, so we are
parsing say three tokens, skipping them and after for example
parse_expr is done, that expression is no linger in the token list.
*/
node_prog *parse_prog (token **tokenhead);

node_stmt *parse_stmt (token *t);

node_expr *parse_expr (token *t);
