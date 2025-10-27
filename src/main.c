#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[100];
  char exit_str[100] = "exit 0";
  do {
    printf("$ ");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';
    if (strcmp(input, exit_str) == 0) {
      exit(0);
    }
    printf("%s: command not found\n", input);

  } while (1);
  return 0;
}
