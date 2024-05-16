#pragma once
#include "parse.h"
#include "stdio.h"

/* ASS is funnier than ASM but means the same thing. */
#define MAX_ASS_SIZE 64000
#define TMP_ASS_NAME "build/tmp.asm"
#define TMP_OBJ_NAME "build/tmp.o"


struct value_struct {
    int data;
    struct value_struct *next;
};

typedef struct value_struct value_s;

/* Hashtable used for keeping track of variables in FelaC. */
typedef struct {
    size_t size;
    void **keys;
    //value_type *values;
    value_s **values;
}   htable;


void generate_code (node_prog *prog);

void gen_exit(FILE *stream, node_expr *expr, htable *ht);

void add_ident (FILE *stream, char *name, node_expr *expr, htable *ht);

int eval_expr (node_expr *expr, htable *ht);

void gen_intlit (FILE *stream, node_expr *expr, htable *ht);

htable *hash_new (size_t size);

void hash_del (htable *ht);

size_t hash_index (htable *ht, char *key);

void hash_insert (htable *ht, void *key, value_s *value);

value_s *hash_lookup (htable *ht, char *key);


void push (FILE *stream, const char *reg);

void pop (FILE *stream, const char *reg);