#pragma once
#include "parse.h"
#include "stdio.h"

/* ASS is funnier than ASM but means the same thing. */
#define MAX_ASS_SIZE 64000
#define TMP_ASS_NAME "build/tmp.asm"
#define TMP_OBJ_NAME "build/tmp.o"

void generate_code (node_prog *prog);

void gen_exit (FILE *stream, struct _EXIT *e);

int eval_expr (node_expr *expr);