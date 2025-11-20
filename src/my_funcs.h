#ifndef MY_FUNCS_H
#define MY_FUNCS_H
#define MAX_ARGS 64

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_exit(char *input);
void handle_echo(char *input);
void handle_type(char *input);
char *find_binary(const char *command);
void execute_binary(char *input);
void handle_pwd();
void parse_input(const char *input, char **argv, int max_args);
void handle_cd(char *path);
int is_cmd(const char *input, const char *cmd);
void handle_single_quote(const char *input);
#endif // !MY_FUNCS_H
