#include <libgen.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void parse_command(char *input, char **argv) {
  int argv_idx = 0;
  int input_len = strlen(input);
  char token[1024];
  int token_idx = 0;
  int quote_mode_flag = 0;

  for (int i = 0; i < input_len; i++) {
    if (quote_mode_flag == 1) {
      if (input[i] == '\'') {
        quote_mode_flag = 0;
      } else {
        token[token_idx++] = input[i];
      }
    } else if (quote_mode_flag == 2) {
      if (input[i] == '\"') {
        quote_mode_flag = 0;
      } else {
        token[token_idx++] = input[i];
      }
    } else {
      if (input[i] == '\'') {
        quote_mode_flag = 1;
      } else if (input[i] == '\"') {
        quote_mode_flag = 2;
      } else if (input[i] == ' ') {
        if (token_idx > 0) {
          token[token_idx] = '\0';
          argv[argv_idx++] = strdup(token);
          token_idx = 0;
        }
      } else if (input[i] == '\\') {
        if (input[i + 1]) {
          char next_c = input[i + 1];
          token[token_idx++] = next_c;
          i++;
        }
      } else {
        token[token_idx++] = input[i];
      }
    }
  }

  if (token_idx > 0) {
    token[token_idx] = '\0';
    argv[argv_idx++] = strdup(token);
  }
  argv[argv_idx] = NULL;
}

void handle_echo(char **argv) {
  int argv_idx = 1;
  while (argv[argv_idx] != NULL) {
    printf("%s", argv[argv_idx]);
    if (argv[argv_idx + 1] != NULL) {
      printf(" ");
    }
    argv_idx++;
  }
  printf("\n");
}

int find_path(char *command, char *buffer) {
  const char *path_var = getenv("PATH");
  if (path_var == NULL)
    return -1;
  char path_var_dup[PATH_MAX];

  strcpy(path_var_dup, path_var);

  char *path_var_saveptr;
  char *path_token = strtok_r(path_var_dup, ":", &path_var_saveptr);

  while (path_token != NULL) {
    char full_path[PATH_MAX];
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
    path_token = strtok_r(NULL, ":", &path_var_saveptr);
  }

  return -1;
}

void handle_type(char **argv) {
  if (argv[1] == NULL) {
    return;
  }
  char path[PATH_MAX];
  if (strcmp(argv[1], "echo") == 0 || strcmp(argv[1], "exit") == 0 ||
      strcmp(argv[1], "type") == 0 || strcmp(argv[1], "pwd") == 0) {
    printf("%s is a shell builtin\n", argv[1]);
  } else if (find_path(argv[1], path) == 1) {
    printf("%s is %s\n", argv[1], path);
  } else {
    printf("%s: not found\n", argv[1]);
  }
}

int handle_exec(char **argv) {
  char path[PATH_MAX];
  if (find_path(argv[0], path) == 1) {
    pid_t parent_pid = getpid();
    pid_t child_pid = fork();

    if (child_pid == -1) {
      perror("fork failed");
      return -1;
    } else if (child_pid > 0) {
      int status;
      waitpid(child_pid, &status, 0);
    } else {
      execv(path, argv);
      exit(EXIT_FAILURE);
    }
    return 1;
  }
  return -1;
}

void handle_cd(char **argv) {
  const char *home_env = getenv("HOME");
  char cwd[PATH_MAX];
  char expanded_path[PATH_MAX];
  char *target_dir;

  if (argv[1] == NULL || strcmp(argv[1], "") == 0 ||
      strcmp(argv[1], "~") == 0) {
    if (chdir(home_env) == 0) {
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
        setenv("PWD", cwd, 1);
      }
      return;
    } else {
      perror("CHDIR failed");
      return;
    }
  }

  if (argv[1][0] == '~') {
    snprintf(expanded_path, sizeof(expanded_path), "%s%s", home_env,
             argv[1] + 1);
    target_dir = expanded_path;
  } else {
    target_dir = argv[1];
  }
  if (chdir(target_dir) == 0) {
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      setenv("PWD", cwd, 1);
      return;
    }
  }
  printf("cd: %s: No such file or directory\n", argv[1]);
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char command[1024];
  do {
    printf("$ ");
    // Get command
    fgets(command, sizeof(command), stdin);
    if (strcmp(command, "\n") == 0)
      continue;
    command[strcspn(command, "\n")] = '\0';

    char *argv[64];
    parse_command(command, argv);
    if (argv[0] == NULL) {
      continue;
    }
    // Command is exit without arguments
    if (strcmp(argv[0], "exit") == 0) {
      exit(EXIT_SUCCESS);
    } else if (strcmp(argv[0], "echo") == 0) {
      handle_echo(argv);
    } else if (strcmp(argv[0], "type") == 0) {
      handle_type(argv);
    } else if (handle_exec(argv) == 1) {
      continue;
    } else if (strcmp(argv[0], "pwd") == 0) {
      const char *pwd_path = getenv("PWD");
      printf("%s\n", pwd_path);
    } else if (strcmp(argv[0], "cd") == 0) {
      handle_cd(argv);
    } else {
      printf("%s: command not found\n", command);
    }
  } while (1);

  return 0;
}
