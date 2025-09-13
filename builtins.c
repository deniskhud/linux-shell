#include "builtins.h"

short get_count_argc(char** argv) {
    short count = 0;
    while (argv[count] != NULL) {
        ++count;
    }
    return count;
}

builtin_command builtins[] = {
    {"cd", shell_cd},
    {"help", shell_help},
    {"exit", shell_exit},
    {"history", shell_history},

};

int shell_count_builtins() {
    return sizeof(builtins) / sizeof(builtin_command);
}

/*** cd ***/
int shell_cd(char** args) {
    short count = get_count_argc(args);

    switch (count) {
        case 1: {
            const char* home = getenv("HOME");
            if (home == NULL) {home = "/";} //if home not exists
            if (chdir(home) != 0) {
                perror("chdir");
            }
            return 1;
        }
        case 2: {
            //try to change directory
            if (chdir(args[1]) != 0 ) {
                fprintf(stderr, "error: directory is not exists\n");
            }
            return 1;
        }
        default: {
            fprintf(stderr, "error: command not found\n");
        }
    }
    return 1;
}
/*** help ***/
int shell_help(char** args) {
    short count = get_count_argc(args);

    switch (count) {
        case 1: {

        }
    }

}
int shell_exit(char** args) {
    return 0;
}


/*** history ***/

void load_history_file() {
    read_history(HISTORY_FILE);
}
void save_history_file() {
    write_history(HISTORY_FILE);
}

/*** history helpful functions ***/

/*show a current history */
int show_history() {

    HIST_ENTRY** h = history_list(); //get history list

    if (!h) return -1;

    for (int i = 0; h[i]; ++i) {
        printf("%s\n", h[i]->line);
    }
    return 0;
}

void set_history_size(int size) {
    stifle_history(size);
}



// main history func
int shell_history(char** args) {
    short count = get_count_argc(args);

    switch (count) {
        case 1: {
            if (strcmp(args[0], "history") == 0) {
                if (show_history() == -1) {
                    fprintf(stderr, "error: history not found\n");
                }
            }
        }
        case 2: {

        }
    }
    return 1;
}




