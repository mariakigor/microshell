#include <stdio.h>
#include <string.h>
#include <unistd.h>  //  for getcwd()
#include <stdlib.h> // for getenv()
#include <fcntl.h> // file open
#include <ctype.h> // isspace()
#include <readline/readline.h>  
#include <readline/history.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h> // ctrl c

#define PATH_MAX 4096 
#define MAX_INPUT 1024

char current_color[20] = "\033[0m";

void command_prompt() {
    char buf[PATH_MAX]; 

    const char *user = getenv("LOGNAME"); // user
    const char *host = getenv("NAME"); // host

    if ((getcwd(buf, sizeof(buf)) != NULL) && (user != NULL) && (host != NULL)) {
        printf("%s%s@%s:[%s] %s$%s", current_color, user, host, buf, current_color, "\033[0m"); // function to store path 
    } else {
        perror("getcwd error"); // error handle
        perror("getenv error");
    }
}


void color(const char *color) {
    if (strcmp(color, "--green") == 0) {
        strcpy(current_color, "\033[0;32m");  
    } else if (strcmp(color, "--red") == 0) {
        strcpy(current_color, "\033[0;31m"); 
    } else if (strcmp(color, "--blue") == 0) {
        strcpy(current_color, "\033[0;34m"); 
    } else if (strcmp(color, "--yellow") == 0) {
        strcpy(current_color, "\033[0;33m");  
     } else if (strcmp(color, "--magenta") == 0) {
        strcpy(current_color, "\033[0;35m");  
    } else if (strcmp(color, "--cyan") == 0) {
        strcpy(current_color, "\033[0;36m");  
    } else if (strcmp(color, "reset") == 0) {
        strcpy(current_color, "\033[0m");  
    } else {
        printf("Unknown color, for reset type color reset. Available: --green, --red, --blue, --yellow, --magenta, --cyan\n");
    }
}


// cd command
void cd(char *path) {
    if (path == NULL) {
        chdir(getenv("HOME"));
    } else {
        if (chdir(path) == -1) {
            int number;
            number = errno;
            if (number == 2) {
                printf("No such file or directory\n");
            }
            else if (number == 13) {
                printf("Permision denied\n");
            }
            else {
                printf("Error / not a directory\n");
            }
        } else {
            chdir(path);
        }
        
    }

}

// cp command
void cp(char *source_file, char *dest_file) {
    if (source_file == NULL) {
        printf("Missing source file\n");
    }

    if (dest_file == NULL) {
        printf("Missing destination file\n");
    }

    int file, new_file;
    char buffer[4096];
    int bytes;

    // source file
    file = open(source_file, O_RDONLY);
    if (file == -1) {
        printf("Cannot open source file\n");
        close(file);
        return;
    }

    // destination file
    new_file = open(dest_file, O_WRONLY | O_CREAT, 0644);  // O_CREAT needs privilege 0 owner (6 - rw_), group (4 - r__), others (4-r__)
    if (new_file == -1) {
        printf("Cannot create destiantion file\n");
        close(new_file);
        return;
    }

    while ( (bytes = read(file, buffer, sizeof(buffer))) > 0 ) {
        write(new_file, buffer, bytes);
    }

    close(new_file);
    close(file);

}

// grep command
void grep_command(char *pattern, char *f) {
    if (pattern == NULL || f == NULL) {
        printf("Try grep PATTERN FILE\n");
        return;
    }

    FILE *file = fopen(f, "r");
    if (file == NULL) {
        printf("Cannot open a file");
        return;
    }

    char line[1024];
    int line_number = 1;
    const char *start = "\033[1;31m"; // color found
    const char *end = "\033[0m"; // color reset

    while (fgets(line, sizeof(line), file) != NULL) {
        char *match = strstr(line, pattern); // first occurance of a pattern
        if (match != NULL) {
            printf("Line %d: ", line_number); 

            char *current = line;
            while (match != NULL) {
                fwrite(current, 1, match - current, stdout);

                printf("%s", start);
                fwrite(match, 1, strlen(pattern), stdout);
                printf("%s", end);

                current = match + strlen(pattern);
                match = strstr(current, pattern);
            }

            // rest
            printf("%s", current);
        }
        line_number++;
    }

    fclose(file);
}

void help() {
    printf("\nAuthor: Igor Mariak\n");
    printf("-----------Available commands-----------\n\n");
    printf("cd /path - change directory | ex. cd /tmp\n");
    printf("cp FILE1 FILE2- copy file | ex. cp one.txt two.txt\n");
    printf("grep PATTERN FILE | ex. grep qui lorem.txt\n");
    printf("color --argument | ex. color --red\n");
    printf("exit - quit the program\n\n");
}


void execute(char *input) {
    char *command = strtok(input, " ");  // first word - command
    char *argument = strtok(NULL, " "); // rest - arguments

    if (command == NULL) {
        printf("\n");
        return;
    }

    if (strcmp(command, "cd") == 0) {
        cd(argument);
    } else if (strcmp(command, "cp") == 0) {
        char *dest_file = strtok(NULL, " ");
        if (argument == NULL || dest_file == NULL) {
        } else {
            cp(argument, dest_file);
        }
    } else if (strcmp(command, "color") == 0) {
        if (argument == NULL) {
        } 
        else {
            color(argument);
            }
    } else if (strcmp(input, "help") == 0) {
        help();

    } else if (strcmp(command, "grep") == 0) {

        char *file = strtok(NULL, " ");
        grep_command(argument, file);

    } else {
        printf("Unknown command. Bash can do it...\n\n");

       // other programs
        char *args[100];
        int i = 0;
        
        // parse
        args[i] = command;
        i++;
        while (argument != NULL) {
            args[i] = argument;
            i++;
            argument = strtok(NULL, " ");
        }
        args[i] = NULL; // last element must be NULL
        
        pid_t id = fork();  // new process

        if (id < 0) {
            printf("Fork error: %d\n", errno);
        } else if (id == 0) {
            // child process
            signal(SIGINT, SIG_DFL); // terminal interrupt signal
            execvp(args[0], args);  
            if (errno == 2) {
                printf("Even I cannot do it :( \n");
            }
            printf("Execvp error: %d\n", errno);
            exit(1);  
        } else {
            signal(SIGINT, SIG_IGN); // signal ignore to not stop the microshell
            wait(NULL);  // wait for the child process 
        }
    }
}

int main() {
    char *input;

    help();


    while(1) {

        command_prompt();

        input = readline(" ");
        
        // delete new line
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        execute(input);

        free(input);

    }
    
    return 0;
}
