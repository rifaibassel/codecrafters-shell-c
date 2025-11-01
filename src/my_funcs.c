#include "my_funcs.h"

void handle_echo(char *input) {

  if (input != NULL) {
    input = strtok(input, " ");
    while (input != NULL) {
      input = strtok(NULL, " ");
      if (input == NULL)
        continue;
      printf("%s ", input);
    }
    printf("\n");
  } else {
    printf("");
  }
}

void handle_exit(char *input) {
  input = strtok(input, " ");
  if (input != NULL) {
    input = strtok(NULL, " ");
    if (input != NULL) {
      if (strcmp(input, "0") == 0) {
        exit(0);
      } else if (strcmp(input, "1") == 0) {
        exit(1);
      }
    } else {
      exit(0);
    }
  }
}
