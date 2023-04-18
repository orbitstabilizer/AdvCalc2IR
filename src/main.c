#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 
#include "executor.h"
#include "debug.h"


#define BUFFER_SIZE 257
#define MAX_FILENAME 30

int main(int argc, char* argv[]) {
  if (argc < 2) {
    LOG_ERROR("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    LOG_ERROR("Could not open file %s", argv[1]);
  }
  // split filename file.adv to file
  char *filename = strtok(argv[1], ".");
  char output_filename[MAX_FILENAME];
  strcpy(output_filename, filename);
  strcat(output_filename, ".ll");

  FILE *output_file = fopen(output_filename, "w");

  char buffer[BUFFER_SIZE];
  Dictionary *dict = new_dictionary();
  while (true) {
    fprintf(output_file, "> ");
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL)
      break;
    char output[31];
    int state = exec(dict,buffer,output);
    switch (state) {
      case 0:
        // printf("%s\n", output);
        fprintf(output_file, "%s\n", output);
        break;
      case 2:
        // printf("Error!\n");
        fprintf(output_file, "Error!\n");
        break;
      default:
        break;
    }
  }
  if(dict)
    free_dict(dict);
  return 0;

}


