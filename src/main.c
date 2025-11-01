#include "my_funcs.h"
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[100];
  char exit_str[100] = "exit";
  char echo_str[100] = "echo";
  char type_str[100] = "type";

  do {
    printf("$ ");

    fgets(input, sizeof(input), stdin);

    if (strcmp(input, "") != 0) {
      input[strlen(input) - 1] = '\0';
    } else {
      return 0;
    }

    if (strncmp(input, exit_str, 4) == 0) {
      // handle_exit();
    }

    if (strncmp(input, echo_str, 4) == 0) {
      handle_echo(input);
      continue;
    }

    if (strncmp(input, type_str, 4) == 0) {
      // handle_type();
    }

    printf("%s: command not found\n", input);

  } while (1);
  return 0;
}
