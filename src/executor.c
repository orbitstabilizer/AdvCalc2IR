
#include "executor.h"
#include "debug.h"
#include "dictionary.h"
#include "lexer.h"
#include "parser.h"



// static long str_to_long(char *str);
static long add(long a, long b);
static long sub(long a, long b);
static long mul(long a, long b);
static long b_and (long a, long b);
static long b_or (long a, long b);
static long b_xor (long a, long b);
static long lshift(long a, long b);
static long rshift(long a, long b);
static long lrotate(long a, long b);
static long rrotate(long a, long b);
static long b_not(long a, long null);


#undef DEBUG
#define DEBUG 0

#define RAISE_ERROR                                                            \
  do {                                                                         \
    goto error;                                                                \
  } while (0)

typedef long (*op_t)(long, long);
op_t token_to_op(TokenType type) {
  switch (type) {
  case TOKEN_STAR:
    return mul;
  case TOKEN_PLUS:
    return add;
  case TOKEN_MINUS:
    return sub;
  case TOKEN_AND:
    return b_and;
  case TOKEN_OR:
    return b_or;
  case TOKEN_XOR:
    return b_xor;
  case TOKEN_LS:
    return lshift;
  case TOKEN_RS:
    return rshift;
  case TOKEN_LR:
    return lrotate;
  case TOKEN_RR:
    return rrotate;
  case TOKEN_NOT:
    return b_not;
  default:
    return NULL;
  }
}

long add(long a, long b) { return a + b; }
long sub(long a, long b) { return a - b; }
long mul(long a, long b) { return a * b; }
long b_and(long a, long b) { return a & b; }
long b_or(long a, long b) { return a | b; }
long b_xor(long a, long b) { return a ^ b; }
long lshift(long a, long b) { return a << b; }
long rshift(long a, long b) { return a >> b; }
long lrotate(long a, long b) {
  return (a << b) | (a >> (sizeof(long) * 8 - b));
}
long rrotate(long a, long b) {
  return (a >> b) | (a << (sizeof(long) * 8 - b));
}
long b_not(long a, long null) {
  // to resolve unused parameter warning
  null = 0;
  a = a + null;
  return ~a;
}
long paran(long a, long null) {
  // to resolve unused parameter warning
  null = 0;
  a = a + null;
  return a;
}

/* long str_to_long(char *str) {
  if (str == NULL) {
    return 0;
  }
  return atol(str);
} */

long eval_util(SyntaxNode *parent, Dictionary *dict, bool *error) {
  if (*error || parent == NULL || parent->type == ERROR)
    RAISE_ERROR;
  long left = 0;
  long right = 0;
  if (parent->type == BINOP || parent->type == UNOP || parent->type == PAREN)
    left = eval_util(parent->left, dict, error);
  if (parent->type == BINOP)
    right = eval_util(parent->right, dict, error);
  if (parent->type == TOKEN) {
    if (parent->token->type == TOKEN_LITERAL) {
      return parent->token->value;
    } else if (parent->token->type == TOKEN_IDENTIFIER) {
      size_t len = parent->token->length;
      char *var = (char *)malloc(sizeof(char) * (len + 1));
      strncpy(var, parent->token->start, len);
      var[len] = '\0';
      long val = get_var(dict, var);
      free(var);
      return val;
    }
  }
  if (parent->type == BINOP || parent->type == UNOP) {
    op_t op = token_to_op(parent->mid->token->type);
    if (op != NULL) {
      return op(left, right);
    }
  }
  if (parent->type == PAREN) {
    return left;
  }
  return 0;
error:
  *error = true;
  return 0;
}


int exec(Dictionary *dict, char *input, char *output) {
  size_t input_len = strlen(input);
  Lexer *lexer = lexer_new(input, input_len);
  SyntaxTree *tree = NULL;
  TokenType type = TOKEN_EOF;
  bool is_assignment = false;
  char *variable = NULL;
  int state = 0;
  do {
    lexer_next(lexer);
    type = lexer->token_list[lexer->cur_token - 1].type;
    if (type == TOKEN_EOF && lexer->cur_token == 1){
      state = 1;
      goto end;
    }
    if (lexer->cur_token == 2 && type == TOKEN_EQ && lexer->token_list[0].type == TOKEN_IDENTIFIER ) {
      variable = strndup(lexer->token_list[0].start, lexer->token_list[0].length);
      is_assignment = true;
    }    
    if (type == TOKEN_UNKNOWN || (type == TOKEN_EQ && !is_assignment)) {
      goto execution_error; 
    }
  } while (type != TOKEN_EOF);

  if (is_assignment)
    tree = parse(lexer->token_list + 2);
  else
    tree = parse(lexer->token_list);

  bool eval_err = false;
  long res = eval_util(tree->root, dict, &eval_err);
  if (eval_err)
    goto execution_error;
  else if (is_assignment)
    set_var(dict, variable, res);
  snprintf(output, 25, "%ld", res);
  goto end;

execution_error:
  state = 2;
end:
  if (DEBUG && tree){
    print_lex(lexer);
    print_syntax_tree(tree);
  }
  if (tree)
    free_syntax_tree(tree);
  if (lexer)
    lexer_free(lexer);
  if (is_assignment && state != 2){
    state = 3;
  }
  if (variable)
    free(variable);
  return state;
}
