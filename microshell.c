#include <stdio.h>
#include <string.h>
#include <unistd.h>  //  Do getcwd(), wyswietla aktualna sciezke
#include <stdlib.h> // Do getenv()
#include <fcntl.h> // otwieranie plikow

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


        if (strncmp(input, "cd", 2) == 0) {
            char *path = input + 3;  // Ścieżka po "cd " (zakładając, że ścieżka zaczyna się po spacji)

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
