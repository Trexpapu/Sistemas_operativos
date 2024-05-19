#include <stdio.h>
#include <stdlib.h>
//Vgr; Si PC = 0, Marco = 0; Si PC = 18, Marco = 1; Si PC = 32, Marco = 2; etc... marco = EL pc normal nLinea / 16 
int CrearArhivoSwap() {
    FILE *file;
    const char *filename = "SWAP.bin";
    size_t num_zeros = 65536; // Número de ceros a escribir
    char zero = 0; // Valor del cero en binario

    // Abrir el archivo en modo binario para escritura
    file = fopen(filename, "wb");
    if (file == NULL) {
        return -205;
    }

    // Escribir ceros en el archivo
    for (size_t i = 0; i < num_zeros; i++) {
        if (fwrite(&zero, sizeof(char), 1, file) != 1) {
            return -206;
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    // Cerrar el archivo
    fclose(file);
    return 201;
}


void ZonaSwap(){
    mvprintw(32, 210, "SWAP");
    refresh();
}

void ZonaTMS(){
    mvprintw(1, 250, "TMS");
    mvprintw(3, 250, "Marco-PID");
    refresh();
}
