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
        printf("%s", found);
        free(found);
      }
    }
  }
}

char *find_binary(char *input) {
  char *path_env = getenv("PATH");
  char *path_env_str = malloc((strlen(path_env) + 1));
  strcpy(path_env_str, path_env);

  char *current_path = strtok(path_env_str, ": ");

  while (current_path != NULL) {
    char *paths_to_open[] = {current_path, NULL};
    FTS *handle = fts_open(paths_to_open, FTS_PHYSICAL, NULL);
    if (handle == NULL) {
      perror(current_path);
      current_path = strtok(NULL, ": ");
      continue;
    }
    FTSENT *fts_p = fts_read(handle);
    FTSENT *child = fts_children(handle, FTS_NAMEONLY);
    while (child) {
      if (strcmp(child->fts_accpath, input) == 0) {
        char *bin_name = child->fts_accpath;
        char *return_str = malloc(strlen(input) + strlen(child->fts_path) +
                                  strlen(child->fts_accpath) + 10);
        char *path =
            malloc(strlen(child->fts_path) + strlen(child->fts_accpath) + 1);
        sprintf(return_str, "%s is %s/%s\n", input, child->fts_path,
                child->fts_accpath);
        sprintf(path, "%s/%s", child->fts_path, child->fts_accpath);
        if (access(path, X_OK) == 0) {
          fts_close(handle);
          free(path_env_str);
          return return_str;
        }
      }
      child = child->fts_link;
    }
    fts_close(handle);
    current_path = strtok(NULL, ": ");
  }
  free(path_env_str);

  char *not_found_str = malloc(strlen(input) + strlen("%s: not found\n"));
  sprintf(not_found_str, "%s: not found\n", input);
  return not_found_str;
}
