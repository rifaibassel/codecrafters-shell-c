#include "my_funcs.h"
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[100];
  char exit_str[100] = "exit";
  char echo_str[100] = "echo";
  char type_str[100] = "type";
  char pwd_str[100] = "pwd";

  do {
    printf("$ ");

    fgets(input, sizeof(input), stdin);

    if (strcmp(input, "") != 0) {
      input[strlen(input) - 1] = '\0';
    } else {
      return 0;
    }

    if (strncmp(input, exit_str, 4) == 0) {
      handle_exit(input);
      continue;
    }

    if (strncmp(input, echo_str, 4) == 0) {
      handle_echo(input);
      continue;
    }
    if (strcmp(input, pwd_str) == 0) {
      handle_pwd();
      continue;
    }

    if (strncmp(input, type_str, 4) == 0) {
      handle_type(input);
      continue;
    } else {
      execute_binary(input);
      continue;
    }

    printf("%s: not found", input);

  } while (1);
  return 0;
}
