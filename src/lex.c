#include "scc.h"

static void *check_ptr(void *ptr)
{
  if (ptr == NULL) {
    fprintf(stderr, "error: malloc failed\n");
    exit(EXIT_FAILURE);
  } else {
    return ptr;
  }
}

#define INITIAL_CAPACITY 256
static void realloc_list(struct token_list_t *token_list)
{
  if (token_list->capacity == 0) {
    token_list->capacity = INITIAL_CAPACITY;
  } else {
    token_list->capacity *= 2;
  }
  token_list->tokens = 
    realloc(token_list->tokens, token_list->capacity * sizeof(struct token_t));
}

static void set_token(struct token_list_t *token_list, struct token_t *token, const char *sf_buf, int token_len, enum token_type_t token_type)
{
  token->token = strndup(sf_buf, token_len);
  token->token[token_len] = '\0';
  token->token_len = token_len;
  token->type = token_type;

  switch (token_type) {
  case TOK_IDENTIFIER:
    token->str_val = strndup(sf_buf, token_len);
    token->str_val[token_len] = '\0';
    break;
  case TOK_CONST:
    token->long_val = atol(token->token);
    break;
  case TOK_SHORT:
    token->long_val = (short)atoi(token->token);
    break;
  case TOK_INT:
    token->long_val = atoi(token->token);
    break;
  case TOK_LONG:
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
      enum token_type_t token_type = TOK_IDENTIFIER;
      for (int j = 0; j < KEYWORDS_LIST_LEN; j++) {
        if (memcmp(&sf_buf[i], Keywords[j], strlen(Keywords[j])) == 0) {
            token_type = j;
        }
      }
      set_token(token_list, token, &sf_buf[i], token_len, token_type);

      i += token_len-1;
    } else if (isdigit(sf_buf[i])) {
      /* should be a constant (number) */
      while (isdigit(sf_buf[i+token_len])) {
        token_len++;
      }
      set_token(token_list, token, &sf_buf[i], token_len, TOK_CONST);
      i += token_len-1;

    } else if (sf_buf[i] == '(') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_OPEN_PARENTHESIS);
    } else if (sf_buf[i] == ')') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_CLOSE_PARENTHESIS);
    } else if (sf_buf[i] == '{') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_OPEN_BRACE);
    } else if (sf_buf[i] == '}') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_CLOSE_BRACE);
    } else if (sf_buf[i] == ';') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_SEMICOLON);
    } else if (sf_buf[i] == '[') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_OPEN_BRACKET);
    } else if (sf_buf[i] == ']') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_CLOSE_BRACKET);
    } else if (sf_buf[i] == '\n') {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_NEWLINE);
    } else {
      set_token(token_list, token, &sf_buf[i], token_len+1, TOK_UNKNOWN);
    }
  }

  return token_list;
}


/* TODO: implement free_token_list()
 *
 *
 * */
