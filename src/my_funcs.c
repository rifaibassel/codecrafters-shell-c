#include "my_funcs.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

void handle_type(char *input) {
  input = strtok(input, " ");
  if (input != NULL) {
    input = strtok(NULL, " ");
    if (input) {
      if (strcmp(input, "echo") == 0 || strcmp(input, "exit") == 0 ||
          strcmp(input, "type") == 0) {
        printf("%s is a shell builtin\n", input);
      } else {
        char *found = find_binary(input);
        if (found != NULL) {

          printf("%s is %s\n", input, found);
        }
        free(found);
      }
    }
  }
}

char *find_binary(const char *command) {
  const char *path_env = getenv("PATH");
  if (!path_env)
    return NULL;
  char *path_copy = strdup(path_env);
  char *dir = strtok(path_copy, ":");
  while (dir) {
    char candidate[512]; // More robust: dynamic alloc
    snprintf(candidate, sizeof(candidate), "%s/%s", dir, command);
    if (access(candidate, X_OK) == 0) {
      char *result = strdup(candidate);
      free(path_copy);
      return result;
    }
    dir = strtok(NULL, ":");
  }
  free(path_copy);
  return NULL;
}

// Contract: void execute_binary(char *input)
// Purpose: parse input, find path, fork-and-exec, print error if needed
void execute_binary(char *input) {
  // 1. Parse input → argv[]
  // 2. Find full path using find_binary
  // 3. If not found, print error
  // 4. Else, fork
  //    - in child: execv(path, argv)
  //    - in parent: wait for child
}
