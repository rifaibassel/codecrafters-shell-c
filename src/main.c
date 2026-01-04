#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int find_path(char *command, char *buffer) {
  const char *path_var = getenv("PATH");
  if (path_var == NULL)
    return -1;
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

void get_argv(char *arguments, char **buffer, char *command) {
  buffer[0] = command;
  if (arguments != NULL) {
    char *arg_saveptr;
    char *arg = strtok_r(arguments, " ", &arg_saveptr);
    int arg_idx = 1;
    while (arg != NULL) {
      if (arg_idx >= 63) {
        break;
      }
      buffer[arg_idx] = arg;
      arg = strtok_r(NULL, " ", &arg_saveptr);
      arg_idx++;
    }
    buffer[arg_idx] = NULL;
  } else {
    buffer[1] = NULL;
  }
}

int handle_exec(char *command, char *rest_of_command) {
  char path[1024];
  if (find_path(command, path) == 1) {
    pid_t parent_pid = getpid();
    pid_t child_pid = fork();

    if (child_pid == -1) {
      perror("fork failed");
      return -1;
    } else if (child_pid > 0) {
      int status;
      waitpid(child_pid, &status, 0);
    } else {
      char *argv[64];
      get_argv(rest_of_command, argv, command);
      execv(path, argv);
      exit(EXIT_FAILURE);
    }

    return 1;
  }
  return -1;
}

void handle_type(char *command) {
  if (strcmp(command, "echo") == 0 || strcmp(command, "exit") == 0 ||
      strcmp(command, "type") == 0 || strcmp(command, "pwd") == 0 ||
      strcmp(command, "cd") == 0) {
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
    if (strcmp(command, "\n") == 0)
      continue;
    command[strcspn(command, "\n")] = '\0';

    // Command is exit without arguments
    if (strcmp(command, "exit") == 0) {
      exit(EXIT_SUCCESS);
    }

    char *command_saveptr;
    char *command_token = strtok_r(command, " ", &command_saveptr);
    if (command_token == NULL)
      continue;

    if (strcmp(command_token, "echo") == 0) {
      char echo_buffer[1024];
      printf("%s\n", command_saveptr);
    } else if (strcmp(command_token, "type") == 0) {
      handle_type(command_saveptr);
    } else if (strcmp(command_token, "pwd") == 0) {
      char *pwd_env = getenv("PWD");
      printf("%s\n", pwd_env);
    } else if (strcmp(command_token, "cd") == 0) {
      if (chdir(command_saveptr) == 0) {
        setenv("PWD", command_saveptr, 1);
      } else {
        printf("cd: %s: No such file or directory\n", command_saveptr);
      }
    } else {
      if (handle_exec(command, command_saveptr) == 1) {
        continue;
      } else {
        printf("%s: command not found\n", command);
      }
    }
  } while (1);

  return 0;
}
