#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum SyntaxNodeType { BINOP, UNOP, PAREN, TOKEN, ERROR } SyntaxNodeType;

typedef struct SyntaxNode SyntaxNode;

/*
 * SyntaxNode Struct
 * type: type of the node,
 *       BINOP: left child, mid child as binary operator, right child,
 *       UNOP: left child, mid child as unary operator,
 *       PAREN: left child as inside of parenthesis,
 *       TOKEN: token type for leave
 * left: left term of the tree
 * mid: contains operators
 * right: right term of the tree
 * token: leave of the tree, tokens are stored here
 */
struct SyntaxNode {
  SyntaxNodeType type;
  SyntaxNode *left;
  SyntaxNode *mid;
  SyntaxNode *right;
  Token *token;
};

typedef struct SyntaxTree SyntaxTree;

/*
 * <expr> = <term><eof> | <eof>
 * <term> = <term><binop><term> | <primary> | (<term>)
 * <primary> = <func> | <var>
 * <func> = <funcname>(<term>,<term>) | <unop>(<term>)
 * <var> = literal or identifier token
 * <binop> = [*+-&|]
 * <funcname> = (xor|ls|rs|lr|rr)
 * <unop> = not
 */
struct SyntaxTree {
  SyntaxNode *root;
};

/*
 * generates a parse tree
 * calls parse_expr
 * input: Token array
 */
SyntaxTree *parse(Token *tokens);

/*
 * <expr> = <term><eof> | <eof>
 * calls parse_term then expects EOF token
 * input: current token
 */
SyntaxNode *parse_expr(Token **tokens);

/*
 * parses <term>
 * calls and returns parse_comma
 */
SyntaxNode *parse_term(Token **tokens);

/*
 * parses <term>,<term>
 * calls parse_or and assigns it to left child as left <term>
 * then if comma exists
 * assigns TOKEN_COMMA to mid child
 * calls parse_comma and assigns it to right child as right <term>
 */
SyntaxNode *parse_comma(Token **tokens);

/*
 * parses <term>|<term>
 * calls parse_and and assigns it to left child as left <term>
 * then if or exists
 * assigns TOKEN_OR to mid child
 * calls parse_and and assigns it to right child as right <term>
 */
SyntaxNode *parse_or(Token **tokens);

/*
 * parses <term>&<term>
 * calls parse_plus_minus and assigns it to left child as left <term>
 * then if and exists
 * assigns TOKEN_AND to mid child
 * calls parse_plus_minus and assigns it to right child as right <term>
 */
SyntaxNode *parse_and(Token **tokens);

/*
 * parses <term>(+|-)<term>
 * calls parse_mul and assigns it to left child as left <term>
 * then if plus or minus exists
 * assigns TOKEN_PLUS or TOKEN_MINUS to mid child
 * calls parse_mul and assigns it to right child as right <term>
 */
SyntaxNode *parse_plus_minus(Token **tokens);

/*
 * parses <term>*<term>
 * calls parse_primary and assigns it to left child as left <term>
 * then if star exists
 * assigns TOKEN_STAR to mid child
 * calls parse_primary and assigns it to right child as right <term>
 */
SyntaxNode *parse_mul(Token **tokens);

/*
 * parses <primary>
 * calls and return parse_func
 */
SyntaxNode *parse_primary(Token **tokens);

/*
 * parses <funcname>(<term>,<term>)
 * first token must be valid <funcname>
 * then calls parse_paren
 * then if comma exits in parenthesis
 * assigns left <term> to left child
 * assigns related token to mid child
 * assigns right <term> to right child
 */
SyntaxNode *parse_func(Token **tokens);

/*
 * parses <var>
 * expects TOKEN_IDENTIFIER or TOKEN_LITERAL
 */
SyntaxNode *parse_var(Token **tokens);

/*
 * parses (<term>)
 * expects TOKEN_LEFT_PAREN as first token
 * calls parse_term
 * at the end expects TOKEN_RIGHT_PAREN as last token
 */
SyntaxNode *parse_paren(Token **tokens);

/*
 * Free all syntax nodes recursively
 */
void free_syntax_tree(SyntaxTree *tree);

/*
 * prints syntax nodes indented according to depths
 */
void print_syntax_tree(SyntaxTree *tree);

#endif
