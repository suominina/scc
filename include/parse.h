#include <stdio.h>

/* list of BNF symbols */
enum ast_type_t {
};

/* represents a single node of AST */
struct ast_node_t {
};

typedef struct ast_t AST;
struct ast_t {
  enum ast_type_t type;
  union {
    /*  */
  };
};
