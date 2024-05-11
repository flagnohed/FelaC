#pragma once
#include "parse.h"


/* ASS is funnier than ASM but means the same thing. */
#define MAX_ASS_SIZE 64000
#define TMP_ASS_NAME "build/tmp.asm"
#define TMP_OBJ_NAME "build/tmp.o"

void generate_exit (node_exit *ne, char asscode[]);