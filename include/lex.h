#ifndef LEX_H
#define LEX_H

/* token_type and Keywords must be in the same order */
enum token_type_t {
  /* --- keywords --- */
  TOK_AUTO, TOK_BREAK, TOK_CASE, TOK_CHAR, TOK_CONST,
  TOK_CONTINUE, TOK_DEFAULT, TOK_DO, TOK_DOUBLE, TOK_ELSE,
  TOK_ENUM, TOK_EXTERN, TOK_FLOAT, TOK_FOR, TOK_GOTO,
  TOK_IF, TOK_INLINE, TOK_INT, TOK_LONG, TOK_REGISTER,
  TOK_RESTRICT, TOK_RETURN, TOK_SHORT, TOK_SIGNED, TOK_SIZEOF, 
  TOK_STATIC, TOK_STRUCT, TOK_SWITCH, TOK_TYPEDEF, TOK_UNION, 
  TOK_UNSIGNED, TOK_VOID, TOK_VOLATILE, TOK_WHILE, TOK_ALIGNAS, 
  TOK_ALIGNOF, TOK_ATOMIC, TOK__BOOL, TOK_COMPLEX, TOK_GENERIC, 
  TOK_IMAGINARY, TOK_NORETURN, TOK_STATIC_ASSERT, TOK_THREAD_LOCAL,
  /* --- keywords --- */

  TOK_IDENTIFIER, 

  TOK_OPEN_PARENTHESIS, TOK_CLOSE_PARENTHESIS, 
  TOK_OPEN_BRACE, TOK_CLOSE_BRACE, 
  TOK_OPEN_BRACKET, TOK_CLOSE_BRACKET, 
  TOK_SEMICOLON, TOK_NEWLINE, 

  /* --- tokens that have value --- */
  TOK_CCHAR, TOK_CINT, TOK_UINT, TOK_CLONG, TOK_CULONG, TOK_STR ,
  /* --- tokens that have value --- */

  TOK_UNKNOWN
};

#define KEYWORDS_LIST_LEN 44
static const char *Keywords[] = {
  "auto", "break", "case", "char", "const",
  "continue", "default", "do", "double", "else",
  "enum", "extern", "float", "for", "goto",
  "if", "inline", "int", "long","register",
  "restrict","return","short","signed","sizeof",
  "static","struct","switch","typedef","union",
  "unsigned","void","volatile","while","_Alignas",
  "_Alignof","_Atomic","_Bool","Complex","Generic",
  "_Imaginary","_Noreturn","_Static_assert","Thread_local"
};

struct token_t {
  enum token_type_t type;
  char *token;
  int token_len;
  union {
    long long_val;
    char *str_val;
  };
};

struct token_list_t {
  struct token_t *tokens;
  int count; /* how many tokens tokens have */
  int capacity; 
};


extern struct token_list_t *tokenize(const char *);


#endif
