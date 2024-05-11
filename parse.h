#pragma once
#include "token.h"

typedef enum {
    INT_LITERAL,
    IDENT
}   expr_t;

// https://keleshev.com/abstract-syntax-tree-an-example-in-c/


/* "We want data members to reference other AST nodes, 
and since C does not allow recursive type definitions, 
we make a forward reference by defining AST as struct AST. 
We can do this because the namespace of structs and the namespace 
of types are separate in C." */
typedef struct node_expr node_expr; 
typedef struct node_stmt node_stmt;

typedef struct {
    expr_t type;

    union {
        struct INT_LITERAL { token *t; } INT_LITERAL;
        struct IDENT { token *t; } IDENT;
    }   data;

}   node_expr;

typedef enum {
    EXIT,
    LET
}   stmt_t;

typedef struct {
    stmt_t type;
    void *next;
    union {
        struct EXIT { node_expr *expr; } EXIT;
        struct LET { node_expr *ident; node_expr *expr; } LET;
    }   data;

}   node_stmt;

typedef struct {
    
}   node_prog;

#define EXPR_NEW(type, ...) \
    expr_new((node_expr){type, {.type=(struct type){__VA_ARGS__}}})

#define STMT_NEW(type, ...) \
    stmt_new((node_stmt){type, {.type=(struct type){__VA_ARGS__}}})



node_prog *parse_exit (token **tokenhead);

node_expr *parse_expr (token *t);
