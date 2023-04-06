#include "lexer.h"
#include "debug.h"/*
 * Parse a long from a string
 * str: string to parse
 * val: pointer to long to store value
 */
static bool parseLong(const char *str, long *val);
static const char *str_token_type[] = {
  "TOKEN_UNKNOWN", 
  "TOKEN_LITERAL",
  "TOKEN_IDENTIFIER",
  "TOKEN_STAR",
  "TOKEN_PLUS",
  "TOKEN_MINUS", 
  "TOKEN_AND", 
  "TOKEN_OR",  
  "TOKEN_NOT", 
  "TOKEN_XOR", 
  "TOKEN_LS", 
  "TOKEN_RS", 
  "TOKEN_LR", 
  "TOKEN_RR", 
  "TOKEN_EQ",      
  "TOKEN_COMMENT", 
  "TOKEN_EOF",     
  "TOKEN_LEFT_PAREN",  
  "TOKEN_RIGHT_PAREN", 
  "TOKEN_COMMA"
};

bool parseLong(const char *str, long *val){
    while(*str == '0' && *(str+1) != '\0')
        str++;
    char *temp;
    bool rc = true;
    errno = 0;
    *val = strtol(str, &temp, 0);

    if (temp == str || *temp != '\0' ||
        ((*val == LONG_MIN || *val == LONG_MAX) && errno == ERANGE))
        rc = false;

    return rc;
}

Lexer *lexer_new(char *input, size_t input_len){
    Lexer *lexer =  (Lexer*) malloc(sizeof(Lexer));
    lexer->input = input;
    lexer->cur_pos = 0;
    lexer->input_len = input_len;
    lexer->cur_token = 0;
    return lexer;

}

void lexer_free(Lexer *lexer){
    free(lexer);
}



void print_lex(Lexer *lexer){
    printf("Lexer{\n\tinput: `%s`\n\tcur_pos: %zu\n\tinput_len: %zu\n\tcur_token: %zu\n",
           lexer->input, lexer->cur_pos, lexer->input_len, lexer->cur_token);
    printf("\tToken List:{\n");
    for (size_t i = 0; i < lexer->cur_token; i++){
        Token *token = &lexer->token_list[i];
        char sub_str[token->length + 1];
        strncpy(sub_str, token->start, token->length);
        sub_str[token->length] = '\0';

        printf("\t\tToken{\n\t\t\ttype: %s\n\t\t\tvalue: %ld\n\t\t\tlength: %zu\n\t\t\tstart: '%s'\n\t\t},\n",
               str_token_type[token->type], token->value, token->length, sub_str);
    }
    printf("\t}\n");
    printf("}\n");
}

void init_token(Token *token, TokenType type, long value, size_t value_len, char* start ){
    token->type = type;
    token->value = value;
    token->length = value_len;
    token->start = start;
    if (type == TOKEN_LITERAL){
        char sub_str[value_len + 1];
        strncpy(sub_str, start, value_len);
        sub_str[value_len] = '\0';
        long val;
        if (parseLong(sub_str, &val)){
            token->value = val;
        }else{
            token->type = TOKEN_UNKNOWN;
        }
    }
}



void lexer_next(Lexer *lexer){
    Token* token = &lexer->token_list[lexer->cur_token];
    if (lexer->cur_pos >= lexer->input_len){
        init_token(token, TOKEN_EOF, 0, 0,  NULL);
        lexer->cur_token++;
        return;
    }
    char c = lexer->input[lexer->cur_pos];
    if (c == ' ' || c == '\t' || c == '\n'){
        while (c == ' ' || c == '\t' || c == '\n'){
            lexer->cur_pos++;
            c = lexer->input[lexer->cur_pos];
        }
    }
    if (c == '\0' || c == EOF){
        init_token(token, TOKEN_EOF, 0, 0,  NULL);
        lexer->cur_token++;
        return;
    }
    bool MATCHED = true;
    switch (c){
        case '+':
            init_token(token, TOKEN_PLUS, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case '-':
            init_token(token, TOKEN_MINUS, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case '*':
            init_token(token, TOKEN_STAR, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case '(':
            init_token(token, TOKEN_LEFT_PAREN, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case ')':
            init_token(token, TOKEN_RIGHT_PAREN, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case '=':
            init_token(token, TOKEN_EQ, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case '&':
            init_token(token, TOKEN_AND, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case '|':
            init_token(token, TOKEN_OR, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        case ',':
            init_token(token, TOKEN_COMMA, 0, 1, &lexer->input[lexer->cur_pos]);
            break;
        default:
            MATCHED = false;
    }
    if (MATCHED){
        lexer->cur_pos++;
        lexer->cur_token++;
        return;
    }

    if (c == '%'){
        init_token(token, TOKEN_EOF, 0, lexer->input_len - lexer->cur_pos, &lexer->input[lexer->cur_pos]);
        lexer->cur_pos = lexer->input_len;
        lexer->cur_token++;
        return;

    }
    //  xor, ls, rs, not, lr, rr
    if (c == 'l' && lexer->cur_pos <= lexer->input_len - 2 && !isalpha( lexer->input[lexer->cur_pos +2] )){
        if (lexer->input[lexer->cur_pos + 1] == 's'){
            init_token(token, TOKEN_LS, 0, 2, &lexer->input[lexer->cur_pos]);
            lexer->cur_pos += 2;
            lexer->cur_token++;
            return;
        }else if (lexer->input[lexer->cur_pos + 1] == 'r'){
            init_token(token, TOKEN_LR, 0, 2, &lexer->input[lexer->cur_pos]);
            lexer->cur_pos += 2;
            lexer->cur_token++;
            return;
        }
    }
    else if (c== 'r' && lexer->cur_pos <= lexer->input_len - 2 && !isalpha( lexer->input[lexer->cur_pos +2] )){
        if (lexer->input[lexer->cur_pos + 1] == 's'){
            init_token(token, TOKEN_RS, 0, 2, &lexer->input[lexer->cur_pos]);
            lexer->cur_pos += 2;
            lexer->cur_token++;
            return;
        }else if (lexer->input[lexer->cur_pos + 1] == 'r'){
            init_token(token, TOKEN_RR, 0, 2, &lexer->input[lexer->cur_pos]);
            lexer->cur_pos += 2;
            lexer->cur_token++;
            return;
        }
    }
    else if (c == 'x' && lexer->cur_pos <= lexer->input_len - 3 && !isalpha( lexer->input[lexer->cur_pos +3] )){
        if (lexer->input[lexer->cur_pos + 1] == 'o' && lexer->input[lexer->cur_pos + 2] == 'r'){
            init_token(token, TOKEN_XOR, 0, 3, &lexer->input[lexer->cur_pos]);
            lexer->cur_pos += 3;
            lexer->cur_token++;
            return;
        }
    }
    else if (c == 'n' && lexer->cur_pos <= lexer->input_len - 3 && !isalpha( lexer->input[lexer->cur_pos +3] )){
        if (lexer->input[lexer->cur_pos + 1] == 'o' && lexer->input[lexer->cur_pos + 2] == 't'){
            init_token(token, TOKEN_NOT, 0, 3, &lexer->input[lexer->cur_pos]);
            lexer->cur_pos += 3;
            lexer->cur_token++;
            return;
        }
    }



    if (isalpha(c)){
        size_t start = lexer->cur_pos;
        while (isalpha(c)){
            lexer->cur_pos++;
            c = lexer->input[lexer->cur_pos];
        }
        init_token(token, TOKEN_IDENTIFIER, 0, lexer->cur_pos - start, &lexer->input[start]);
        lexer->cur_token++;
        return;
    }else if (isdigit(c)){
        size_t start = lexer->cur_pos;
        while (isdigit(c)){
            lexer->cur_pos++;
            c = lexer->input[lexer->cur_pos];
        }
        init_token(token, TOKEN_LITERAL, 0, lexer->cur_pos - start, &lexer->input[start]);
        lexer->cur_token++;

        return;
    }
    else {
        init_token(token, TOKEN_UNKNOWN, 0, 1, &lexer->input[lexer->cur_pos]);
        lexer->cur_pos++;
        lexer->cur_token++;

    }

}
/*
int main(){
    char *input = "xor(1,2)";//"XOR_var = lr(not(5) + 3*93580280895, 1) + lru % ajefo ";// = (a + b) - cenk*31 + 696789876789876789876787";
    size_t input_len = strlen(input);
    Lexer *lexer = lexer_new(input, input_len);
    do {
        lexer_next(lexer);
    } while (lexer->token_list[lexer->cur_token -1].type != TOKEN_EOF);

    print_lex(lexer);

    lexer_free(lexer);

}
*/
