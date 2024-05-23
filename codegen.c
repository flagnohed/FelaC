#include "codegen.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

// static size_t hash_entries = 0;


int eval_expr (node_expr *expr, hashmap *map) {
    /* Base case. If its just a number, return it. */
    if (expr->expr_t == _INT_LITERAL) {
        struct _INT_LITERAL intlit = expr->data._INT_LITERAL;
        return intlit.val; 
    }
    /* Get the expression of variable, evaluate it. */
    else if (expr->expr_t == _IDENT) {
        struct _IDENT id = expr->data._IDENT;
        return get_value (map, id.name); 
        
    }
    else {
        printf ("Could not evaluate expression. \n");
        exit (EXIT_FAILURE);
    }
}


void gen_exit(FILE *stream, node_expr *expr, hashmap *map) {
    /* 60 is the syscall number for exit. */
    fprintf (stream, "\tmov rax, 60\n"); fflush (stream);
    /* Put the evaluated expression as an int into rdi (error code). */
    fprintf (stream, "\tmov rdi, %d\n", eval_expr (expr, map)); fflush (stream);
    fprintf (stream, "\tsyscall\n"); fflush (stream);
}

/* If variable does not exist yet, add it to HT. We dont need to
    create assembly code for this, as we keep track of variables in
    the hash table. */
// void add_ident (FILE *stream, char *name, node_expr *expr, hashmap *map) {
//     /* Make sure that we havent seen this variable before. */
//     if (hash_lookup (map, name) != NULL) {
//         printf ("Identifier %s has already been declared. \n", name);
//         exit (EXIT_FAILURE);
//     }
//     value_s *val = calloc (1, sizeof (value_s));
//     val->data = eval_expr (expr, ht);
//     hash_insert (ht, name, val);
// }

void gen_intlit (FILE *stream, node_expr *expr, hashmap *map) {
    fprintf (stream, "\tmov rax, %d\n", eval_expr (expr, map));
    fflush (stream);
    push (stream, "rax");
}

// push REG to the stack
void push (FILE *stream, const char *reg) {
    fprintf (stream, "\tpush %s\n", reg);
    fflush (stream);
}

// pop the top of the stack into REG
void pop (FILE *stream, const char *reg) {
    fprintf (stream, "\tpop %s", reg);
    fflush (stream);
}

/* Reverses a linked list of node statements (a program). */
void stmt_reverse (node_stmt **head) {
	node_stmt *prev = NULL;
	node_stmt *cur = *head;
	node_stmt *next = NULL;

	while (cur != NULL) {
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}
	*head = prev;
}


void generate_code (node_prog *prog) {
    node_stmt *head = prog->head;
    stmt_reverse (&head);
    size_t sz = 0;
    size_t hsz = 10;
    int r, c;

    hashmap *map = create_hashmap (hsz);
    FILE *stream, *assfile;
    char *asscode;
    stream = open_memstream (&asscode, &sz);
    fprintf (stream, "global _start\n"); fflush (stream);
    fprintf (stream, "_start:\n"); fflush (stream);
    /* Go through each statement, call appropriate functions 
    and fill asscode. */
    while (head != NULL) {

        switch (head->stmt_t) {
            case _EXIT:
            
                struct _EXIT e = head->data._EXIT;
                gen_exit (stream, e.expr, map);
                break;
            case _LET:
                struct _LET l = head->data._LET;
                insert (map, l.name, eval_expr (l.expr, map));
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

#define HSIZE_FACTOR 137

hashmap *create_hashmap (size_t size) {
    hashmap *map = malloc (sizeof (hashmap));
    map->size = size;
    map->vars = malloc (size * sizeof (variable *));

    for (size_t i = 0; i < size; i++) 
        map->vars[i] = NULL;

    return map;
}

size_t hash_djb2 (const char *str, size_t size) {
    size_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % size;  // to give us an index in the database
}

void insert (hashmap *map, char *key, int value) {
    size_t i = hash_djb2 (key, map->size);
    variable *v = malloc (sizeof (variable));
    v->name = key;
    v->value = value;
    v->next = map->vars[i];
    map->vars[i] = v;
    printf ("Successfully registered %s as %d, at index %zu. \n", key, value, i);
}

int get_value (hashmap *map, char *key) {
    size_t i = hash_djb2 (key, map->size);
    printf ("i == %zu \n", i);
    variable *v = map->vars[i];
    while (v != NULL) {
        if (strcmp (key, v->name) == 0)
            return v->value;
        v = v->next;
    }

    /* If we come here, we didnt find the variable in the hashmap, 
    so we throw error. We do not want to return something like -1 since a variable 
    should be able to be all ints (atm). */
    printf ("Tried accessing an undefined variable. \n");
    exit(EXIT_FAILURE);
}




