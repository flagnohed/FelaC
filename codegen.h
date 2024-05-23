#pragma once
#include "parse.h"
#include "stdio.h"

/* ASS is funnier than ASM but means the same thing. */
#define MAX_ASS_SIZE 64000
#define TMP_ASS_NAME "build/tmp.asm"
#define TMP_OBJ_NAME "build/tmp.o"


typedef struct value_struct {
    int data;
    struct value_struct *next;
} value_s;



/* Hashtable used for keeping track of variables in FelaC. */
typedef struct variable {
    char *name;
    int value;
    struct variable *next;
} variable;

typedef struct hashmap {
    size_t size;
    variable **vars;
} hashmap;

/* TODO: make map global so we dont have to send it in parameter. */

hashmap *create_hashmap (size_t size);

size_t hash_djb2 (const char *str, size_t size);

void insert (hashmap *map, char *key, int value);

void generate_code (node_prog *prog);

void gen_exit(FILE *stream, node_expr *expr, hashmap *map);

// void add_ident (FILE *stream, char *name, node_expr *expr, hashmap *map);

int get_value (hashmap *map, char *key);

int eval_expr (node_expr *expr, hashmap *map);

void gen_intlit (FILE *stream, node_expr *expr, hashmap *map);

void push (FILE *stream, const char *reg);

void pop (FILE *stream, const char *reg);