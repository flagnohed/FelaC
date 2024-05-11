#include "codegen.h"
#include <stdio.h>


void generate_exit (node_exit *ne, char asscode[]) {
    if (ne == NULL)
        printf("ne is null\n");
    
    if (ne->ne == NULL)
        printf("no expression\n");

    /* Put error code in rdi, get it from expression within exit node. */
    sprintf (asscode, "global _start\n_start:\n\tmov rax, 60\n\tmov rdi, %d\n\tsyscall\n", ne->ne->int_literal);
}