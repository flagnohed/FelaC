#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


/* Creates an assembly file (and returns a pointer to its FILE object)
 * corresponding to the current tokens in tokenhead. Essentially, 
 * this is the parser. kinda. */ 
void tokens2asm (token **tokenhead) {
	/* T is our current token. Reverse to make parsing easier.  */
	reverse (tokenhead);
	token *t = *tokenhead, *next = NULL, *next2 = NULL;
	// size_t i;
	char asscode[MAX_ASS_SIZE] = "";
	
	/* This is the main loop for tyring to figure out how to write 
	the FelaC code in assembly. After exiting the loop, asscode buffer
	contains the assembly code. */
	while (t != NULL) {
		
		if (t->type == RETURN) {
			next = t->next;
			if (next->type == INT_LITERAL) {
				next2 = next->next;
				if (next2->type == SEMICOLON) {
					/* We want to return next->value, and 
						since next2->type is a semicolon, we know that
						the line is done, so we can create the assembly code
						for it, which is placed in the asscode buffer. */
					sprintf (asscode, 
							"global _start\n_start:\n\tmov rax, 60\n\tmov rdi, %s\n\tsyscall\n",
							next->value);
					// free (t);
					// free (next);
					// free (next2);
					break;
				}
			}
		}
	}
	FILE *assfile;
	int r, c;
	/* Open or create tmp.asm. W means write over any existing text, 
		W+ means append to existing text (if any). */
	assfile = fopen ("tmp.asm", "w+");
	if (assfile == NULL) 
		printf ("Failed to create 'tmp.asm'. \n");
	
	/* Write asscode to assfile. */
	if ((r = fputs (asscode, assfile)) == EOF) {
		printf ("Failed to write to file. \n");
		exit (EXIT_FAILURE);
	}

	c = fclose (assfile);
	assert (c == 0);
	
}





/* Creates a token with attributes TYPE and VALUE and
 * adds it to the linked list of tokens. Returns a pointer
 * to the new token. */
token *add_token (token_t type, char *value, token **tokenhead) {

	token *new = malloc (sizeof (token));
	char *valcpy = strdup (value);
	printf ("valcpy: %s \n", valcpy);	
	if (new == NULL) 
		return NULL;

	new->type = type;
	new->value = valcpy;
	new->next = *tokenhead;

	*tokenhead = new;
	
	return new;
}	

/* Print all tokens. */
void print_tokens (token **tokenhead) {
	token *cur = *tokenhead;
	while (cur->next != NULL) {
		printf ("(%d, %s)<-", cur->type, cur->value);
		cur = cur->next;
	}
	if (cur == NULL)
		printf ("cur is null! \n");	
	printf ("(%d, %s) \n", cur->type, cur->value);
}



/* Reverses to the correct order. When inserting, the first token added
 * is located at the tail (stack) */
void reverse (token **head) {
	token *prev = NULL;
	token *cur = *head;
	token *next = NULL;

	while (cur != NULL) {
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}
	*head = prev;
}


#define RET_STR "return"

/* Tokenizes the .fc file and fills the token list. */
void tokenize (char contents[], token **tokenhead)
{	
	/* Index relative to current token being parsed. */
	size_t tmpi, i;
	size_t contentsize = strlen (contents);
	token_t tt;
	token *t;
	char valuebuf[contentsize];
	/* Start the parsing loop. We wont go through the loop CONTENTSIZE times.
		When we find some interesting character, we start the actual tokenizing. 
		Then, we jump the size of token in terms of i. */
	for (i = 0; i < contentsize; i++) {
		/* A token cannot have a value larger than 
			the entire contents array. */
		
		 
		/* If C is a letter, start reading into VALUEBUF */
		if (isalpha (contents[i])) {
			tmpi = 0;	
			valuebuf[tmpi] = contents[i];
		
			/* Keep looking for either letters or numbers. 
				For example, variables can not start with a number 
				but can contain it. */
			while (isalnum (contents[++i])) 
				valuebuf[++tmpi] = contents[i];
			
			valuebuf[tmpi + 1] = '\0';	
			printf ("valuebuf: %s \n", valuebuf);
			i--;
			/* Figure out what type this is. */
			if (strcmp (RET_STR, valuebuf) == 0) {
				/* This is a return keyword, so add the
					token to the list. */
				tt = RETURN;
				if ((t = add_token (tt, valuebuf, tokenhead)) == NULL) {
					printf ("Could not add token %s. \n", valuebuf);
					exit (EXIT_FAILURE);
				}
				/* We are done with this token, so jump to the beginning of
					the for loop at the start of next token. */
				
			
				continue;
			} 

			else {
				printf ("Undefined keyword found, syntax error! \n");
				exit (EXIT_FAILURE);
			}
		}
 
		else if (isdigit (contents[i])) { 
			tmpi = 0;
			valuebuf[tmpi] = contents[i];
			/* Keep reading digits. */
			while (isdigit (contents[++i]))
				valuebuf[++tmpi] = contents[i];
			i--;
			valuebuf[tmpi + 1] = '\0';
			printf ("valuebuf: %s\n", valuebuf);			
			tt = INT_LITERAL;
			if ((t = add_token (tt, valuebuf, tokenhead)) == NULL) {
				printf ("Could not add token %s. \n", valuebuf);
				exit (EXIT_FAILURE);
			}
			continue;
			
		}

		else if (contents[i] == ';') {
			tt = SEMICOLON;
			if ((t = add_token (tt, ";", tokenhead)) == NULL) {
				printf ("Could not add semicolon as token. \n");
				exit (EXIT_FAILURE);
			}
		}
		
		else if (contents[i] == ' ') 
			continue;

		// else
			// printf("Unrecognized char: %c. Continuing... \n", contents[i]);		
	}
}

