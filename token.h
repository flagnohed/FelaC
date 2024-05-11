#pragma once

typedef enum {
	EXIT,
	INT_LITERAL,
	SEMICOLON,
	OPEN_PAREN,
	CLOSE_PAREN
}	token_t;
	
typedef struct {
	token_t type;	/* Token type. */
	char *value;
	void *next;		/* Pointer to the next token. */
}	token;



void tokens2asm (token **tokenhead);

token *add_token (token_t type, char *value, token **tokenhead);

void print_tokens (token **tokenhead);

void reverse (token **head);

void tokenize (char contents[], token **tokenhead);
