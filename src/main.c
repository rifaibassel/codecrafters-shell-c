#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int find_path(char *command, char *buffer) {
  const char *path_var = getenv("PATH");
  char path_var_dup[1024];

  strcpy(path_var_dup, path_var);

  char *path_var_saveptr;
  char *path_token = strtok_r(path_var_dup, ": ", &path_var_saveptr);

  while (path_token != NULL) {
    char full_path[1024];
    if (path_token[strlen(path_token) - 1] == '/') {
      strcpy(full_path, path_token);
      strcat(full_path, command);
      int access_status = access(full_path, X_OK);
      if (access_status == 0) {
        strcpy(buffer, full_path);
        return 1;
      }
    } else {
      strcpy(full_path, path_token);
      strcat(full_path, "/");
      strcat(full_path, command);
      int access_status = access(full_path, X_OK);
      if (access_status == 0) {
        strcpy(buffer, full_path);
        return 1;
      }
    }
    path_token = strtok_r(NULL, ": ", &path_var_saveptr);
  }

  return -1;
}

void handle_type(char *command) {
  if (strcmp(command, "echo") == 0 || strcmp(command, "exit") == 0 ||
      strcmp(command, "type") == 0) {
    printf("%s is a shell builtin\n", command);
  } else {
    char full_path[1024];
    if (find_path(command, full_path) == 1) {
      printf("%s is %s\n", command, full_path);

      return;
    }
    printf("%s: not found\n", command);
  }
}

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
    } else if (strcmp(command_token, "type") == 0) {
      handle_type(command_saveptr);
    } else {
      printf("%s: command not found\n", command);
    }
  } while (1);

  return 0;
}
