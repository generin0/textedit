#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUILTINS_ERROR   1
#define BUILTINS_SUCCESS 0

#define GREEN  "\x1B[32m"
#define RED    "\x1B[31m"
#define RESET  "\x1B[0m"

int file_exists(const char *filename) {
  return access(filename, F_OK);
}

int print_file(FILE *file) {
  char text[1000];

  while(fgets(text, sizeof(text), file) != NULL) {
    printf("%s", text);
  }

  fclose(file);
  return BUILTINS_SUCCESS;
}

int output(FILE *file, char *buffer, size_t size) {
  printf(GREEN"> "RESET);

  while(fgets(buffer, size, stdin) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';
    fprintf(file, "%s\n", buffer);
    printf(GREEN"> "RESET);
  }

  return BUILTINS_SUCCESS;
}

int main(int argc, char *argv[]) {
  char buffer[256] = {0};
  char file_name[256] = {0};
  FILE *file = NULL;

  if(argc >= 3 && strcmp("r", argv[2]) == 0) {
    file = fopen(argv[1], "r");
    if(!file){
      printf(RED"Cannot open the file.\n"RESET);
      return BUILTINS_ERROR;
    }
    print_file(file);
    return BUILTINS_SUCCESS;
  }

  if(!file_exists(argv[1])) {
    file = fopen(argv[1], "w+");
    return output(file, buffer, sizeof(buffer));
  }

  printf(GREEN"Enter the file name > "RESET);

  if(fgets(file_name, sizeof(file_name), stdin) == NULL) {
    printf(RED"Error reading the file name.\n"RESET);
  :  return BUILTINS_ERROR;
  }

  file_name[strcspn(file_name, "\n")] = '\0';

  if(strlen(file_name) == 0) {
    printf(RED"File name cannot be empty.\n"RESET);
    return BUILTINS_ERROR;
  }
  
  file = fopen(file_name, "w+");

  if(!file) {
    perror(RED"Cannot open the file to write."RESET);
    return BUILTINS_ERROR;
  }
  
  int result = output(file, buffer, sizeof(buffer));
  fclose(file);
  return result;
}
