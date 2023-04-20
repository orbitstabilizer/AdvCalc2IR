#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include "executor.h"
#include "debug.h"


#define BUFFER_SIZE 257
#define MAX_FILENAME 30
void prelog(FILE* fp);
void epilog(FILE* fp);
/*
; ModuleID = 'advcalc2ir'
declare_var i32 @printf(i8*, ...)
@print.str = constant [4 x i8] c"%d\0A\00"

define i32 @lr(i32 %x, i32 %c) {
  %l1 = shl i32 %x, %c
  %l2 = sub i32 32, %c
  %l3 = lshr i32 %x, %l2
  %l4 = or i32 %l1, %l3
  ret i32 %l4
}

define i32 @rr(i32 %x, i32 %c) {
  %l1 = lshr i32 %x, %c
  %l2 = sub i32 32, %c
  %l3 = shl i32 %x, %l2
  %l4 = or i32 %l1, %l3
  ret i32 %l4
}

define i32 @not(i32 %x) {
  %n1 = xor i32 %x, -1
  ret i32 %n1
}

define i32 @main() {
*/
void prelog(FILE* fp){
  fprintf(fp, "; ModuleID = 'advcalc2ir'\n\n");
  fprintf(fp, "declare i32 @printf(i8*, ...)\n");
  fprintf(fp, "@print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n\n");
  fprintf(fp, "define i32 @lr(i32 %%x, i32 %%c) {\n");
  fprintf(fp, "  %%l1 = shl i32 %%x, %%c \n");
  fprintf(fp, "  %%l2 = sub i32 32, %%c \n");
  fprintf(fp, "  %%l3 = lshr i32 %%x, %%l2 \n");
  fprintf(fp, "  %%l4 = or i32 %%l1, %%l3 \n");
  fprintf(fp, "  ret i32 %%l4 \n");
  fprintf(fp, "}\n");
  fprintf(fp, "define i32 @rr(i32 %%x, i32 %%c) {\n");
  fprintf(fp, "  %%l1 = lshr i32 %%x, %%c \n");
  fprintf(fp, "  %%l2 = sub i32 32, %%c \n");
  fprintf(fp, "  %%l3 = shl i32 %%x, %%l2 \n");
  fprintf(fp, "  %%l4 = or i32 %%l1, %%l3 \n");
  fprintf(fp, "  ret i32 %%l4 \n");
  fprintf(fp, "}\n");
  fprintf(fp, "define i32 @not(i32 %%x) {\n");
  fprintf(fp, "  %%n1 = xor i32 %%x, -1 \n");
  fprintf(fp, "  ret i32 %%n1 \n");
  fprintf(fp, "}\n\n");
  fprintf(fp, "define i32 @main() {\n");

}
void epilog(FILE* fp){
  fprintf(fp, "  ret i32 0\n");
  fprintf(fp, "}\n");
}

int main(int argc, char* argv[]) {

  if (argc < 2) {
    LOG_ERROR("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    LOG_ERROR("Could not open file %s", argv[1]);
  }
  int line_number = 1 ;
  // split filename file.adv to file
  char *filename = strtok(argv[1], ".");
  char output_filename[MAX_FILENAME];
  strcpy(output_filename, filename);
  strcat(output_filename, ".ll");

  FILE *output_file = fopen(output_filename, "w");
  if (output_file == NULL) {
    LOG_ERROR("Could not open file %s", output_filename);
  }
  prelog(output_file);

  char buffer[BUFFER_SIZE];
  Dictionary *dict = new_dictionary();
  while (true) {
    // fprintf(output_file, "> ");
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL)
      break;
    // char output[31];
    int state = exec(dict,buffer,output_file);
    switch (state) {
      case 0:
        // printf("%s\n", output);
        // fprintf(output_file, "%s\n", output);
        break;
      case 2:
        printf("Error on line %d\n", line_number);
        // fprintf(output_file, "Error!\n");
        break;
      default:
        break;
    }
    line_number++;
  }
  if(dict)
    free_dict(dict);
  epilog(output_file);
  fclose(fp);
  return 0;

}


