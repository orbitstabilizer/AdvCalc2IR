#ifndef TRANSPILER_H
#define TRANSPILER_H

#include "stdio.h"
#include "parser.h"
#include "dictionary.h"

typedef int32_t operand_t;
/*
 * Translates the given expression to LLVM IR and writes it to the given file.
 *
 * Returns following values, depending on the result:
 * 0: expression
 * 1: assignment
 * 2: error
 * 3: empty line
 *
 * dict: dictionary storing the declared variables
 * 
 * The function creates its own lexer and parser, and uses them to parse the input string.
 */
int translate(Dictionary *dict, char *input, FILE* output_file);

/*
 * Returns a new register number to use for temporary storage.
 */
int new_register();

/*
 * Loads the value of the given variable into a register, and writes the corresponding
 * LLVM IR to the output file.
 *
 * Returns 0 if successful, or 1 if an error occurs.
 *
 * dict: dictionary storing the declared variables
 * value: name of the variable to load
 * output_file: file to write LLVM IR to
 */
int load(Dictionary *dict, char *value, FILE *output_file);

/*
 * Declares the given variable, and writes the corresponding LLVM IR to the output file.
 *
 * Returns 0 if successful, or 1 if an error occurs.
 *
 * dict: dictionary storing the declared variables
 * variable: name of the variable to declare
 * output_file: file to write LLVM IR to
 */
int declare(Dictionary *dict, char *variable, FILE *output_file);

/*
 * Writes the value of the given register to the output file.
 *
 * output_file: file to write LLVM IR to
 * reg: token representing the register to print
 */
void print_var(FILE *output_file, Token* reg);

/*
 * Writes the LLVM IR for the given binary operator to the output file, using the given
 * left and right operand registers.
 *
 * Returns a pointer to the register containing the result of the operation.
 *
 * type: type of the binary operator
 * root: root token of the expression subtree
 * left_reg: register containing the left operand
 * right_reg: register containing the right operand
 * output_file: file to write LLVM IR to
 * error: pointer to boolean flag indicating whether an error occurred
 */
Token *print_op(enum SyntaxNodeType type, Token *root, Token *left_reg, Token *right_reg, FILE *output_file, bool *error);


/*
 * Stores the value of the given register in the given variable, and writes the corresponding
 * LLVM IR to the output file.
 *
 * name: name of the variable to store the value in
 * reg: token representing the register containing the value to store
 * output_file: file to write LLVM IR to
 */
void store(Dictionary *dict, char *name, Token *reg, FILE *output_file);


#endif
