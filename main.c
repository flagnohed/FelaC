#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


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
	char *valcpy = strdup (value);
	printf ("valcpy: %s \n", valcpy);	
	if (new == NULL) 
		return NULL;

	new->type = type;
	new->value = valcpy;
	new->next = tokenhead;

	tokenhead = new;
	
	return new;
}	

/* Print all tokens. */
void print_tokens () {
	token *cur = tokenhead;
	while (cur->next != NULL) {
		printf ("(%d, %s)<-", cur->type, cur->value);
		cur = cur->next;
	}
	if (cur == NULL)
		printf ("cur is null! \n");	
	printf ("(%d, %s) \n", cur->type, cur->value);
}

/* Print token at index i in the list. */
void print_token (size_t i) {
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

/* Tokenizes the .fc file and updates the token linked list (tokenhead). */
void tokenize (char contents[])
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
			i--;
			valuebuf[tmpi + 1] = '\0';
			printf ("valuebuf: %s\n", valuebuf);			
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

		// else
			// printf("Unrecognized char: %c. Continuing... \n", contents[i]);		
	}
}
/* ASS is funnier than ASM but mean the same thing. */
#define MAX_ASS_SIZE 64000
#define TMP_ASS_NAME "tmp.asm"
/* Creates an assembly file (and returns a pointer to its FILE object)
 * corresponding to the current tokens in tokenhead. Essentially, 
 * this is the parser. kinda. */ 
void tokens2asm () {
	/* T is our current token. Reverse to make parsing easier.  */
	reverse (&tokenhead);
	token *t = tokenhead, *next = NULL, *next2 = NULL;
	size_t i;
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
	
	tokenize (contents);	
	tokens2asm ();		
	
	/* Call nasm and ld. */
	char nasmbuf[256];
	char ldbuf[256];

	sprintf (nasmbuf, "nasm -felf64 %s", TMP_ASS_NAME);
	system (nasmbuf);

	sprintf (ldbuf, "ld -o out tmp.o");
	system (ldbuf);


	return EXIT_SUCCESS;
}
