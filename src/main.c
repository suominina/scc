#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define INITIAL_CAPACITY 256
#define SF_BUFSIZE 4096

enum token_type_t {
  RETURN,
  INT,
  VOID,
  SEMICOLON,
  TOKEN_END
};

struct token_t {
  enum token_type_t type;
  char *value;
};

struct token_list_t {
  struct token_t *tokens;
  int count;
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

struct token_t tokenize(const char *sf_buf)
{
  struct token_t token = {0};
  token.value = check_ptr(malloc(sizeof(struct token_t) * strlen(sf_buf) + 1));
  /*
  memcpy(token.value, sf_buf, strlen(sf_buf) + 1);
  */
  token.type = TOKEN_END;
  return token;
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

  struct token_list_t token_list = {0}; /* dynamic array */
  while(1) {
    if (token_list.count >= token_list.capacity) {
      if (token_list.capacity == 0) {
        token_list.capacity = INITIAL_CAPACITY;
      } else {
        token_list.capacity *= 2;
      }
      token_list.tokens = 
        realloc(token_list.tokens, token_list.capacity * sizeof(struct token_t));
    }
    token_list.tokens[token_list.count++] = tokenize(sf_buf);
    if (token_list.tokens[token_list.count-1].type == TOKEN_END) {
      break;
    }
  }

  for (int i = 0; i < token_list.count; i++) {
    printf("%s", token_list.tokens[i].value);
  }

  fclose(sf_ptr);

  return EXIT_SUCCESS;
}
