#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "kbhit.h"
#include "procesador.h"
#include "lista.h"



char archivo[200];//variable global de archivo
char numero_de_kill[200];
int global_sleep = 500000; //espera para ver cada lectura del archivo
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

                    }else if(strcmp(accion, "kill") == 0){
                        strcpy(numero_de_kill, archivo); // Copiar archivo a numero_de_kill

                        if(atoi(numero_de_kill)){
                            return 237; // si se puede ejecutar el comando kill
                        }else{
                            return 437; //atoi necesita un numero entero
                        }

                    }else{
                        return 400;//printf("comando no reconocido\n");
                    }

                }else if(sscanf(comando, "%s %s", accion, archivo)==1){
                    if (strcmp(accion, "load") == 0){
                        //printf("Error el comando es: load file\n");
                        return 410;
                    }else if(strcmp(accion, "exit") == 0){
                        return 10;
                    }else if(strcmp(accion, "kill") == 0){
                        return 437;//kill necesita un numero entero como parametro

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
            
            
        }else if(comando[*j] == 127){//para el backspace
            if((*j)>0){
                comando[*j] = '\0';
                (*j)--;
                mvprintw(*y, 0, "                                                              ");

            }else if((*j) == 0){
                comando[*j] = '\0';
                mvprintw(*y, 0, "                                                              ");
            }
        }else if(comando[(*j)-2] == 27){//verificar secuencia de flechas
            if(comando[(*j) - 1] == 91){//verificar la secuendia de teclas
                
                if(comando[*j] == 67){//flecha derecha
                    comando[*j] = '\0';
                    (*j)--;
                    comando[*j] = '\0';
                    (*j)--;
                    comando[*j] = '\0';
                    mvprintw(*y, 0, "                                                              ");
                    if (global_sleep < 800000){
                        global_sleep += 100000;
                    }
                }else if(comando[*j] == 68){//flecha izquierda
                    comando[*j] = '\0';
                    (*j)--;
                    comando[*j] = '\0';
                    (*j)--;
                    comando[*j] = '\0';
                    mvprintw(*y, 0, "                                                              ");
                    if (global_sleep > 100000){
                        global_sleep -= 100000;
                    }
                }
            }
        }else{
            (*j)++;
        }
    }//fin kbhit

    if((*y)>=20){//limpiar la pantalla cuando mi prompt llegue a cierto limite
            for (int i =0; i<=25;i++){
                mvprintw(i, 0, "                                                 ");
            }
            (*y)=0;

        }
    // Imprimir el prompt y el comando
    mvprintw(*y, 0, "PROMPT > %s", comando);
    refresh();

    return 0;
}



void prints(int *y, char *comando){
    for (int i = 0; i < 300; i++) {
        mvprintw(30, i, "*");
        mvprintw(i, 80, "*");
    }
    mvprintw(*y, 0, "PROMPT > %s", comando);
    refresh();
}

int mostrar_respuesta_a_comandos(int opcion, char *comando, int *j, int *y){
    int error_de_archivo=0;
    int ejey = 40;
    int ejex = 1;
    switch (opcion) {

            case 200:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                error_de_archivo=verificar_archivo(archivo);//verficamos el archivo
                if (error_de_archivo==800){//si el archivo se puede abrir correctamente
                    mvprintw(ejey, ejex, "                                                                                                               ");
                    mvprintw(ejey, ejex, "Error al abrir el archivo");
                }else{//si no regresa 800
                    mvprintw((*y), 0, "PROMPT > %s", comando);
                    return 200;//se puede leer el archivo
                }
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 237: //se puede ejecutar el kill
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 237;
                break;

                
            case 405:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "No existe el archivo ingresado");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 410:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Error load necesita como parametro un archivo... Ejemplo: load file.txt");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            
            case 437:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Error kill necesita un numero como parametro...");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 400:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Comando invalido");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

        }

        refresh();
    return 0;
}

void mostrar_errores_de_archivo(struct PCB *ejecucion, int opcion){
    char operacion[60];
    char p1[60];
    char p2[60];
    char error[60];
    int ejey = 40;
    int ejex = 1;
    sscanf((ejecucion)->IR, "%s %s %s %s", operacion, p1, p2, error);
    switch(opcion){
        case 99:
            mvprintw(ejey, ejex, "                                                                                                              ");
            mvprintw(ejey, ejex, "EL proceso %d llegó al final del archivo", (ejecucion)->PID);
            break;
        case 401:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> %s no es una operacion reconocida",ejecucion->PID, operacion);
            break;
        case 402:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> %s no es un registro reconocido", ejecucion->PID, p1);
            break;
        case 403:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> %s no es un registro o un número",ejecucion->PID, p2);
            break;
        case 404:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> división entre cero, p2 tiene valor cero. p2 = (%s)", ejecucion->PID, p2);
            break;
        case 405:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Se acaba de leer END en el proceso %d indicando fin de programa", ejecucion->PID);
            break;
        case 406:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> la instrucción contiene demasiados elementos %s", ejecucion->PID, error);
            break;
        case 501:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> la intrucción %s requiere un único registro",ejecucion->PID, operacion);
            break;
        case 502:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> %s requiere un regristro y un número o registro",ejecucion->PID, operacion);
            break;
        case 503:
            mvprintw(ejey, ejex, "                                                                                                               ");
            mvprintw(ejey, ejex, "Error en el proceso %d-> la operación %s necesita más parámetros ",ejecucion->PID, operacion);
            break;
    }refresh();

    
}

void mostrar_mensajes_ejecucion(int opcion){
    int ejey = 40;
    int ejex = 1;
    switch (opcion)
    {
    
    case 1:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "Error no existe un proceso con el PID: %s", numero_de_kill);
        break;
    case 2:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "Se elimino el proceso en ejecucion con el PID: %s ", numero_de_kill);
        break;
    case 3:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "Se elimino el proceso con el PID: %s ", numero_de_kill);
        break;
    

    }
    
    refresh();            

}

int leer_lineas(struct PCB **ejecucion, int *bandera, FILE *n_archivo, int *quantum, int *programa_cargado, struct PCB **listos) {
    int x = 0; // Recibe el retorno de operaciones de archivo
    if (fgets((*ejecucion)->IR, sizeof((*ejecucion)->IR), n_archivo) != NULL) { // Leeemos la línea del archivo
        
        x = validar_operaciones_de_archivo(*ejecucion); // Ejecutamos función pasando la estructura
        (*ejecucion)->PC ++;
        (*quantum)++;
        impresionPCB(*ejecucion);
        if (x != 0) { // Manejo de errores
            fclose(n_archivo);
            (*bandera) = 0;
            (*quantum) = 0;
            mostrar_errores_de_archivo((*ejecucion), x);
        }

    } else { // Si se llega al final del archivo lo cerramos y (*bandera) False
        fclose(n_archivo);
        (*bandera) = 0;
        (*quantum) = 0;
        mostrar_errores_de_archivo((*ejecucion), 99);
    }
    
    if ((*quantum) >= 15) {//si se llego al limite de lineas leidas por archivo reinstarmos al final de la lista listos
        (*bandera) = 0;
        (*programa_cargado) = 0;
        re_insert(listos, *ejecucion);
        kill(ejecucion, (*ejecucion)->PID);
        (*quantum) = 0;
    }
    return 0;
}


void manejar_procesos(struct PCB **listos, struct PCB **terminados, struct PCB **ejecucion, int *bandera, int *programa_cargado, int ejecucion_a_comandos, char archivo_valido[200], int retorno_kill) {
    if (ejecucion_a_comandos == 200) {
        strcpy(archivo_valido, archivo);
        insert(listos, archivo_valido);

    } else if (ejecucion_a_comandos == 237) {//si se ejecuta el kill

        retorno_kill = kill_push(listos, atoi(numero_de_kill), terminados);//buscamos en la lista listos
        if (retorno_kill != 0) {
            retorno_kill = kill_push(ejecucion, atoi(numero_de_kill), terminados);//buscamos en el nodo en ejecucion
            if (retorno_kill != 0) {
                    mostrar_mensajes_ejecucion(1);//si no se encontro en ningun lado
                
            } else {//si se encontro en ejecucion
                *bandera = 0;
                *programa_cargado = 0;
                mostrar_mensajes_ejecucion(2);
            }
        }
        else{//si se encontro en listos
            mostrar_mensajes_ejecucion(3);
        }
    }

    if (*programa_cargado == 0) {
        *ejecucion = pull(listos);
        if (*ejecucion != NULL) {
            impresionPCB(*ejecucion);
            *programa_cargado = 1;
            *bandera = 1;
        }
    }

    if (*programa_cargado == 1) {
        if (*bandera == 0) {
            push(terminados, *ejecucion);
            *programa_cargado = 0;
        }
    }
}


void print_sleep(int contador_programas){
    mvprintw(28, 130, "sleep                  ");
    mvprintw(28, 130, "sleep %d", global_sleep);
    mvprintw(28, 160, "                   ");
    mvprintw(28, 160, "Programas: %d", contador_programas);
    refresh();
}


int main(void) {
    struct PCB *listos = NULL; // Lista de nodos en espera por ser ejecutados
    struct PCB *terminados = NULL; //lista de nodos terminados
    struct PCB *ejecucion = malloc(sizeof(struct PCB));//nodo unico en ejecucion
    int quantum = 0;
    int contador_programas = 0;
    
    
    char comando[60] = {0};//comando o cadena donde se leera lo escrito
    int posicion_de_char = 0;//posicion de un char en la cadena (j)
    int recibe_valores = 0;//recibe el retorno de atiende shell
    int nivel_de_prompt = 0;//en que altura se escribe el prompt (y)
    int bandera = 0;//para saber si el archivo se puede seguir leyendo con normalidad
    int ejecucion_a_comandos = 0;//variable que recibe valores del retorno mostrar_respuesta_a_comandos
    int programa_cargado = 0;//variable de control para realizar pull y push
    int retorno_kill = 0;//este sirve para  recibir el valor retornado por kill
    char archivo_valido[200];//variable que recibe valores de la variable global archivo

    
    
    initscr();
    prints(&nivel_de_prompt, comando);
    prints_procesador();
    prints_titulos();
    
    while (recibe_valores != 10) {

        recibe_valores = atiende_shell(comando, &posicion_de_char, &nivel_de_prompt);
        ejecucion_a_comandos = mostrar_respuesta_a_comandos(recibe_valores, comando, &posicion_de_char, &nivel_de_prompt);

        imprimir_ejecion(ejecucion, 140, 3);
        imprimir_listos(listos, 82, 35);
        imprimir_terminados(terminados, 124, 35);
        print_sleep(contador_programas);

        manejar_procesos(&listos, &terminados, &ejecucion, &bandera, &programa_cargado, ejecucion_a_comandos, archivo_valido, retorno_kill);

        if (bandera==1){//si bandera es 1 entonces podemos seguir leyendo el archivo
            usleep(global_sleep); //espera para ver cada lectura del archivo
            leer_lineas(&ejecucion, &bandera, ejecucion->programa, &quantum, &programa_cargado, &listos);        
            }

        contador_de_programas(&listos, &ejecucion, &contador_programas);
        }



    endwin();
    printf("Antes de liberar recursos: ejecucion=%p, listos=%p, terminados=%p\n", (void*)ejecucion, (void*)listos, (void*)terminados);
    librerar_recursos(ejecucion);
    liberar_listos(&listos);
    liberar_lista_terminados(&terminados);
    printf("Después de liberar recursos: ejecucion=%p, listos=%p, terminados=%p\n", (void*)ejecucion, (void*)listos, (void*)terminados);

    return 0;
}