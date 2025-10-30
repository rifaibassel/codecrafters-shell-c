#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[100];
  char exit_str[100] = "exit 0";
  char echo_str[100] = "echo";
  char type_str[100] = "type";

  do {
    printf("$ ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';
    if (strcmp(input, exit_str) == 0) {
      exit(0);
    }

    if (strncmp(input, echo_str, 4) == 0) {
      char *token = strtok(input, " ");
      token = strtok(NULL, " ");
      if (token == NULL) {
        printf("$");
      }
      while (token != NULL && strcmp(token, "(null)") != 0) {
        printf("%s ", token);
        token = strtok(NULL, " ");
      }
      printf("\n");
      continue;
    }

    if (strncmp(input, type_str, 4) == 0) {
      char *token = strtok(input, " ");
      token = strtok(NULL, " ");
      if (token == NULL) {
        printf("$");
      }
      while (token != NULL && strcmp(token, "(null)") != 0) {
        if (strcmp(token, echo_str) == 0 || strcmp(token, exit_str) == 0 ||
            strcmp(token, type_str) == 0) {
          printf("%s is a shell builtin\n", token);
          token = strtok(NULL, " ");
        } else {
          printf("%s: command not found\n", token);
          token = strtok(NULL, " ");
        }
      }
      printf("\n");
      continue;
    }

    printf("%s: command not found\n", input);

  } while (1);
  return 0;
}
