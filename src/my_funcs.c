#include "my_funcs.h"

void handle_echo(char *str_to_echo) {

  if (str_to_echo != NULL) {
    str_to_echo = strtok(str_to_echo, " ");
    while (str_to_echo != NULL) {
      str_to_echo = strtok(NULL, " ");
      if (str_to_echo == NULL)
        continue;
      printf("%s ", str_to_echo);
    }
    printf("\n");
  } else {
    printf("");
  }
}
