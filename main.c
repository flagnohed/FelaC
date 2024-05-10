#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "token.h"



/* List of tokens */
token *tokenhead = NULL;

void tokenize (char contents[], token **tokenhead);
void tokens2asm (token **tokenhead);


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
	
	tokenize (contents, &tokenhead);	
	tokens2asm (&tokenhead);		
	
	/* Call nasm and ld. */
	char nasmbuf[256];
	char ldbuf[256];
	int code;

	sprintf (nasmbuf, "nasm -felf64 %s", TMP_ASS_NAME);
	if ((code = system (nasmbuf)))
		printf ("system() returned %d. \n", code); 
		

	sprintf (ldbuf, "ld -o fcprogram %s", TMP_OBJ_NAME);
	if ((code = system (ldbuf)))
		printf ("system() returned %d. \n", code);


	return EXIT_SUCCESS;
}
