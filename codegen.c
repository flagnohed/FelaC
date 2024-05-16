#include "codegen.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

static size_t hash_entries = 0;


int eval_expr (node_expr *expr, htable *ht) {
    /* Base case. If its just a number, return it. */
    if (expr->expr_t == _INT_LITERAL) {
        struct _INT_LITERAL intlit = expr->data._INT_LITERAL;
        return intlit.val; 
    }
    /* Get the expression of variable, evaluate it. */
    else if (expr->expr_t == _IDENT) {
        struct _IDENT id = expr->data._IDENT;
        value_s *val = hash_lookup (ht, id.name); 
        return val->data;
    }
    else {
        printf ("Could not evaluate expression. \n");
        exit (EXIT_FAILURE);
    }
}


void gen_exit(FILE *stream, node_expr *expr, htable *ht) {
    /* 60 is the syscall number for exit. */
    fprintf (stream, "\tmov rax, 60\n"); fflush (stream);
    /* Put the evaluated expression as an int into rdi (error code). */
    fprintf (stream, "\tmov rdi, %d\n", eval_expr (expr, ht)); fflush (stream);
    fprintf (stream, "\tsyscall\n"); fflush (stream);
}

/* If variable does not exist yet, add it to HT. We dont need to
    create assembly code for this, as we keep track of variables in
    the hash table. */
void add_ident (FILE *stream, char *name, node_expr *expr, htable *ht) {
    /* Make sure that we havent seen this variable before. */
    if (hash_lookup (ht, name) != NULL) {
        printf ("Identifier %s has already been declared. \n", name);
        exit (EXIT_FAILURE);
    }
    value_s *val = calloc (1, sizeof (value_s));
    val->data = eval_expr (expr, ht);
    hash_insert (ht, name, val);
}

void gen_intlit (FILE *stream, node_expr *expr, htable *ht) {
    fprintf (stream, "\tmov rax, %d\n", eval_expr (expr, ht));
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

    htable *ht = hash_new (hsz);
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
                printf ("exit\n");
                gen_exit (stream, e.expr, ht);
                break;
            case _LET:
                struct _LET l = head->data._LET;
                // struct _IDENT i = l.ident->data._IDENT;  
                add_ident (stream, l.name, l.expr, ht);
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

htable *hash_new (size_t size) {

    /* The hash table should be an array with length about 1.3 times 
        the maximum number of keys that will actually be in the table, 
        and. Size of hash table array should be a prime number. */
    size_t hsize = HSIZE_FACTOR * size;
    htable *ht = calloc (1, sizeof (htable));
    ht->keys = calloc (hsize, sizeof (char *));
    ht->values = calloc (hsize, sizeof (value_s));
    ht->size = hsize;
    return ht;
}


void hash_del (htable *ht) {
    free (ht->keys);
    ht->keys = NULL;
    free (ht->values);
    ht->values = NULL;
}


size_t hash_index(htable *ht, char *key) {
    size_t i = (size_t) key % ht->size;
    printf ("i = %zu\n", i);
    /* If we find someone at "our" spot, and its not us, 
        try the next spot until we find a vacant spot. */
    while (ht->keys[i] && ht->keys[i] != key)
        i = (i + 1) % ht->size;

    return i;
}


void hash_insert (htable *ht, void *key, value_s *value) {
    size_t i = hash_index (ht, key);

    if (ht->values[i] != NULL) {
        printf ("Encountered a value at index %zu (should be NULL). \n", i);
        exit (EXIT_FAILURE);
    }

    ht->keys[i] = key;
    ht->values[i] = value;
    hash_entries++;

    printf("[DEBUG]: added ident %s with value %d. \n", ht->keys[i], *((int *)ht->values[i]));
}

/* Returns a pointer to the corresponding VALUE from KEY if it exists, 
    else NULL. */
value_s *hash_lookup (htable *ht, char *key) {

    if (hash_entries == 0) return NULL;

    size_t i = hash_index (ht, key);
    
    return ht->values[i];
}


