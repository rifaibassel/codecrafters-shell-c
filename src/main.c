#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char command[1024];
  do {
    printf("$ ");
    // Get command
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';

    // Command is exit without arguments
    if (strcmp(command, "exit") == 0) {
      exit(EXIT_SUCCESS);
    }

    char *command_saveptr;
    char *command_token = strtok_r(command, " ", &command_saveptr);

    if (strcmp(command_token, "echo") == 0) {
      char echo_buffer[1024];
      printf("%s\n", command_saveptr);
    } else {
      printf("%s: command not found\n", command);
    }
  } while (1);

  return 0;
}
