#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define INITIAL_CAPACITY 256

/* token_type and Keywords must be in the same order */
enum token_type_t {
  AUTO, BREAK, CASE, CHAR, CONST,
  CONTINUE, DEFAULT, DO, DOUBLE, ELSE,
  ENUM, EXTERN, FLOAT, FOR, GOTO,
  IF, INLINE, INT, LONG, REGISTER,
  RESTRICT, RETURN, SHORT, SIGNED, SIZEOF, 
  STATIC, STRUCT, SWITCH, TYPEDEF, UNION, 
  UNSIGNED, VOID, VOLATILE, WHILE, _ALIGNAS, 
  _ALIGNOF, _ATOMIC, _BOOL, COMPLEX, GENERIC, 
  _IMAGINARY, _NORETURN, _STATIC_ASSERT, THREAD_LOCAL,
  IDENTIFIER, CONSTANT, OPEN_PARENTHESIS, CLOSE_PARENTHESIS, OPEN_BRACE,
  CLOSE_BRACE, SEMICOLON, NEWLINE, UNKNOWN
};

#define Keywords_LIST_LEN 44
const char *Keywords[] = {
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

void *check_ptr(void *ptr)
{
  if (ptr == NULL) {
    fprintf(stderr, "error: malloc failed\n");
    exit(EXIT_FAILURE);
  } else {
    return ptr;
  }
}

void realloc_list(struct token_list_t *token_list)
{
  if (token_list->capacity == 0) {
    token_list->capacity = INITIAL_CAPACITY;
  } else {
    token_list->capacity *= 2;
  }
  token_list->tokens = 
    realloc(token_list->tokens, token_list->capacity * sizeof(struct token_t));
}

void set_token(struct token_list_t *token_list, struct token_t *token, const char *sf_buf, int token_len, enum token_type_t token_type)
{
  token->token = strndup(sf_buf, token_len);
  token->token[token_len] = '\0';
  token->token_len = token_len;
  token->type = token_type;

  switch (token_type) {
  case IDENTIFIER:
    token->str_val = strndup(sf_buf, token_len);
    token->str_val[token_len] = '\0';
    break;
  case SHORT:
  case INT:
  case LONG:
    token->long_val = atol(token->token);
    break;
  default:
    break;
  }

  token_list->tokens[token_list->count] = *token;
  token_list->count++;
}

struct token_list_t *tokenize(const char *sf_buf)
{
  struct token_list_t *token_list = 
    check_ptr(malloc(sizeof(struct token_list_t)));
  token_list->count = 0;
  token_list->capacity = 0;
  struct token_t *token = 
    check_ptr(malloc(sizeof(struct token_t)));
  token->token_len = 0;

  /* tokenization process */
  for (int i = 0; sf_buf[i] != '\0'; i++) {
    if (token_list->count >= token_list->capacity) {
      realloc_list(token_list);
    }

    int token_len = 0;
    if (isblank(sf_buf[i])) {
      continue;
    } else if (isalpha(sf_buf[i])) {
      /* should be an identifier or a keyword */
      while (isalnum(sf_buf[i+token_len]) || sf_buf[i+token_len] == '_') {
        token_len++;
      }

      /* check if token is one of the keywords */
      enum token_type_t token_type = IDENTIFIER;
      for (int j = 0; j < Keywords_LIST_LEN; j++) {
        if (strlen(Keywords[j]) >= token_len) {
          if (memcmp(&sf_buf[i], Keywords[j], token_len) == 0) {
            token_type = j;
          } 
        } else if (strlen(Keywords[j]) < token_len) {
          if (memcmp(&sf_buf[i], Keywords[j], strlen(Keywords[j])) == 0) {
            token_type = j; 
          }
        } 
      }
      set_token(token_list, token, &sf_buf[i], token_len, token_type);

      i += token_len-1;
    } else if (isdigit(sf_buf[i])) {
      /* should be a constant */
      while (isdigit(sf_buf[i+token_len])) {
        token_len++;
      }
      set_token(token_list, token, &sf_buf[i], token_len, CONSTANT);
      i += token_len-1;

    /* TODO: should use switch statament insted */
    } else if (sf_buf[i] == '(') {
      set_token(token_list, token, &sf_buf[i], token_len+1, OPEN_PARENTHESIS);
    } else if (sf_buf[i] == ')') {
      set_token(token_list, token, &sf_buf[i], token_len+1, CLOSE_PARENTHESIS);
    } else if (sf_buf[i] == '{') {
      set_token(token_list, token, &sf_buf[i], token_len+1, OPEN_BRACE);
    } else if (sf_buf[i] == '}') {
      set_token(token_list, token, &sf_buf[i], token_len+1, CLOSE_BRACE);
    } else if (sf_buf[i] == ';') {
      set_token(token_list, token, &sf_buf[i], token_len+1, SEMICOLON);
    } else if (sf_buf[i] == '\n') {
      set_token(token_list, token, &sf_buf[i], token_len+1, NEWLINE);
    } else {
      set_token(token_list, token, &sf_buf[i], token_len+1, UNKNOWN);
    }
  }

  return token_list;
}


int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "usage: scc <target>\n");
    return EXIT_FAILURE;
  }

  /* sf stands for source file */
  FILE *sf_ptr = fopen(argv[1], "r");
  if (sf_ptr == NULL) {
    fprintf(stderr, "error: failed to open source file\n");
    return EXIT_FAILURE;
  }

  /* get file size */
  fseek(sf_ptr, 0, SEEK_END);
  size_t sf_bytes = ftell(sf_ptr);
  rewind(sf_ptr);

  char *sf_buf = malloc(sizeof(char) * sf_bytes+1);
  if (sf_buf == NULL) {
    fprintf(stderr, "error: failed to allocate memory\n");
    return EXIT_FAILURE;
  }

  /* read source file */
  size_t bytes_read = 0;
  while ((bytes_read = fread(sf_buf, 1, sf_bytes+1, sf_ptr)) > 0) {
    if (feof(sf_ptr)) {
      sf_buf[sf_bytes] = '\0';
      printf("read %ld bytes\n", bytes_read);
    } else if (ferror(sf_ptr)) {
      fprintf(stderr, "error: failed to read source file\n");
      return EXIT_FAILURE;
    }
  }

  struct token_list_t *token_list = tokenize(sf_buf);
  if (token_list->count == 0) {
    fprintf(stderr, "error: no token was found\n");
  }

  /* won't use sf_buf anymore */
  if (sf_buf != NULL) {
    free(sf_buf);
  }

  /* for testing */
  printf(" --- tokens ---\n");
  for (int i = 0; i < token_list->count; i++) {
    /* use switch statement to show token type */
    if (token_list->tokens[i].type == NEWLINE) {
      continue;
    } else {
      printf("'%s': %d\n", token_list->tokens[i].token, token_list->tokens[i].type);
    }
  }

  //free_token_list();
  fclose(sf_ptr);

  return EXIT_SUCCESS;
}


/* TODO: implement free_token_list()
 *
 *
 * */
