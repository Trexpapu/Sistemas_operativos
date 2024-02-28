#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "kbhit.h"
#include "procesador.h"
#include "lista.h"



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

                    }else if(strcmp(accion, "insert") == 0){
                        if (access(archivo, F_OK) != -1){
                           //retornamos 300 es decir si existe el archivo y podemos cargarlo en la lista
                            return 300;
                            
                         }else{
                            //printf("No existe el archivo ingresado\n");
                            return 405;
                         }
                        
                        
                    }else if(strcmp(accion, "pull") == 0){
                        return 732;//PULL no necesita parametros

                    }else if(strcmp(accion, "push") == 0){
                        return 782;//PPUSH NO NECESITA PARAMETROS

                    }else{
                        return 400;//printf("comando no reconocido\n");
                    }

                }else if(sscanf(comando, "%s %s", accion, archivo)==1){
                    if (strcmp(accion, "load") == 0){
                        //printf("Error el comando es: load file\n");
                        return 410;
                    }else if(strcmp(accion, "exit") == 0){
                        return 10;
                    }else if(strcmp(accion, "insert") == 0){
                        return 420;//insert necesita un archivo
                
                    }else if(strcmp(accion, "pull") == 0){
                        return 225;//comando de pull correcto
                        
                    }else if(strcmp(accion, "push") == 0){
                        return 226;//PUSH correcto

                    }else{
                        return 400;// printf("comando no reconocido\n");
                    }
                    
                }else{
                    return 400;// printf("comando no reconocido\n");
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
                mvprintw(i, 0, "                                                                                         ");
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

int mostrar_respuesta_a_comandos(int opcion, char *comando, int *j, int *y){
    int error_de_archivo=0;
    switch (opcion) {

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
                    return 200;//se puede leer el archivo
                }
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 225://podemos ejecutar el pull
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 255;
                break;

            case 226:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 226;
                break;


            case 300: //podemos cargar el archivo en la lista
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 300; //indicando que se puede agregar un nodo al inicio de la lista
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
                mvprintw(40, 10, "Error load necesita como parametro un archivo... Ejemplo: load file.txt");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 420:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Error insert necesita como parametro un archivo");
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

            case 732:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Error pull no necesita parametros...");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;
            case 782:
                mvprintw(40, 10, "                                                                                                               ");
                mvprintw(40, 10, "Error push no necesita parametros...");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

                
        }
        refresh();
    return 0;
}

void mostrar_errores_de_archivo(struct PCB *pcb, int opcion){
    char operacion[60];
    char p1[60];
    char p2[60];
    char error[60];
    sscanf(pcb->IR, "%s %s %s %s", operacion, p1, p2, error);
    switch(opcion){
        case 401:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error %s no es una operacion reconocida", operacion);
            break;
        case 402:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error %s no es un registro reconocido", p1);
            break;
        case 403:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error %s no es un registro o un numero", p2);
            break;
        case 404:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error no es posible dividir entre (%s) porque es 0", p2);
            break;
        case 405:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Se acaba de leer END indicando fin de programa");
            break;
        case 406:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error la instruccion contiene demasiados elementos %s", error);
            break;
        case 501:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error %s solo necesita un registro como parametro ", operacion);
            break;
        case 502:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error %s necesita dos parametros p1: registro, p2: registro o numero", operacion);
            break;
        case 503:
            mvprintw(40, 10, "                                                                                                               ");
            mvprintw(40, 10, "Error %s necesita mas parametros ", operacion);
            break;
    }refresh();

    
}

int leer_lineas(struct PCB *pcb, int *bandera, FILE *n_archivo){
    strcpy(pcb->fileName, archivo);
    int x=0;//recibe el retorno de leer archivo
     if (fgets(pcb->IR, sizeof(pcb->IR), n_archivo) != NULL){//leeemos la linea del archivo
                impresionPCB(pcb);
                x=validar_operaciones_de_archivo(pcb);//ejecutamos funcion pasando la estructura

                if(x!=0){//manejo de errores
                    fclose(n_archivo);
                    (*bandera)=0;
                    mostrar_errores_de_archivo(pcb, x);
                }
            }else{//si se llega al final del archivo lo cerramos y (*bandera) False
                fclose(n_archivo);
                (*bandera)=0;
            }
    return 0;
}


void reiniciar_valores_de_struct(struct PCB *pcb){
            strcpy(pcb->fileName, "                ");
            pcb->AX=0;
            pcb->BX=0;
            pcb->CX=0;
            pcb->DX=0;
            pcb->PC=0;

}

void mostrar_errores_de_ejecucion(int opcion){
    switch (opcion)
    {
    case 1:
        mvprintw(40, 10, "                                                                                                               ");
        mvprintw(40, 10, "Error la lista esta vacia");
        break;
    case 2:
        mvprintw(40, 10, "                                                                                                               ");
        mvprintw(40, 10, "Error no se puede ejecutar pull mientras hay un programa en ejecucion");
        break;
    case 3:
        mvprintw(40, 10, "                                                                                                               ");
        mvprintw(40, 10, "Error no hay ningun programa cargado");
        break;
    }
    
    refresh();            

}
int main(void) {
    struct PCB *cabeza = NULL; // Inicializar la lista como vacía
    //struct PCB *pull_struct = malloc(sizeof(struct PCB));//reservamos la estructura
    struct PCB *pcb = malloc(sizeof(struct PCB));//reservamos la estructura
    FILE *n_archivo;
    
    
    char comando[60] = {0};//comando o cadena donde se leera lo escrito
    int posicion_de_char = 0;//posicion de un char en la cadena (j)
    int recibe_valores = 0;//recibe el retorno de atiende shell
    int nivel_de_prompt = 0;//en que altura se escribe el prompt (y)
    int bandera = 0;//para saber si el archivo se puede seguir leyendo con normalidad
    int ejecucion_a_comandos = 0;
    int programa_cargado = 0;
    int pid=0;
    char filename[256];
    
    
    initscr();
    prints(&nivel_de_prompt, comando);
    prints_procesador();
    
    while (recibe_valores != 10) {

        recibe_valores = atiende_shell(comando, &posicion_de_char, &nivel_de_prompt);
        ejecucion_a_comandos = mostrar_respuesta_a_comandos(recibe_valores, comando, &posicion_de_char, &nivel_de_prompt);

        if(ejecucion_a_comandos == 200){//si mostrar mensajes retorna 205 abrimos el archivo e inicializamos las variables, ademas ponemos bandera en true
            n_archivo = fopen(archivo, "r");
            reiniciar_valores_de_struct(pcb);
            bandera=1;

        }else if(ejecucion_a_comandos == 300){//se ejecuta el insert
            INSERT(&cabeza, archivo);
            imprimirLista(cabeza);

        }else if(ejecucion_a_comandos == 255){//se ejecuta el pull
            if(bandera == 0){

                if (programa_cargado==0){
                    
                    struct PCB* nodo_extraido = pull(&cabeza, &pid, filename);
                    if(nodo_extraido == NULL){
                        mostrar_errores_de_ejecucion(1);
                    }else{
                        imprimirLista(cabeza);
                        impresionPCB(nodo_extraido);
                        programa_cargado = 1;
                    }
                }else {
                  // programa_cargado = 0;
                    // Reinsertar el nodo extraído de vuelta a la lista
                   // REINSERT(&cabeza, filename, pid);////TENGO DUDAS HACER PULL O REINSERT PRIMERO               
                }
                    
            }else{
                mostrar_errores_de_ejecucion(2);
            }
        }else if(ejecucion_a_comandos == 226){//se puede ejecutar el push
            if(programa_cargado == 1){//quiere decir que ya se uso pull una vez anteriormente
                insertar_final(&cabeza, filename, pid);  
                prints_procesador();
                imprimirLista(cabeza);
                programa_cargado = 0;

            }else{
                mostrar_errores_de_ejecucion(3);
            }
        
        
        }


        if (bandera==1){//si bandera es 1 entonces podemos seguir leyendo el archivo
            leer_lineas(pcb, &bandera, n_archivo);
        }
    }

    endwin();
    librerar_recursos(pcb);//liberamos recursos
    return 0;
}