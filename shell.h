#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include<stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "builtins.h"
#include <dirent.h>

char* shell_read_line();

char** shell_parse_line(char* line);

int shell_launch(char** args);

int shell_execute(char** args);

void shell_loop();

void init_shell();

//system executable commands

extern char** system_commands;
extern int system_commands_size;

//get a system executable commands from PATH
void get_system_commands();
void clear_system_commands();


char* command_generate(const char* text, int state);
char** shell_completion(const char* text, int start, int end);


#endif //SHELL_H
