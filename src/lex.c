#include "scc.h"

#define KEYWORDS_LIST_LEN (int)(sizeof(Keywords) / sizeof(Keywords[0]))
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
  struct token_t *tmp = 
    realloc(token_list->tokens, token_list->capacity * sizeof(struct token_t));
  if (tmp == NULL) {
    fprintf(stderr, "error: realloc failed\n");
    exit(EXIT_FAILURE);
  } else {
    token_list->tokens = tmp;
  }
}

static void set_token(struct token_list_t *token_list, const char *sf_buf, int token_len, enum token_type_t token_type)
{
  struct token_t token  = {0};

  token.token = check_ptr(strndup(sf_buf, token_len));
  token.token_len = token_len;
  token.type = token_type;

  switch (token_type) {
  /* token.str_val is needed to store string literal(TOK_STR).
  case TOK_IDENTIFIER:
    token.str_val = check_ptr(strndup(sf_buf, token_len));
    break;
  */
  case TOK_NUMBER:
    token.long_val = atol(token.token);
    break;
  default:
    break;
  }

  token_list->tokens[token_list->count] = token;
  token_list->count++;
}

struct token_list_t *tokenize(const char *sf_buf)
{
  struct token_list_t *token_list = 
    check_ptr(malloc(sizeof(struct token_list_t)));
  token_list->count = 0;
  token_list->capacity = 0;

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
        if (token_len == (int)strlen(Keywords[j]) 
            && memcmp(&sf_buf[i], Keywords[j], strlen(Keywords[j])) == 0) {
            token_type = j;
            break;
        }
      }
      set_token(token_list, &sf_buf[i], token_len, token_type);

      i += token_len-1;
    } else if (isdigit(sf_buf[i])) {
      /* should be a constant (number) */
      while (isdigit(sf_buf[i+token_len])) {
        token_len++;
      }
      set_token(token_list, &sf_buf[i], token_len, TOK_NUMBER);
      i += token_len-1;

    } else if (sf_buf[i] == '(') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_OPEN_PARENTHESIS);
    } else if (sf_buf[i] == ')') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_CLOSE_PARENTHESIS);
    } else if (sf_buf[i] == '{') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_OPEN_BRACE);
    } else if (sf_buf[i] == '}') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_CLOSE_BRACE);
    } else if (sf_buf[i] == ';') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_SEMICOLON);
    } else if (sf_buf[i] == '[') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_OPEN_BRACKET);
    } else if (sf_buf[i] == ']') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_CLOSE_BRACKET);
    } else if (sf_buf[i] == '+') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_PLUS);
    } else if (sf_buf[i] == '-') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_MINUS);
    } else if (sf_buf[i] == '*') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_ASTR);
    } else if (sf_buf[i] == '/') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_SLASH);
    } else if (sf_buf[i] == '\n') {
      set_token(token_list, &sf_buf[i], token_len+1, TOK_NEWLINE);
    } else {
      /* should raise an error here? 
       * or check token type somewhere else */
      set_token(token_list, &sf_buf[i], token_len+1, TOK_UNKNOWN);
    }
  }

  return token_list;
}


/* TODO: implement free_token_list()
 *
 *
 * */
