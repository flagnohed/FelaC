#include "parse.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/* Looks for an expression in the provided token list t. Returns 
1 if expression found, 0 otherwise. Takes an empty node_expr as input and the 
token chain with the token(s) to be evaluated as t. */
node_expr *parse_expr (token *t) {
    token *tp = t;
    node_expr *ne = malloc (sizeof (node_expr));
    if (tp->type == INT_LITERAL) {
        ne->has_value = 1;
        ne->int_literal = atoi (tp->value);  
        return ne;  
    }
    
    return NULL;
}

/* sprintf (asscode, "global _start\n_start:\n\tmov rax, 60\n\tmov rdi, %s\n\tsyscall\n", next->value);*/

/* Creates an assembly file (and returns a pointer to its FILE object)
 * corresponding to the current tokens in tokenhead. Essentially, 
 * this is the parser. kinda. */ 
node_exit *parse (token *tokenhead) {
	/* T is our current token. Reverse to make parsing easier.  */
	reverse (tokenhead);
	token *next = NULL, *next2 = NULL;
	// size_t i;
	char asscode[MAX_ASS_SIZE] = "";
	unsigned int r;
    node_expr *nexpr = NULL;
    node_exit *nexit = NULL;
	/* This is the main loop for trying to figure out how to write 
	the FelaC code in assembly. After exiting the loop, asscode buffer
	contains the assembly code. */
	while (tokenhead != NULL) {
		
		if (tokenhead->type == EXIT) {
            tokenhead = tokenhead->next;
            if ((nexpr = parse_expr (tokenhead)) == NULL) {
                printf ("Expected an expression after exit. \n");
                exit (EXIT_FAILURE);
            }

            nexit = malloc (sizeof (node_exit));
            nexit->ne = nexpr;

            tokenhead = tokenhead->next;
            if (tokenhead->type != SEMICOLON) {
                printf ("Expected semicolon. \n");
                exit (EXIT_FAILURE);
            } 

            return nexit;
		}
	}
    return NULL;
	// FILE *assfile;
	// int r, c;
	// /* Open or create tmp.asm. W means write over any existing text, 
	// 	W+ means append to existing text (if any). */
	// assfile = fopen ("build/tmp.asm", "w+");
	// if (assfile == NULL) 
	// 	printf ("Failed to create 'build/tmp.asm'. \n");
	
	// /* Write asscode to assfile. */
	// if ((r = fputs (asscode, assfile)) == EOF) {
	// 	printf ("Failed to write to file. \n");
	// 	exit (EXIT_FAILURE);
	// }

	// c = fclose (assfile);
	// assert (c == 0);
	
}
