#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int i = 0;
    char comando[30] = {0}; // Inicializa el arreglo con ceros
    int j = 0;

    while (1) {
        if (i > 10) {
            i = 0;
        }

        printf("[%d] ingrese el comando 'outnow' para salir: %s", i, comando);
        printf("\n");

        // Si hay una tecla presionada
        if (kbhit()) {
            // Leer la entrada del usuario, solo un carácter a la vez
            comando[j] = getch();

            // Si se presiona la tecla Enter
            if (comando[j] == '\r') {
                // Elimina el salto de línea del final de la cadena
                comando[j] = '\0';
                if (strcmp(comando, "outnow") == 0) {
                    sleep(2);
                }
                // vaciando la cadena puntero, con que lo va a llenar, tamaño de bytes que se ocupan
                memset(comando, 0, sizeof(comando));
                j = 0;
            }else{
                j ++;
                }
        }
        i++;
    }

    return 0;
}
