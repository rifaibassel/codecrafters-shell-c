#include "my_funcs.h"
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void free_argv(char **argv) {
  for (int i = 0; argv[i] != NULL; i++) {
    free(argv[i]);
  }
}

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
          strcmp(input, "type") == 0 || strcmp(input, "pwd") == 0 ||
          strcmp(input, "cd") == 0) {
        printf("%s is a shell builtin\n", input);
      } else {
        char *found = find_binary(input);
        if (found != NULL) {

          printf("%s is %s\n", input, found);
        } else {
          printf("%s: not found\n", input);
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
    char candidate[512];
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

void execute_binary(char *input) {
  char *argv[MAX_ARGS];
  parse_input(input, argv, MAX_ARGS);
  if (argv[0] == NULL) {
    return;
  }
  char *path = find_binary(argv[0]);

  if (!path) {
    printf("%s: command not found\n", argv[0]);
    free_argv(argv);
    return;
  }
  pid_t pid = fork();
  if (pid == 0) {
    execv(path, argv);
    perror("execv");
    exit(1);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
    free_argv(argv);
  } else {
    free_argv(argv);
    perror("fork");
  }
  free(path);
}

void parse_input(const char *input, char **argv, int max_args) {
  int in_quote = 0;
  int arg_idx = 0;
  char token[256] = {0};
  int t = 0;
  for (int i = 0; input[i] != '\0'; ++i) {
    char c = input[i];
    if (c == '"') {
      in_quote = !in_quote;
    } else if (c == ' ' && !in_quote) {
      if (t > 0) {
        token[t] = '\0';
        if (arg_idx < max_args - 1) {
          argv[arg_idx++] = strdup(token);
        }
        t = 0;
      }
    } else {
      if (t < (int)sizeof(token) - 1)
        token[t++] = c;
    }
  }
  if (t > 0) {
    token[t] = '\0';
    if (arg_idx < max_args - 1)
      argv[arg_idx++] = strdup(token);
  }
  argv[arg_idx] = NULL;
}

void handle_pwd() {
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
  }
}

void handle_cd(char *path) {
  char cwd[PATH_MAX];
  if (path == NULL || strcmp(path, "") == 0) {
    const char *home_env = getenv("HOME");
    if (!home_env) {
      return;
    }
    if (chdir(home_env) != 0) {
      perror("cd");
      return;
    }
  } else {
    if (chdir(path) != 0) {
      printf("cd: %s: No such file or directory\n", path);
      return;
    }
  }

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    setenv("PWD", cwd, 1);
  }
}

int is_cmd(const char *input, const char *cmd) {
  size_t len_of_cmd = strlen(cmd);
  return strncmp(input, cmd, len_of_cmd) == 0 &&
         (input[len_of_cmd] == '\0' || input[len_of_cmd] == ' ');
}
