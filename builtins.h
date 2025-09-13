#ifndef BUILTINS_H
#define BUILTINS_H
#define HISTORY_FILE "/home/khud/projects/shell_c/history.txt"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/history.h>

//func for getting count args line
short get_count_argc(char** argv);


int shell_cd(char** args);

int shell_exit(char** args);

int shell_help(char** args);




int shell_history(char** args);
void load_history_file();
void save_history_file();

int shell_version(char** args);

typedef int (*builtin_fn)(char **);

typedef struct builtin_command {
    char* name;
    builtin_fn fn;
} builtin_command;

extern builtin_command builtins[];



int shell_count_builtins();

#endif //BUILTINS_H
