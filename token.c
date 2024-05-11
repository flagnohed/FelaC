#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


/* Creates a token with attributes TYPE and VALUE and
 * adds it to the linked list of tokens. Returns a pointer
 * to the new token. */
token *add_token (token_t type, char *value, token **tokenhead) {

	token *new = malloc (sizeof (token));
	char *valcpy = strdup (value);
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
		printf ("'%s' ", cur->value);
		cur = cur->next;
	}
	printf ("'%s' \n", cur->value);
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


#define EXIT_STR "exit"

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
	while (i < contentsize) {
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
			i--;
			/* Figure out what type this is. */
			if (strcmp (EXIT_STR, valuebuf) == 0) {
				/* This is a return keyword, so add the
					token to the list. */
				tt = EXIT;
				if ((t = add_token (tt, valuebuf, tokenhead)) == NULL) {
					printf ("Could not add token %s. \n", valuebuf);
					exit (EXIT_FAILURE);
				}
				/* We are done with this token, so jump to the beginning of
					the for loop at the start of next token. */
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
			tt = INT_LITERAL;
			if ((t = add_token (tt, valuebuf, tokenhead)) == NULL) {
				printf ("Could not add token '%s'. \n", valuebuf);
				exit (EXIT_FAILURE);
			}
		}

		else if (contents[i] == '(') {
			tt = OPEN_PAREN;
			if ((t = add_token (tt, "(", tokenhead)) == NULL) {
				printf ("Could not add token '('. \n");
				exit (EXIT_FAILURE);
			}
		}

		else if (contents[i] == ')') {
			tt = CLOSE_PAREN;
			if ((t = add_token (tt, ")", tokenhead)) == NULL) {
				printf ("Could not add token ')'. \n");
				exit (EXIT_FAILURE);
			}
		}

		else if (contents[i] == ';') {
			tt = SEMICOLON;
			if ((t = add_token (tt, ";", tokenhead)) == NULL) {
				printf ("Could not add token ';'. \n");
				exit (EXIT_FAILURE);
			}
		}
		
		else if (contents[i] == ' ') 
			continue;

		

		// else
			// printf("Unrecognized char: %c. Continuing... \n", contents[i]);		
	}
}

