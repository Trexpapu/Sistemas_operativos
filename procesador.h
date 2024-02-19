#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
struct PCB
{
    char IR[100];
    int AX;
    int BX;
    int CX;
    int DX;
    int PC;
};


int leer_archivo(char archivo[100]){
    struct PCB pcb;
    pcb.PC=0;
    FILE *n_archivo;
    n_archivo = fopen(archivo, "r");
    // Lee y muestra cada línea del archivo
    while (fgets(pcb.IR, sizeof(pcb.IR), n_archivo) != NULL) {
        mvprintw(25, 100, "IR                                                                       ");
        mvprintw(25, 100, "IR: %s", pcb.IR);
        pcb.PC++;
        mvprintw(20, 100, "PC:         ");
        mvprintw(20, 100, "PC: %d", pcb.PC);
        usleep(500000);
        refresh();
    }

    // Cierra el archivo
    fclose(n_archivo);
    return 0;

}

int verificar_archivo(char archivo[100]){
    FILE *n_archivo;
    char linea[100];
    n_archivo = fopen(archivo, "r");
    // Verifica si el archivo se abrió correctamente
    if (n_archivo == NULL) {
        return 800; //no se pudo abrir correctamente
    }
    fclose(n_archivo);

    return 0;
}

void prints_procesador(){
    struct PCB pcb = {"", 0, 0, 0, 0, 0};
    mvprintw(0, 100, "AX: %d", pcb.AX);
    mvprintw(5, 100, "BX: %d", pcb.BX);
    mvprintw(10, 100, "CX: %d", pcb.CX);
    mvprintw(15, 100, "DX: %d", pcb.DX);
    mvprintw(20, 100, "PC: %d", pcb.PC);
    mvprintw(25, 100, "IR:");
    mvprintw(0,0,"");
    refresh();
}