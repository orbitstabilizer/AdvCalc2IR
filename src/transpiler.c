
#include "transpiler.h"
#include "debug.h"

Token * translate(SyntaxNode *root, Dictionary *dict, FILE *output_file, bool *error);
int new_register();
int load(Dictionary *dict, char *value, FILE *output_file);
int declare(Dictionary *dict, char *variable, FILE *output_file);
void print_var(FILE *output_file, Token* reg);
Token *print_op(enum SyntaxNodeType type, Token *root, Token *left_reg, Token *right_reg, FILE *output_file, bool *error);
void store(Dictionary *dict, char *name, Token *reg, FILE *output_file);

Token *print_op(enum SyntaxNodeType type, Token *root, Token *left_reg, Token *right_reg, FILE *output_file, bool *error) {
    if (type == UNOP){
        if (*error || !left_reg ){
            *error = true;
            return NULL;
        }
        root->reg = new_register();
        if (left_reg->reg == -1) {
            fprintf(output_file, "  %%reg_%d = call i32 @not(i32 %s)\n", root->reg, left_reg->start);
        } else {
            fprintf(output_file, "  %%reg_%d = call i32 @not(i32 %%reg_%d)\n", root->reg, left_reg->reg);
        }
        return root;

    }
    else if (type == BINOP){
        if (*error || !left_reg || !right_reg){
            *error = true;
            return NULL;
        }
        char op_name[20];
        int is_func = 0;
        switch (root->type) {
            case TOKEN_PLUS:
                strcpy(op_name, "add");
                break;
            case TOKEN_MINUS:
                strcpy(op_name, "sub");
                break;
            case TOKEN_STAR:
                strcpy(op_name, "mul");
                break;
            case TOKEN_DIV:
                strcpy(op_name, "sdiv");
                break;
            case TOKEN_MOD:
                strcpy(op_name, "srem");
                break;
            case TOKEN_AND:
                strcpy(op_name, "and");
                break;
            case TOKEN_OR:
                strcpy(op_name, "or");
                break;
            case TOKEN_XOR:
                strcpy(op_name, "xor");
                break;
            case TOKEN_LS:
                strcpy(op_name, "shl");
                break;
            case TOKEN_RS:
                strcpy(op_name, "lshr");
                break;
            case TOKEN_RR:
                is_func = 1;
                strcpy(op_name, "rr");
                break;
            case TOKEN_LR:
                is_func = 1;
                strcpy(op_name, "lt");
                break;
            default:
                *error = true;
                return NULL;
        }
        if (!is_func){
            if (left_reg->reg == -1 && right_reg->reg == -1) {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = %s i32 %s, %s\n", root->reg, op_name, left_reg->start, right_reg->start);
            } else if (left_reg->reg == -1) {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = %s i32 %s, %%reg_%d\n", root->reg, op_name, left_reg->start, right_reg->reg);
            } else if (right_reg->reg == -1) {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = %s i32 %%reg_%d, %s\n", root->reg, op_name, left_reg->reg, right_reg->start);
            } else {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = %s i32 %%reg_%d, %%reg_%d\n", root->reg, op_name, left_reg->reg, right_reg->reg);
            }
        }
        else{
            if (left_reg->reg == -1 && right_reg->reg == -1) {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = call i32 @%s(i32 %s, i32 %s)\n", root->reg, op_name, left_reg->start, right_reg->start);
            } else if (left_reg->reg == -1) {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = call i32 @%s(i32 %s, i32 %%reg_%d)\n", root->reg, op_name, left_reg->start, right_reg->reg);
            } else if (right_reg->reg == -1) {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = call i32 @%s(i32 %%reg_%d, i32 %s)\n", root->reg, op_name, left_reg->reg, right_reg->start);
            } else {
                root->reg = new_register();
                fprintf(output_file, "  %%reg_%d = call i32 @%s(i32 %%reg_%d, i32 %%reg_%d)\n", root->reg, op_name, left_reg->reg, right_reg->reg);
            }

        }
        return root;

    }
    *error = true;
    return NULL;
}
int load(Dictionary *dict, char *value, FILE *output_file) {
    if (! is_declared(dict, value)){
        return -1;
    }
    int reg = new_register();
    fprintf(output_file, "  %%reg_%d = load i32, i32* %%%s\n", reg, value);
    return reg;
}
int declare(Dictionary *dict, char *variable, FILE *output_file) {
    if (is_declared(dict, variable)){
        return -1;
    }
    fprintf(output_file, "  %%%s = alloca i32\n", variable);
    return 0;
}
void store(Dictionary *dict, char *name, Token *reg, FILE *output_file) {
    declare_var(dict, name);
    if (reg->reg == -1){
        fprintf(output_file, "  store i32 %s, i32* %%%s\n", reg->start, name);
    }
    else{
        fprintf(output_file, "  store i32 %%reg_%d, i32* %%%s\n", reg->reg, name);
    }

}
void print_var(FILE *output_file, Token* reg) {
    if (reg->reg == -1)
        fprintf(output_file, "  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s)\n", reg->start);
    else
        fprintf(output_file, "  call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %%reg_%d)\n", reg->reg);


}

int register_count = 1;
int new_register() { return register_count++; }
#define RAISE_ERROR                                                            \
  do {                                                                         \
    goto error;                                                                \
  } while (0)

Token* translate(SyntaxNode *root, Dictionary *dict, FILE *output_file, bool *error) {
    if (*error || root == NULL || root->type == ERROR)
        RAISE_ERROR;
    if (root->type == TOKEN) {
        if (root->token->type == TOKEN_IDENTIFIER){
            int reg = load(dict, root->token->start, output_file);
            if (reg == -1)
                RAISE_ERROR;
            root->token->reg = reg;

            return root->token;
        }
        if (root->token->type == TOKEN_LITERAL) {
            // No need to create a register for a literal
            return root->token;
        }
    }
    if (root->type == PAREN){
        // Parentheses just evaluate the expression inside
        return translate(root->left, dict, output_file, error);
    }
    if (root->type == UNOP){
        Token* reg = translate(root->left, dict, output_file, error);

        return print_op(UNOP,root->mid->token,  reg, NULL, output_file, error);
    }
    if (root->type == BINOP) {
        Token* left_reg = translate(root->left, dict, output_file, error);
        Token* right_reg = translate(root->right, dict, output_file, error);

        return print_op(BINOP,root->mid->token, left_reg, right_reg, output_file, error);
    }

    error:
    *error = true;
    return NULL;
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
  if (is_assignment)
      declare(dict, variable,output_file);
  Token* res = translate(tree->root, dict, output_file,  &eval_err);
  if (eval_err)
      goto execution_error;
  else if(is_assignment){

      store(dict, variable, res, output_file);
  }


  else{
      print_var(output_file, res);
  }
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





