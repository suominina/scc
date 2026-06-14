#include "scc.h"

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
    if (token_list->tokens[i].type == TOK_NEWLINE) {
      continue;
    } else {
      printf("'%s': %d\n", token_list->tokens[i].token, token_list->tokens[i].type);
    }
  }

  //free_token_list();
  fclose(sf_ptr);

  return EXIT_SUCCESS;
}

