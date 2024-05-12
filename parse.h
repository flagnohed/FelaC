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

/* C does not have tagged unions, so this is the closest we get to it. */

typedef struct {
    enum {
        INT_LITERAL,
        IDENT
}   expr_t;

    union {
        struct INT_LITERAL { int val; } INT_LITERAL;
        struct IDENT { char *name; node_expr *val; } IDENT;
    }   data;

}   node_expr;



typedef struct {
    enum {
        EXIT,
        LET
}   stmt_t;
    union {
        struct EXIT { node_expr *expr; } EXIT;
        struct LET { node_expr *ident; node_expr *expr; } LET;
    }   data;
    node_stmt *next;
}   node_stmt;


node_stmt *add_stmt (node_stmt **head, node_stmt *new);


typedef struct {
    /* A program is a list of statements. */

    node_stmt *head;  // pointer to the first statement.

}   node_prog;







#define EXPR_NEW(type, ...) \
    expr_new((node_expr){type, {.type=(struct type){__VA_ARGS__}}})

#define STMT_NEW(type, ...) \
    stmt_new((node_stmt){type, {.type=(struct type){__VA_ARGS__}}})


/* Token *t will be changed in these "helper"functions, so we are
parsing say three tokens, skipping them and after for example
parse_expr is done, that expression is no linger in the token list.
*/
node_prog *parse_prog (token *t);

node_stmt *parse_stmt (token *t);

node_expr *parse_expr (token *t);
