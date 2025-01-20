#include <stdio.h>
#include <string.h>
#include <unistd.h>  //  Do getcwd(), wyswietla aktualna sciezke
#include <stdlib.h> // Do getenv()
#include <fcntl.h> // otwieranie plikow
#include <ctype.h> // do isspace()
#include <readline/readline.h>  //bajery do historii i przegladania
#include <readline/history.h>
#include <errno.h>

#define PATH_MAX 4096 
#define MAX_INPUT 1024

char current_color[20] = "\033[0m";

void znak_zachety() {
    char buf[PATH_MAX]; // Bufor dla sciezki 

    const char *user = getenv("LOGNAME"); // uzytkownik
    const char *host = getenv("NAME"); //host

    if ((getcwd(buf, sizeof(buf)) != NULL) && (user != NULL) && (host != NULL)) {
        printf("%s%s@%s:[%s] %s$ %s", current_color, user, host, buf, current_color, "\033[0m"); // Funkcja zapisuje ścieżkę do bufora
    } else {
        perror("getcwd error"); // Obsługa błędu
        perror("getenv error");
    }
}

void color(const char *color) {
    if (strcmp(color, "-green") == 0) {
        strcpy(current_color, "\033[0;32m");  
    } else if (strcmp(color, "-red") == 0) {
        strcpy(current_color, "\033[0;31m"); 
    } else if (strcmp(color, "-blue") == 0) {
        strcpy(current_color, "\033[0;34m"); 
    } else if (strcmp(color, "-yellow") == 0) {
        strcpy(current_color, "\033[0;33m");  
     } else if (strcmp(color, "-magenta") == 0) {
        strcpy(current_color, "\033[0;35m");  
    } else if (strcmp(color, "-cyan") == 0) {
        strcpy(current_color, "\033[0;36m");  
    } else if (strcmp(color, "reset") == 0) {
        strcpy(current_color, "\033[0m");  
    } else {
        printf("Unknown color, for reset type color reset. Available: -green, -red, -blue, -yellow, -magenta, -cyan\n");
    }
}


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
        } else {
            chdir(path);
        }
        
    }

}

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

    // plik zrodlowy
    file = open(source_file, O_RDONLY);
    if (file == -1) {
        printf("Cannot open source file\n");
        close(file);
        return;
    }

    // plik docelowy
    new_file = open(dest_file, O_WRONLY | O_CREAT, 0644);  // O_CREAT wymaga uprawnien 0 specjalne wlasciciel (6 - rw_), grupa (4 - r__), inni (4-r__)
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

void execute(char *input) {
    char *command = strtok(input, " ");  // pierwsze slowo komenda
    char *argument = strtok(NULL, " "); // reszta - argumenty

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
        } else {
            color(argument);
        }
    } else {
        printf("Unknown command. Bash will know.\n");
    }
}

int main() {
    char *input;

    read_history("history.txt"); //wczytanie historii

    while(1) {

        znak_zachety();

        // odczyt polecenia
        input = readline("");

        

        // usuwanie nowej lini
        input[strcspn(input, "\n")] = '\0';

        // Gdy exit - koniec programu
        if (strcmp(input, "exit") == 0) {
            break;
        }

        execute(input);

        // dodawanie historii
        add_history(input);
        write_history("history.txt");

        free(input);

    }
    
    return 0;
}
