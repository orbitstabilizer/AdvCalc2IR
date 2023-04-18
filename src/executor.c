
#include "executor.h"
#include "debug.h"
#include "dictionary.h"
#include "lexer.h"
#include "parser.h"



// static operand_t str_to_long(char *str);
static operand_t add(SyntaxNode* parent, FILE* output_file);
static operand_t sub(SyntaxNode* parent, FILE* output_file);
static operand_t mul(SyntaxNode* parent, FILE* output_file);
static operand_t sdiv(SyntaxNode* parent, FILE* output_file);
static operand_t mod(SyntaxNode* parent, FILE* output_file);
static operand_t b_and (SyntaxNode* parent, FILE* output_file);
static operand_t b_or (SyntaxNode* parent, FILE* output_file);
static operand_t b_xor (SyntaxNode* parent, FILE* output_file);
static operand_t lshift(SyntaxNode* parent, FILE* output_file);
static operand_t rshift(SyntaxNode* parent, FILE* output_file);
static operand_t lrotate(SyntaxNode* parent, FILE* output_file);
static operand_t rrotate(SyntaxNode* parent, FILE* output_file);
static operand_t b_not(SyntaxNode* parent, FILE* output_file);


#undef DEBUG
#define DEBUG 0

#define RAISE_ERROR                                                            \
  do {                                                                         \
    goto error;                                                                \
  } while (0)

typedef operand_t (*op_t)(SyntaxNode*, FILE*);
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
  case TOKEN_DIV:
    return sdiv;
  case TOKEN_MOD:
    return mod;
  default:
    return NULL;
  }
}

int register_count = 1;
int new_register() { return register_count++; }

int print_util(int new_reg, SyntaxNode* parent, FILE* output_file, char* op) {
    char left[50];
    char right[50];
    char new_reg_str[50];

    snprintf(new_reg_str, 50, "%%reg_%d", new_reg);

    if(parent->left->token->type == TOKEN_LITERAL) {
        snprintf(left, parent->left->token->length, "%s", parent->left->token->start);
    } else if(parent->left->token->type == TOKEN_IDENTIFIER) {
        snprintf(left, parent->left->token->length, "%%%s", parent->left->token->start);
    } else {
        snprintf(left, 50, "%%reg_%d", parent->left->reg);
    }

    if(parent->right->token->type == TOKEN_LITERAL) {
        snprintf(right, parent->right->token->length, "%s", parent->right->token->start);
    } else if(parent->right->token->type == TOKEN_IDENTIFIER) {
        snprintf(right, parent->right->token->length, "%%%s", parent->right->token->start);
    } else {
        snprintf(right, 50, "%%reg_%d", parent->right->reg);
    }

    fprintf(output_file, "%s = %s i32 %s, %s\n", new_reg_str, op, left, right);

    return parent->reg = new_reg;
}

operand_t add(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "add");
}
operand_t sub(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "sub");
}
operand_t mul(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "mul");
}
operand_t sdiv(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "sdiv");
}
operand_t mod(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "srem");
}
operand_t b_and(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "and");
}
operand_t b_or(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "or");
}
operand_t b_xor(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "xor");
}
operand_t lshift(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "lshift");
}
operand_t rshift(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "rshift");
}
operand_t lrotate(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "lrotate");
}
operand_t rrotate(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "rrorate");
}
operand_t b_not(SyntaxNode* parent, FILE* output_file) {
    return print_util(new_register(), parent, output_file, "not_is_not_supported");
}
operand_t paran(SyntaxNode* parent, FILE* output_file) {
  //return reg_a;
}

/* operand_t str_to_long(char *str) {
  if (str == NULL) {
    return 0;
  }
  return atol(str);
} */

operand_t eval_util(SyntaxNode *parent, Dictionary *dict, bool *error, FILE* output_file) {
  if (*error || parent == NULL || parent->type == ERROR)
    RAISE_ERROR;
  operand_t left = 0;
  operand_t right = 0;
  if (parent->type == BINOP || parent->type == UNOP || parent->type == PAREN)
    left = eval_util(parent->left, dict, error, output_file);
  if (parent->type == BINOP)
    right = eval_util(parent->right, dict, error, output_file);
  if (parent->type == TOKEN) {
    if (parent->token->type == TOKEN_LITERAL) {
      return parent->token->value;
    } else if (parent->token->type == TOKEN_IDENTIFIER) {
      size_t len = parent->token->length;
      char *var = (char *)malloc(sizeof(char) * (len + 1));
      strncpy(var, parent->token->start, len);
      var[len] = '\0';
      int is_declared = 0;
      operand_t val = get_var(dict, var, &is_declared);
      if (!is_declared) {
          *error = true;
      }
      free(var);
      // TODO: register the variable
      return val;
    }
  }
  if (parent->type == BINOP || parent->type == UNOP) {
    op_t op = token_to_op(parent->mid->token->type);
    if (op != NULL) {
      return op(parent, output_file);
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


int exec(Dictionary *dict, char *input, FILE* output_file) {
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
  operand_t res = eval_util(tree->root, dict, &eval_err, output_file);
  if (eval_err)
    goto execution_error;
  else if (is_assignment)
    set_var(dict, variable, res);
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
