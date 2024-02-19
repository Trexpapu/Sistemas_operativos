#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "kbhit.h"
#include "procesador.h"

char archivo[200];//variable global de archivo


int validar_comandos(char *comando){
    char accion[100];
    //verificar que esta bien el comando LOAD FILE
                if (sscanf(comando, "%s %s", accion, archivo)==2){
                    //comprobar si la accion es load
                    if (strcmp(accion, "load") == 0){
                        //verificar si existe el archivo
                         if (access(archivo, F_OK) != -1){
                           //retornamos 200 es decir si existe el archivo
                            return 200;
                            
                         }else{
                            //printf("No existe el archivo ingresado\n");
                            return 405;
                         }

                    }else{
                        //printf("comando no reconocido\n");
                        return 400;
                    }
                }else if(sscanf(comando, "%s %s", accion, archivo)==1){
                    if (strcmp(accion, "load") == 0){
                        //printf("Error el comando es: load file\n");
                        return 410;
                    }else if(strcmp(accion, "exit") == 0){
                        return 10;
                    }else{
                        // printf("comando no reconocido\n");
                        return 400;
                    }
                    
                }

                return 0;
    
}
int atiende_shell(char *comando, int *j, int *y) {
    int ch = getch(); // Leer la entrada del usuario

    if (ch != ERR) { // Comprobar si se presionó una tecla
        comando[*j] = ch; // Almacenar la tecla en el comando
        
        if (ch == '\n') {//verificar si la tecla fue enter
            comando[*j] = '\0';//eliminar enter
            (*y)++; // Incrementar y para pasar a la siguiente línea
            return validar_comandos(comando);
            
            
        } else {
            (*j)++;
        }
        
        // Imprimir el prompt y el comando
        mvprintw(*y, 0, "PROMPT > %s", comando);
    }
    refresh();

    return 0;
}

void prints(int *y, char *comando){
    for (int i = 0; i < 300; i++) {
        mvprintw(30, i, "*");
        mvprintw(i, 90, "*");
    }
    mvprintw(*y, 0, "PROMPT > %s", comando);
    refresh();
}

void mostrar_mensajes(int x, char *comando, int *j, int *y){
    int error_de_archivo=0;
    switch (x) {
            case 200:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                error_de_archivo=verificar_archivo(archivo);
                if (error_de_archivo==800){
                    mvprintw(40, 10, "                                                                                                               ");
                    mvprintw(40, 10, "Error al abrir el archivo");
                }else{
                    leer_archivo(archivo);
                }
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;
                
            case 405:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "No existe el archivo ingresado");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 410:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Error uso correcto del comando, ejemplo load file.txt");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 400:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Comando invalido");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;
                
        }
        refresh();

}

int main(void) {
    char comando[60] = {0};
    int posicion_de_char = 0;
    int recibe_valores = 0;
    int nivel_de_prompt = 0;
    initscr();
    prints(&nivel_de_prompt, comando);
    prints_procesador();
    while (recibe_valores != 10) {
        recibe_valores = atiende_shell(comando, &posicion_de_char, &nivel_de_prompt);
        mostrar_mensajes(recibe_valores, comando, &posicion_de_char, &nivel_de_prompt);
    }

    endwin();

    return 0;
}
