#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


/* Creates a token with attributes TYPE and VALUE and
 * adds it to the linked list of tokens. Returns a pointer
 * to the new token or NULL if something failed. */
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
	if (cur == NULL) return;
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
#define LET_STR "let"
#define BE_STR "be"

/* Tokenizes the .fc file and fills the token list. */
void tokenize (char contents[], token **tokenhead)
{	
	/* Index relative to current token being parsed. */
	size_t tmpi, i = 0;
	size_t contentsize = strlen (contents);
	token_t tt;
	token *t;
	/* A token cannot have a value larger than 
			the entire contents array. */
	char valuebuf[contentsize];
	/* Start the parsing loop. We wont go through the loop CONTENTSIZE times.
		When we find some interesting character, we start the actual tokenizing. 
		Then, we jump the size of token in terms of i. */
	while (i < contentsize) {
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
			
			/* Figure out what type this is. 
				Set tt to the correct token type. */
			if (strcmp (EXIT_STR, valuebuf) == 0) 
				tt = EXIT;
				
			else if (strcmp (LET_STR, valuebuf) == 0) 
				tt = LET;

			else if (strcmp (BE_STR, valuebuf) == 0) 
				tt = BE;
				
			else 
				/* If its not a built in keyword, it must be 
					a variable (or identifier if you will)*/
				tt = IDENT;
			
			/* Add token to the list. */
			if ((t = add_token (tt, valuebuf, tokenhead)) == NULL) {
					printf ("Could not add token %s. \n", valuebuf);
					exit (EXIT_FAILURE);
				}

			continue;
			
		
		}
		
		else if (isdigit (contents[i])) { 
			tmpi = 0;
			valuebuf[tmpi] = contents[i];
			/* Keep reading digits. */
			while (isdigit (contents[++i]))
				valuebuf[++tmpi] = contents[i];
			
			valuebuf[tmpi + 1] = '\0';

			tt = INT_LITERAL;
			if ((t = add_token (tt, valuebuf, tokenhead)) == NULL) {
				printf ("Could not add token '%s'. \n", valuebuf);
				exit (EXIT_FAILURE);
			}
			continue;
		}
		// maybe make this a switch statement?
		else if (contents[i] == '(') {
			tt = OPEN_PAREN;
			i++;
			if ((t = add_token (tt, "(", tokenhead)) == NULL) {
				printf ("Could not add token '('. \n");
				exit (EXIT_FAILURE);
			}
			continue;
		}

		else if (contents[i] == ')') {
			tt = CLOSE_PAREN;
			i++;
			if ((t = add_token (tt, ")", tokenhead)) == NULL) {
				printf ("Could not add token ')'. \n");
				exit (EXIT_FAILURE);
			}
			continue;
		}

		else if (contents[i] == ';') {
			tt = SEMICOLON;
			i++;
			if ((t = add_token (tt, ";", tokenhead)) == NULL) {
				printf ("Could not add token ';'. \n");
				exit (EXIT_FAILURE);
			}
			continue;
		}
		
		else if (contents[i] == ' ' || contents[i] == '\n') {
			i++;
			continue;
		}
			

		

		// else
			// printf("Unrecognized char: %c. Continuing... \n", contents[i]);		
	}
}

