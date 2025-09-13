#include "shell.h"

#include <stdio.h>

char* shell_read_line() {
    char* line = readline("> ");
    if (line == NULL) {
        fprintf(stderr, "Error reading input\n");
    }
    if (line && *line) {

        add_history(line);  //add to history
    }
    return line;
}



#define PARSE_BUFFER_SIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"
char** shell_parse_line(char* line) {
    int buffsize = PARSE_BUFFER_SIZE, pos = 0;
    char** tokens = (char**)malloc(sizeof(char*) * buffsize);
    char* token;

    if (!tokens) {
        fprintf(stderr, "Error allocating memory\n");
        exit(1);
    }
    //tokenize
    token = strtok(line, SHELL_TOK_DELIM);

    while (token != NULL) {
        tokens[pos++] = token;

        //reallocating memory
        if (pos >= buffsize) {
            buffsize += PARSE_BUFFER_SIZE;

            tokens = (char**)realloc(tokens, sizeof(char*) * buffsize);
            if (!tokens) {
                fprintf(stderr, "Error allocating memory\n");
                exit(1);
            }
        }
        token = strtok(NULL, SHELL_TOK_DELIM);
    }
    tokens[pos] = NULL;
    return tokens;
}

int shell_launch(char** args) {
    pid_t pid, wpid;

    int status;

    pid = fork();
    if (pid == 0) {

        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(1);
    }
    else if (pid < 0) {
        perror("fork");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int shell_execute(char** args) {
    int i;

    if (args[0] == NULL) {
        return 1;
    }
    //проверка на правильность введенной команды
    for (i = 0; i < shell_count_builtins(); ++i) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            return builtins[i].fn(args); //вызываем именно ту команду, которую написали
        }
    }
    return shell_launch(args);
}

void shell_loop() {
    char* line;
    char** args;
    int status;

    do {
        //wprintf("> ");
        line = shell_read_line();
        args = shell_parse_line(line);
        status = shell_execute(args);

        save_history_file();
        free(line);
        free(args);
    }while (status);
}




void init_shell() {
    using_history();
    stifle_history(1000);
    load_history_file();
    get_system_commands(); //get
    rl_attempted_completion_function = shell_completion;

    shell_loop();

    save_history_file();
    clear_system_commands();

}



char* command_generator(const char* text, int state) {
    static int list_index, len;
    char* name;

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = system_commands[list_index++])) {
        if (strncmp(name, text, len) == 0)
            return strdup(name);
    }

    return NULL;
}

char** shell_completion(const char* text, int start, int end) {
    return rl_completion_matches(text, command_generator);
}

/*** get a system executable commands ***/
char** system_commands = NULL;
int system_commands_size = 0;
int system_commands_capacity = 2566;

void get_system_commands() {
    char* path_env = getenv("PATH");
    if (!path_env) return;

    //copy the current path
    char* path_copy = strdup(path_env); // malloc
    char* dir = strtok(path_copy, ":");

    //malloc
    system_commands = malloc(sizeof(char*) * system_commands_capacity);

    while (dir != NULL) {
        DIR* d = opendir(dir);
        if (d) {
            struct dirent* entry;
            while ((entry = readdir(d)) != NULL) {
                char fullpath[1024];
                snprintf(fullpath, 1024, "%s/%s", dir, entry->d_name);

                //if command is executable
                if (access(fullpath, X_OK) == 0) {
                    system_commands[system_commands_size++] = strdup(entry->d_name);    //add to array

                    //realloc
                    if (system_commands_size >= system_commands_capacity) {
                        system_commands_capacity *= 2;
                        system_commands = realloc(system_commands, sizeof(char*) * system_commands_capacity);
                    }
                }
            }
            closedir(d);
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
}

void clear_system_commands() {
    for (int i = 0; i < system_commands_size; ++i) {
        free(system_commands[i]);
    }
    free(system_commands);
    system_commands = NULL;
    system_commands_size = 0;
    system_commands_capacity = 0;
}
