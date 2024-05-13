#include "codegen.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// void generate_exit (node_exit *ne, char asscode[]) {
//     if (ne == NULL)
//         printf("ne is null\n");
    
//     if (ne->ne == NULL)
//         printf("no expression\n");

//     /* Put error code in rdi, get it from expression within exit node. */
//     sprintf (asscode, "global _start\n_start:\n\tmov rax, 60\n\tmov rdi, %d\n\tsyscall\n", ne->ne->int_literal);
// }



int eval_expr (node_expr *expr) {
    if (expr->expr_t == _INT_LITERAL) {
        struct _INT_LITERAL intlit = expr->data._INT_LITERAL;
        return intlit.val; 
    }

    else {
        printf ("Could not evaluate expression. \n");
        exit (EXIT_FAILURE);
    }
}



void gen_exit (FILE *stream, struct _EXIT *e) {
    printf("in gen_exit()\n");
    fprintf (stream, "global _start\n");
    fflush (stream);
    fprintf (stream, "_start:\n");
    fflush (stream);
    /* 60 is the syscall number for exit. */
    fprintf (stream, "\tmov rax, 60\n");
    fflush (stream);
    /* Put the evaluated expression as an int into rdi (error code). */
    fprintf (stream, "\tmov rdi, %d\n", eval_expr (e->expr));
    fflush (stream);
    fprintf (stream, "\tsyscall\n");
    fflush (stream);
}

void generate_code (node_prog *prog) {
    node_stmt *head = prog->head;
    size_t sz = 0;
    int r, c;
    // send stream around the helper functions to keep writing 
    // assembly code to asscode buffer. 
    FILE *stream, *assfile;
    char *asscode;
	printf("%d\n", prog->head == NULL);  //1
    stream = open_memstream (&asscode, &sz);
    // struct AST_NUMBER data = ast.data.AST_NUMBER;
    /* Go through each statement, call appropriate functions 
    and fill asscode. */
    while (head != NULL) {

        switch (head->stmt_t) {
            case EXIT:
                struct _EXIT e = head->data._EXIT;
                gen_exit (stream, &e);
                break;
            case LET:
              
                break;
            default:
                printf ("Unrecognized statement type. \n");
                exit (EXIT_FAILURE);
        }

        head = head->next;
    }
    fclose (stream);
    printf ("generated code: \n%s\n", asscode);
    /* Now we have placed the assembly code into asscode buffer. 
        Now we need to write it to the asm file. */

    /* The assembly code is now in asscode. Put it in a file. */
	
	/* Open or create tmp.asm. W means write over any existing text, 
		W+ means append to existing text (if any). */
	assfile = fopen ("build/tmp.asm", "w+");
	if (assfile == NULL) 
		printf ("Failed to create 'build/tmp.asm'. \n");
	
	/* Write asscode to assfile. */
	if ((r = fputs (asscode, assfile)) == EOF) {
		printf ("Failed to write to file. \n");
		exit (EXIT_FAILURE);
	}

	c = fclose (assfile);
	assert (c == 0);
}