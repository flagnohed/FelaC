#pragma once
#include "token.h"

typedef struct {
    unsigned int has_value;     
    int int_literal;
}   node_expr;

typedef struct {
    node_expr *ne;
}   node_exit;

node_exit *parse (token **tokenhead);

node_expr *parse_expr (token *t);
