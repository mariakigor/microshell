#include <stdio.h>
#include <string.h>
#include <unistd.h>  //  Do getcwd(), wyswietla aktualna sciezke
#include <stdlib.h> // Do getenv()

#define PATH_MAX 4096 
#define MAX_INPUT 1024

void znak_zachety() {
    char buf[PATH_MAX]; // Bufor dla sciezki 

    const char *user = getenv("LOGNAME"); // uzytkownik
    const char *host = getenv("NAME"); //host

    if ((getcwd(buf, sizeof(buf)) != NULL) && (user != NULL) && (host != NULL)) {
        printf("%s@%s:[%s] $ ", user, host, buf); // Funkcja zapisuje ścieżkę do bufora
    } else {
        perror("getcwd error"); // Obsługa błędu
        perror("getenv error");
    }
}

void cd(char *path) {
    if (path == NULL) {
        chdir(getenv("HOME"));
    } else {
        if (chdir(path) == -1) {
            printf("No such file or directory\n");
        } else {
            chdir(path);
        }
        
    }

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


        if (strncmp(input, "cd", 2) == 0) {
            char *path = input + 3;  // Ścieżka po "cd " (zakładając, że ścieżka zaczyna się po spacji)

            if (*path == '\0') {
                // samo cd 
                cd(NULL);
            } else {
                cd(path);
            }
        }

    }
    
    return 0;
}
