#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 
#include "executor.h"
#include "debug.h"


#define BUFFER_SIZE 256


int main() {
  // plus 1 for null terminator 
  char buffer[BUFFER_SIZE+1];
  Dictionary *dict = new_dictionary();
  while (true) {
    printf("> ");
    TODO("Fix buffer overflow, when input is too long it carrys over to the next line");
    if(fgets(buffer, BUFFER_SIZE+1, stdin) == NULL)
      break;
    // int c;
    // while ((c = getchar()) != '\n' && c != EOF) { }
    char output[31];
    int state = exec(dict,buffer,output);
    switch (state) {
      case 0:
        printf("%s\n", output);
        break;
      case 2:
        printf("Error!\n");
        break;
      default:
        break;
    }
  }
  if(dict)
    free_dict(dict);
  return 0;

}
