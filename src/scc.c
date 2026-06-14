#include "scc.h"
#include "tests/test.h"

static const char *token_names[] = {
  "TOK_AUTO",
  "TOK_BREAK",
  "TOK_CASE",
  "TOK_CHAR",
  "TOK_CONST",
  "TOK_CONTINUE",
  "TOK_DEFAULT",
  "TOK_DO",
  "TOK_DOUBLE",
  "TOK_ELSE",
  "TOK_ENUM",
  "TOK_EXTERN",
  "TOK_FLOAT",
  "TOK_FOR",
  "TOK_GOTO",
  "TOK_IF",
  "TOK_INLINE",
  "TOK_INT",
  "TOK_LONG",
  "TOK_REGISTER",
  "TOK_RESTRICT",
  "TOK_RETURN",
  "TOK_SHORT",
  "TOK_SIGNED",
  "TOK_SIZEOF",
  "TOK_STATIC",
  "TOK_STRUCT",
  "TOK_SWITCH",
  "TOK_TYPEDEF",
  "TOK_UNION",
  "TOK_UNSIGNED",
  "TOK_VOID",
  "TOK_VOLATILE",
  "TOK_WHILE",
  "TOK__ALIGNAS",
  "TOK__ALIGNOF",
  "TOK__ATOMIC",
  "TOK__BOOL",
  "TOK__COMPLEX",
  "TOK__GENERIC",
  "TOK__IMAGINARY",
  "TOK__NORETURN",
  "TOK__STATIC_ASSERT",
  "TOK__THREAD_LOCAL",
  "TOK_IDENTIFIER",
  "TOK_OPEN_PARENTHESIS",
  "TOK_CLOSE_PARENTHESIS",
  "TOK_OPEN_BRACE",
  "TOK_CLOSE_BRACE",
  "TOK_OPEN_BRACKET",
  "TOK_CLOSE_BRACKET",
  "TOK_SEMICOLON",
  "TOK_NEWLINE",
  "TOK_PLUS",
  "TOK_MINUS",
  "TOK_ASTR",
  "TOK_SLASH",
  "TOK_EQ",
  "TOK_NUMBER",
  "TOK_CCHAR",
  "TOK_CINT",
  "TOK_CUINT",
  "TOK_CLONG",
  "TOK_CULONG",
  "TOK_STR",
  "TOK_UNKNOWN"
};

static void test(struct token_list_t *token_list)
{
  printf("--- tokens ---\n");
  for (int i = 0; i < token_list->count; i++) {
    if (token_list->tokens[i].type == TOK_NEWLINE) { 
      continue;
    }
    token_list->tokens[i].type == TOK_NUMBER 
      ? printf("'%s' %ld: %s\n", 
          token_list->tokens[i].token, 
          token_list->tokens[i].long_val,
          token_names[token_list->tokens[i].type])
      : printf("'%s': %s\n", 
          token_list->tokens[i].token, 
          token_names[token_list->tokens[i].type]);
  }
}

static void free_token_list(struct token_list_t *token_list) {
  if (token_list == NULL) return;

  for (int i = 0; i < token_list->count; i++) {
    free(token_list->tokens[i].token);
    if (token_list->tokens[i].type == TOK_STR) {
      free(token_list->tokens[i].str_val);
    }
  }

  free(token_list->tokens);
  free(token_list);
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "usage: scc <target>\n");
    return EXIT_FAILURE;
  }

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

  if (sf_buf != NULL) {
    free(sf_buf);
  }

  /* for testing */
  test(token_list);

  free_token_list(token_list);
  fclose(sf_ptr);

  return EXIT_SUCCESS;
}

