#pragma once

typedef enum {
	RETURN,
	INT_LITERAL,
	SEMICOLON
}	token_t;
	
typedef struct {
	token_t type;	/* Token type. */
	char *value;
	void *next;		/* Pointer to the next token. */
}	token;

/* ASS is funnier than ASM but mean the same thing. */
#define MAX_ASS_SIZE 64000
#define TMP_ASS_NAME "tmp.asm"
#define TMP_OBJ_NAME "tmp.o"

void tokens2asm (token **tokenhead);

token *add_token (token_t type, char *value, token **tokenhead);

void print_tokens (token **tokenhead);

void reverse (token **head);

void tokenize (char contents[], token **tokenhead);
