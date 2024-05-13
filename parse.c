#include "parse.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* Looks for an expression in the provided token list t. Returns 
1 if expression found, 0 otherwise. Takes an empty node_expr as input and the 
token chain with the token(s) to be evaluated as t. */
node_expr *parse_expr (token *t) {
    node_expr *expr = NULL;
    /* Token IDENT and expr_t IDENT are not the same technically. */
    if (t->type == INT_LITERAL)
        expr = EXPR_NEW(_INT_LITERAL, atoi(t->value));
    else if (t->type == IDENT) {

        // todo
    }

    else {
        printf ("Unrecognized expression found while parsing. \n");
        exit (EXIT_FAILURE);
    }

    return expr;
}

/* AST *ast_new(AST ast) {
  AST *ptr = malloc(sizeof(AST));
  if (ptr) *ptr = ast;
  return ptr;
}   */


node_expr *expr_new (node_expr expr) {
    node_expr *ptr = malloc (sizeof (node_expr));
    if (ptr) *ptr = expr;
    return ptr;
}

node_stmt *stmt_new (node_stmt stmt) {
    node_stmt *ptr = malloc (sizeof (node_stmt));
    if (ptr) *ptr = stmt;
    return ptr;
}

/*
should have parse_head, so we send in tokenhead, and then cut of 
branches in the tree (like if the first token is exit, then
the second one must be parenthesis, if the first is LET then
the next one must be an identifier, and so on. have a switch stmt
in parse_head over token_t type. )

*/

node_stmt *parse_stmt (token *t) {
    node_stmt *stmt;
    node_expr *nexpr;
    // char *ident_name = NULL;


    if (t->type == EXIT) {
        t = t->next;
        if (t->type != OPEN_PAREN) {
            printf ("Expected '('. \n");
            exit (EXIT_FAILURE);
        }

        t = t->next;
        if ((nexpr = parse_expr (t)) == NULL) {
            printf ("Expected an expression. \n");
            exit (EXIT_FAILURE);
        }

        t = t->next;
        if (t->type != CLOSE_PAREN) {
            printf ("Expected ')'. \n");
            exit (EXIT_FAILURE);
        }

        t = t->next;
        if (t->type != SEMICOLON) {
            printf ("Expected semicolon. \n");
            exit (EXIT_FAILURE);
        }
        t = t->next;
        stmt = STMT_NEW(_EXIT, nexpr);
    }

    else if (t->type == LET) {
        t = t->next;

        if (t->type != IDENT) {
            printf("Expected an identifier. \n");
            exit (EXIT_FAILURE);
        }
        /* Save the name of the identifier. */
        
        
        t = t->next;
        if (t->type != BE) {
            printf("Expected the 'be' keyword after identifier. \n");
            exit (EXIT_FAILURE);
        }

        t = t->next;
        if ((nexpr = parse_expr (t)) == NULL) {
            printf ("Expected an expression. \n");
            exit (EXIT_FAILURE);
        }
        t = t->next;
        if (t->type != SEMICOLON) {
            printf ("Expected semicolon. \n");
            exit (EXIT_FAILURE);
        }
        t = t->next;

        stmt = STMT_NEW(_LET, nexpr);
    }

    else {
        printf ("Error parsing statement. \n");
        exit (EXIT_FAILURE);
    }

    return stmt;
}

/* Adds node_stmt NEW to stmt list pointed to by HEAD. 
When creating a statement with STMT_NEW, the next pointer is NULL.
So we need to do it here to have the correct order in node_prog. */
node_stmt *add_stmt (node_stmt **head, node_stmt *new) {
    new->next = *head;
    *head = new;
    return new;
}


node_prog *parse_prog (token *t) {
    node_prog *prog = malloc (sizeof (node_prog));
    node_stmt *stmt;
    reverse (&t);

    while (t != NULL) {
        /* Look for statements to add to the statement list. */
        if (t->type == EXIT || t->type == LET) {
            printf ("adding exit statement\n");
            stmt = parse_stmt (t);
            add_stmt (&prog->head, stmt);
        }

        t = t->next;
    }


    return prog;
}