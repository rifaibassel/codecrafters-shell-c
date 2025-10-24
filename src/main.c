#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char input[100];

  printf("$ ");
  fgets(input, sizeof(input), stdin);
  input[strlen(input) - 1] = '\0';

  printf("%s: command not found", input);

  return 0;
}
