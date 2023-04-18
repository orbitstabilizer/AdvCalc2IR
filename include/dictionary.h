#ifndef DICTIONARY_H
#define DICTIONARY_H
#define HASHSIZE 31013
#include <stdint.h>
typedef int32_t operand_t;
/*
 * Chain struct
 * next: pointer to next entry in chain
 * name: name of variable
 * value: value of variable
 */
typedef struct Chain Chain;
typedef struct Chain { /* table entry: */
    Chain *next; /* next entry in chain */
    char *name; /* defined name */
    operand_t value;
}Chain;

/*
 * Dictionary struct
 * hashtab: array of pointers to chains
 */
typedef struct Dictionary Dictionary;

struct Dictionary{
  Chain* hashtab[HASHSIZE];   
};

/*
 * Create a new dictionary
 */
Dictionary* new_dictionary();

/*
 * Set a variable in the dictionary
 * dict: dictionary to set variable in
 * name: name of variable
 * value: value of variable
 */
void set_var(Dictionary *dict, char *name, operand_t value);

/*
 * Get a variable from the dictionary
 * dict: dictionary to get variable from
 * name: name of variable
 */
operand_t get_var(Dictionary *dict, char *name, int* is_declared);

/*
 * Free a dictionary
 * dict: dictionary to free
 */
void free_dict(Dictionary* dict);

/*
 * Print a dictionary
 * dict: dictionary to print
 */
void print_dict(Dictionary* dict);

/*
 * Hash function
 * s: string to hash
 */
unsigned hash(char *s);


#endif
