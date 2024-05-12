#include "codegen.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>


// void generate_exit (node_exit *ne, char asscode[]) {
//     if (ne == NULL)
//         printf("ne is null\n");
    
//     if (ne->ne == NULL)
//         printf("no expression\n");

//     /* Put error code in rdi, get it from expression within exit node. */
//     sprintf (asscode, "global _start\n_start:\n\tmov rax, 60\n\tmov rdi, %d\n\tsyscall\n", ne->ne->int_literal);
// }





void gen_exit (struct EXIT *e, char asscode[]) {




}

void generate_code (node_prog *prog) {
    node_stmt *head = prog->head;
    char asscode[MAX_ASS_SIZE] = "";
    size_t sz = 0;
    // send stream around the helper functions to keep writing 
    // assembly code to asscode buffer. 
    FILE *stream = open_memstream (&asscode, &sz);
    // struct AST_NUMBER data = ast.data.AST_NUMBER;
    /* Go through each statement, call appropriate functions. */
    while (head != NULL) {

        switch (head->stmt_t) {
            case EXIT:
                struct EXIT e = head->data.EXIT;
                gen_exit (&e, asscode);
                break;
            case LET:
                gen_let (head);
                break;
            default:
                printf ("Unrecognized statement type. \n");
                exit (EXIT_FAILURE);
        }
    }
}