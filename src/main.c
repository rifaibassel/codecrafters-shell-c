#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  char input[100];
  // Uncomment this block to pass the first stage
  printf("$ ");
  scanf("%s", input);
  printf("%s: command not found", input);

  return 0;
}
