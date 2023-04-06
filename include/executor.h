#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"
#include "dictionary.h"
/*
 * This function executes the statetement given as a input string, and writes the output to the output string.
 * return value represents the status of the execution.
 * 0: expression
 * 1: assignment
 * 2: error
 * 3: empty line
 *
 * dict: dictionary to store or retrieve variables
 * 
 * The function creates its own lexer and parser, and uses them to parse the input string. It then executes the parsed statement.
 * The output string is written to the output string.
 */
int exec(Dictionary *dict, char *input, char *output);

#endif
