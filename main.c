#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "token.h"
#include "parse.h"
#include "codegen.h"


/* List of tokens */
token *tokenhead = NULL;


void print_info_and_exit (int exit_code)
{
	printf ("Usage: make clean && make && ./felac <file.fc> && ./fcprogram\n");
	exit (exit_code);
}

#define EXEC_NAME "fcprogram"

int main (int argc, char **argv) 
{
	if (argc != 2) {
		printf ("Invalid number of arguments. \n");
		print_info_and_exit (EXIT_FAILURE);
		
	}
	char *filename = argv[1];
	/* Check for '-h' --> display usage. */
	if (strcmp (filename, "-h") == 0)
		print_info_and_exit (EXIT_SUCCESS);
	
	/* Complain if wrong extension. */
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
	
	printf ("Tokenizing...\n");
	tokenize (contents, &tokenhead);
	print_tokens(&tokenhead);
	printf ("Parsing...\n");	
	node_prog *prog = parse_prog (tokenhead);
	printf ("Generating assembly...\n");		
	generate_code (prog); 
	
	/* Call nasm and ld. */
	char nasmbuf[256];
	char ldbuf[256];
	int code;

	sprintf (nasmbuf, "nasm -felf64 %s", TMP_ASS_NAME);
	if ((code = system (nasmbuf))) {
		printf ("nasm error: system() returned %d. \n", code); 
		exit (EXIT_FAILURE);
	}
		
	sprintf (ldbuf, "ld -o %s %s", EXEC_NAME, TMP_OBJ_NAME);
	if ((code = system (ldbuf))) {
		printf ("ld error: system() returned %d. \n", code);
		exit (EXIT_FAILURE);
	}
		
	printf ("Compilation done. Main executable: fcprogram. \n");
	return EXIT_SUCCESS;
}
