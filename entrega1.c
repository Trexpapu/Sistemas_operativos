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
    if (kbhit()) { // Comprobar si se presionó una tecla
        comando[*j] = getch(); // Almacenar la tecla en el comando
        
        if (comando[*j] == '\n') {//verificar si la tecla fue enter
            comando[*j] = '\0';//eliminar enter
            (*y)++; // Incrementar y para pasar a la siguiente línea
            return validar_comandos(comando);
            
            
        }else {
            (*j)++;
        }



        if((*y)>=20){//limpiar la pantalla cuando mi prompt llegue a cierto limite
            for (int i =0; i<=20;i++){
                mvprintw(i, 0, "                   ");
            }
            (*y)=0;

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

int mostrar_mensajes(int x, char *comando, int *j, int *y){
    int error_de_archivo=0;
    switch (x) {

            case 200:


                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                error_de_archivo=verificar_archivo(archivo);//verficamos el archivo
                if (error_de_archivo==800){//si el archivo se puede abrir correctamente
                    mvprintw(40, 10, "                                                                                                               ");
                    mvprintw(40, 10, "Error al abrir el archivo");
                }else{//si no regresa 800
                    mvprintw((*y), 0, "PROMPT > %s", comando);
                    return 205;//se puede leer el archivo
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
    return 0;
}

void mostrar_error_de_archivo(){
     mvprintw(40, 10, "                                                                                                               ");
    mvprintw(40, 10, "Error de sintaxis en el archivo");
    refresh();
}

int main(void) {
    struct PCB *pcb = malloc(sizeof(struct PCB));//reservamos la estructura
     FILE *n_archivo;
    
    
    char comando[60] = {0};//comando o cadena donde se leera lo escrito
    int posicion_de_char = 0;//posicion de un char en la cadena (j)
    int recibe_valores = 0;//recibe el retorno de atiende shell
    int nivel_de_prompt = 0;//en que altura se escribe el prompt (y)
    int bandera = 0;//para saber si el archivo se puede seguir leyendo con normalidad
    int x=0;//recibe el retorno de leer archivo
    
    initscr();
    prints(&nivel_de_prompt, comando);
    prints_procesador();
    
    while (recibe_valores != 10) {
        recibe_valores = atiende_shell(comando, &posicion_de_char, &nivel_de_prompt);
        if(mostrar_mensajes(recibe_valores, comando, &posicion_de_char, &nivel_de_prompt)==205){//si mostrar mensajes retorna 205 abrimos el archivo e inicializamos las variables, ademas ponemos bandera en true
            n_archivo = fopen(archivo, "r");
            pcb->AX=0;
            pcb->BX=0;
            pcb->CX=0;
            pcb->DX=0;
            pcb->PC=0;
            bandera=1;
        }
        if (bandera==1){//si bandera es 1 entonces podemos seguir leyendo el archivo

            if (fgets(pcb->IR, sizeof(pcb->IR), n_archivo) != NULL){//leeemos la linea del archivo
                x=leer_archivo(pcb);//ejecutamos funcion pasando la estructura
                if(x==425){//manejo de errores
                    fclose(n_archivo);
                    bandera=0;
                }else if(x==405){
                    fclose(n_archivo);
                    bandera=0;
                    mostrar_error_de_archivo();
                }
                

            }else{//si se llega al final del archivo lo cerramos y bandera False
                fclose(n_archivo);
                bandera=0;
            }

        }
    }

    endwin();
    librerar_recursos(pcb);//liberamos recursos
    return 0;
}
