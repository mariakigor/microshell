#include <stdio.h>
#include <string.h>
#include <unistd.h>  //  Do getcwd(), wyswietla aktualna sciezke
#include <stdlib.h> // Do getenv()
#include <fcntl.h> // otwieranie plikow
#include <ctype.h> // do isspace()

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

void change_color(const char *color) {
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
            perror("cd");
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

int main() {
    char input[MAX_INPUT];

    while(1) {

        znak_zachety();

        // Odczytanie polecenia od użytkownika
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;  
        }

        // Usuwanie nowej linii, fgets dodaje nowa linie na koncu
        input[strcspn(input, "\n")] = '\0';

        // Gdy exit - koniec programu
        if (strcmp(input, "exit") == 0) {
            break;
        }

        if (strncmp(input, "color", 5) == 0) {
            char *color = input + 6;
            change_color(color);  // Zmiana koloru
        }


        if (strncmp(input, "cd", 2) == 0) {
            char *path = (input + 3);  
            printf("%s\n", path);

            if (*path == '\0') {
                // samo cd 
                cd(NULL);
            } else {
                cd(path);
            }
        }
        else if (strncmp(input, "cp", 2) == 0) {
            char *source = strtok(input + 3, " "); // strtok dzieli na tokeny - mniejsze czesci stringa
            char *destination = strtok(NULL, " ");

            cp(source, destination);
        }

    }
    
    return 0;
}
