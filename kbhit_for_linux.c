#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>

// Function to check if a key has been pressed
bool kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

// Function to read a single character from the terminal
char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror ("tcsetattr ~ICANON");
    return (buf);
}

int main() {
    int i = 0;
    char comando[30] = {0}; // Initialize the array with zeros
    int j = 0;

    while (1) {
        if (i > 10) {
            i = 0;
        }

        printf("[%d] ingrese el comando 'outnow' para salir: %s", i, comando);
        printf("\n");

        // If a key is pressed
        if (kbhit()) {
            // Read user input, only one character at a time
            comando[j] = getch();

            // If the Enter key is pressed
            if (comando[j] == '\n') {
                // Remove the newline character from the end of the string
                comando[j] = '\0';
                if (strcmp(comando, "outnow") == 0) {
                    sleep(2);
                    
                }
                // Clearing the string
                memset(comando, 0, sizeof(comando));
                j = 0;
            } else {
                j++;
            }
        }
        i++;
    }

    return 0;
}

