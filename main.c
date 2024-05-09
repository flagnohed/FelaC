#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

typedef enum {
	RETURN,
	INT_LITERAL,
	SEMICOLON
}	token_t;
	
typedef struct {
	token_t type;	/* Token type. */
	char *value;
	void *next;		/* Pointer to the next token. */
}	token;

/* List of tokens */
token *tokenhead = NULL;

/* Creates a token with attributes TYPE and VALUE and
 * adds it to the linked list of tokens. Returns a pointer
 * to the new token. */
token *add_token (token_t type, char *value) {

	token *new = malloc (sizeof (token));
		
	if (new == NULL) 
		return NULL;

	new->type = type;
	new->value = value;
	new->next = tokenhead;

	tokenhead = new;
	
	return new;
}	

#define RET_STR "return"

/* Tokenizes the .fc file and updates the token linked list (tokenhead). */
void tokenize (char contents[])
{	
	
	

	/* Index relative to current token being parsed. */
	size_t tmpi, i;
	size_t contentsize = strlen (contents);
	token_t tt;
	token *t;

	/* Start the parsing loop. We wont go through the loop CONTENTSIZE times.
		When we find some interesting character, we start the actual tokenizing. 
		Then, we jump the size of token in terms of i. */
	for (i = 0; i < contentsize; i++) {
		/* A token cannot have a value larger than 
			the entire contents array. */
		char valuebuf[contentsize];
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
			

			/* Figure out what type this is. */
			if (strcmp (RET_STR, valuebuf) == 0) {
				/* This is a return keyword, so add the
					token to the list. */
				tt = RETURN;
				if ((t = add_token (tt, valuebuf)) == NULL) {
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
			
			valuebuf[tmpi + 1] = '\0';
			
			tt = INT_LITERAL;
			if ((t = add_token (tt, valuebuf)) == NULL) {
				printf ("Could not add token %s. \n", valuebuf);
				exit (EXIT_FAILURE);
			}
			continue;
			
		}

		else if (contents[i] == ';') {
			tt = SEMICOLON;
			if ((t = add_token (tt, ";")) == NULL) {
				printf ("Could not add semicolon as token. \n");
				exit (EXIT_FAILURE);
			}
		}
		
		else if (contents[i] == ' ') 
			continue;		
	}
}



void print_info_and_exit (int exit_code)
{
	printf ("Usage: felac <file.fc> \n");
	exit (exit_code);
}


int main (int argc, char **argv) 
{
	if (argc != 2) {
		printf ("Invalid number of arguments. \n");
		print_info_and_exit (EXIT_FAILURE);
		
	}
	
	/* Complain if wrong extension. */
	char *filename = argv[1];
	size_t s = strlen (filename);
	if (filename[s-3] != '.' || 
		filename[s-2] != 'f' || 
		filename[s-1] != 'c') {
			printf ("Unrecognized file format. \n");
			print_info_and_exit (EXIT_FAILURE);
	}  
		
	/* We assume everything is good to go here, get the file! */
	FILE *fp;
	
	fp = fopen (filename, "r");
	if (fp == NULL) {
		printf ("The specified file was not found. \n");
		exit (EXIT_FAILURE);
	}
	
	/* Get the size of the file.  */
	long int sz;
	fseek (fp, 0L, SEEK_END);
	sz = ftell (fp);
	rewind (fp);

	/* Create a buffer for the file's contents. */
	if (sz <= 0) {
		printf ("Invalid file size. \n");
		exit (EXIT_FAILURE);
	}
	char contents[sz];	
	
	int cur, chars_added = 0;
	while ((cur = fgetc (fp)) != EOF) {
		contents[chars_added] = cur;
		chars_added++;
	}
	contents[sz] = '\0';
	
	printf ("%s \n", contents);				
	
	tokenize (contents);	
			
	printf ("first token value: %s\n", tokenhead->value);
	// printf ("second token value: %s\n", tokenhead->next->value);

	return EXIT_SUCCESS;
}
