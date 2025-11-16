#include "my_funcs.h"
#include <string.h>

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  char input[100];

  do {
    printf("$ ");
    if (!fgets(input, sizeof(input), stdin)) {
      break;
    }

    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
    }
    if (input[0] == '\0') {
      continue;
    }

    if (is_cmd(input, "exit")) {
      handle_exit(input);
      continue;
    }

    if (is_cmd(input, "echo")) {
      handle_echo(input);
      continue;
    }
    if (is_cmd(input, "pwd")) {
      handle_pwd();
      continue;
    }

    if (is_cmd(input, "cd")) {
      char *cmd = strtok(input, " ");
      char *path = strtok(NULL, " ");
      handle_cd(path);
      continue;
    }

    if (is_cmd(input, "type")) {
      handle_type(input);
      continue;
    } else {
      execute_binary(input);
      continue;
    }

  } while (1);

  return 0;
}
