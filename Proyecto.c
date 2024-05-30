#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "kbhit.h"
#include "procesador.h"
#include "lista.h"
#include "CrearSwap.h"
//SECCION DE VARIABLES GLOBALES

char archivo[200];//variable global de archivo
char numero_de_kill[200];
int global_sleep = 500000; //espera para ver cada lectura del archivo
#define maxQUANTUM 5 //cantidad máxima de lectura de lineas
int incCPU = 60 / maxQUANTUM; // Quantum por proceso (60/MAXQUANTUM).
int PBase = 60; //Prioridad base para todos los procesos (60).
int NumUs = 0; //Cantidad de usuarios para los cuales planificar (0).
float W = 0.0; //Posteriormente (1/NumUsuarios)
char UserID[40]; // se usa para la conversion atoi
int copia_userID = 0; //se guarda la copia si el atoi es exitoso
int contador_usuarios = 0;
char UltimoComando[264];




int validar_comandos(char *comando){
    char accion[100];
    //verificar que esta bien el comando LOAD FILE
                if (sscanf(comando, "%s %s %s", accion, archivo, UserID)==3){
                    //comprobar si la accion es load
                    if (strcmp(accion, "load") == 0){
                        //verificar si existe el archivo
                         if (access(archivo, F_OK) != -1){
                            if(atoi(UserID)){
                                if (verificar_archivo(archivo)==800){//si el archivo se puede abrir correctamente
                                    return 800;
                                }else{//si no regresa 800
                                     //return 200 para decir que si existe el archivo y el UserId es un numero valido
                                    copia_userID = atoi(UserID);
                                    return 200;
                                }
                               
                            }else{
                                //para decir que userID no es un numero entero valido
                                return 777;
                            }    
                         }else{
                            //printf("No existe el archivo ingresado\n");
                            return 405;
                         }

                    }else if(strcmp(accion, "kill") == 0){
                        return 437; //kill necesita solo un numero entero como parámetro

                    }else{
                        return 400;//printf("comando no reconocido\n");
                    }

                }else if(sscanf(comando, "%s %s %s", accion, archivo, UserID)==2){
                    if (strcmp(accion, "load") == 0){
                        //faltan parámetros en load
                        return 410;
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
                }else if(sscanf(comando, "%s %s %s", accion, archivo, UserID)==1){
                    if (strcmp(accion, "load") == 0){
                        //faltan parámetros en load
                        return 410;
                    }else if(strcmp(accion, "exit") == 0){
                        return 10;
                    }else if(strcmp(accion, "kill") == 0){
                        return 437;//kill necesita un numero entero como parametro

                    }else if(strcmp(accion, "sig") == 0){
                        return 188;//cambiar la paginacion de swap

                    }else if(strcmp(accion, "ant") == 0){
                        return 189;//cambiar la paginacion de swap

                    }else if(strcmp(accion, "tmsd") == 0){
                        return 663;//cambiar la paginacion de swap

                    }else if(strcmp(accion, "tmsa") == 0){
                        return 664;//cambiar la paginacion de swap

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
            strcpy(UltimoComando, comando);
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
                    if (global_sleep < 2400000){
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
                }else if(comando[*j] == 65){

                    comando[*j] = '\0';
                    (*j)--;
                    comando[*j] = '\0';
                    (*j)--;
                    comando[*j] = '\0';
                    mvprintw(*y, 0, "                                                              ");
                    strcpy(comando, UltimoComando);
                    (*j) = strlen(UltimoComando);
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
    for (int i = 0; i < 400; i++) {
        mvprintw(30, i, "*");
        mvprintw(i, 80, "*");
    }
    mvprintw(*y, 0, "PROMPT > %s", comando);
    refresh();
}

int mostrar_respuesta_a_comandos(int opcion, char *comando, int *j, int *y){
    int ejey = 40;
    int ejex = 1;
    switch (opcion) {

            case 200:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0; 
                return 200;
                break;

            case 188:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Paginacón hacia delante SWAP");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 188;
                break;

            case 189:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Paginacón hacia atras SWAP");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 189;
                break;


            case 237: //se puede ejecutar el kill
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Comando ejecutado exitosamente");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 237;
                break;

            case 663:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Paginación hacia delante TMS");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 663;
                break;

            case 664:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Paginación hacia atras TMS");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                return 664;
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
                mvprintw(ejey, ejex, "Error load necesita como parametros archivo y ID. Ejemplo: load file.txt ID");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            
            case 437:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Error kill solo necesita un número como parámetro...");
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
            case 777:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "El UserID ingresado no es valido, necesita ser un número entero");
                memset(comando, 0, (*j));
                (*j) = 0;
                mvprintw((*y), 0, "PROMPT > %s", comando);
                break;

            case 800:
                mvprintw(ejey, ejex, "                                                                                                               ");
                mvprintw(ejey, ejex, "Error al abrir el archivo");
                memset(comando, 0, (*j));
                (*j) = 0; 
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

void MensajesAlCrearSwap(int opcion){
    int ejey = 40;
    int ejex = 1;
    switch (opcion){
        case 0:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "SWAP abierto");
        break;
        case 201:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "El archivo SWAP se creo con exito!");
        break;
        case -205:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "Hubo un error al abrir o crear el archivo SWAP");
        break;
        case -206:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "Hubo un error al escribir en el archivo SWAP");
        break;

    }
    refresh();
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
        mvprintw(ejey, ejex, "Se eliminó el proceso en ejecucion-> PID: %s ", numero_de_kill);
        break;
    case 3:
        mvprintw(ejey, ejex, "                                                                                                               ");
        mvprintw(ejey, ejex, "Se eliminó el proceso de listos con el PID: %s ", numero_de_kill);
        break;
    

    }
    
    refresh();            

}

void Actualizar_W(){
    if(contador_usuarios != 0){ ////////////////actualizar w al crear o terminar proceso
            W = 1.0 / (float)contador_usuarios;
        }
    refresh();
}



int leer_swap(struct PCB **ejecucion, int *bandera, FILE *n_archivo, int *quantum, int *programa_cargado, struct PCB **listos) {

    //vemos el limite de pc de cada archivo
    char NombrePrograma[256];
    strcpy(NombrePrograma, (*ejecucion)->fileName);
    FILE *programa = fopen(NombrePrograma, "r");


    char linea[256];
    int Limite_pc = 0;

    // Leer cada línea del archivo del programa
    while (fgets(linea, sizeof(linea), programa) != NULL){
        Limite_pc++;
    }
    fclose(programa);


    //leer del swap y sacar el dato
    int PC = (*ejecucion)->PC;
    int marco = PC / 16;
    int offset = PC % 16;
    int marco_swap = (*ejecucion)->TMP[marco];
    int tamano_marco = 16;
    int desplazamiento_real = (marco_swap * tamano_marco + offset) * 32;
    int DRS = (marco_swap << 4) | offset;
    //mover el puntero
    fseek(file, desplazamiento_real, SEEK_SET);

    // Leer la instrucción del archivo
    





    int x = 0; // Recibe el retorno de operaciones de archivo
    if ((*ejecucion)->PC < Limite_pc) { // verificamos que todavia esta dentro de los limites del archivo
    fread((*ejecucion)->IR, sizeof((*ejecucion)->IR), 1, file); //leemos en el archivo binario 
        
        x = validar_operaciones_de_archivo(*ejecucion); // Ejecutamos función pasando la estructura
        //Si hay algún proceso en ejecución y aún no termina su quantum
        //Actualiza los contadores de uso del CPU para el proceso en ejecución: KCPU = KCPU + IncCPU
        //Actualiza los contadores de uso del CPU para todos los procesos (no Terminados) del usuario dueño del proceso en ejecución: KCPUxU = KCPUxU + IncCPU
        (*ejecucion)->KCPU += incCPU;
        (*ejecucion)->KCPUxU += incCPU;
        actualizarKCPUxU(listos, (*ejecucion)->UID, incCPU);
        (*ejecucion)->PC ++;
        (*quantum)++;
        impresionPCB(*ejecucion, DRS);
        if (x != 0) { // Cuando el archivo tiene un error lo tronamos con su respectivo error
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
    
    if ((*quantum) >= maxQUANTUM) {//si se llego al limite de lineas leidas por archivo reinstarmos al final de la lista listos
        (*bandera) = 0;
        (*programa_cargado) = 0;
        re_insert(listos, *ejecucion);
        (*quantum) = 0;
        

    }
    return 0;
}


void manejar_procesos(struct PCB **listos, struct PCB **terminados, struct PCB **ejecucion,struct PCB **nuevos, int *bandera, int *programa_cargado, int ejecucion_a_comandos, char archivo_valido[200], int retorno_kill) {
    if (ejecucion_a_comandos == 200) {
        strcpy(archivo_valido, archivo);
        insert(listos, archivo_valido, PBase, copia_userID, nuevos, terminados, ejecucion);
        

    } else if (ejecucion_a_comandos == 237) {//si se ejecuta el kill

        retorno_kill = kill_push(listos, atoi(numero_de_kill), terminados, ejecucion, 0);//buscamos en la lista listos
        if (retorno_kill != 0) {
            retorno_kill = kill_push(listos, atoi(numero_de_kill), terminados, ejecucion, 1);//buscamos en el nodo en ejecucion
            if (retorno_kill != 0) {
                    mostrar_mensajes_ejecucion(1);//si no se encontro en ningun lado
                
            }else {//si se encontro en ejecucion
                *bandera = 0;
                *programa_cargado = 0;
                mostrar_mensajes_ejecucion(2);
                
               
            }
        }else{//si se encontro en listos
            mostrar_mensajes_ejecucion(3);
            
        }
    }

    if (*programa_cargado == 0) {
         //Actualiza los parámetros de planificación, para todos los nodos de la Listos:
        Actualizar_W();
        Actualizar_planificacion(listos, PBase, W);
        //limpiando ejecucion
        mvprintw(3, 140, "                                                                 -");
        refresh();
        *ejecucion = pull(listos);//pull es para extraccion listos a ejecucion
        if (*ejecucion != NULL) {
            *programa_cargado = 1;
            *bandera = 1;
        }
         
    }

    if (*programa_cargado == 1) {
        if (*bandera == 0) {
            push(terminados, *ejecucion, listos);//push es para extraer de ejecucion a terminados
            *programa_cargado = 0;
            //chequeo para ver si nuevos se puede meter a listos
            MeterNuevos_Listos(nuevos, listos, ejecucion);
            
        }
    }
}


void printData(int contador_usuarios, int limite, int limiteTMS){
    mvprintw(28, 150, "sleep                  ");
    mvprintw(28, 150, "sleep %d", global_sleep);
    mvprintw(28, 170, "                 ");
    mvprintw(28, 170, "Usuarios %d", contador_usuarios);
    mvprintw(28, 200, "                   ");
    mvprintw(28, 200, "LimiteSwap: %d", limite);
    mvprintw(28, 220, "                   ");
    mvprintw(28, 220, "LimiteTMS: %d", limiteTMS);

    refresh();
}

int abrirSwap(){
    // Abrir el archivo en modo binario para escritura
    file = fopen(filename, "r+b");
    if (file == NULL) {
        return -205;
    }
    return 0;
}

void cerrarSwap() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
        printf("Swap cerrado\n");
    }
}

int ImprimirDatosSwap(int opcion, int *referencia) {
    // Verificar que el archivo esté abierto
    if (file == NULL) {
        return 404;
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET); // Volver al inicio del archivo

    // Reservar memoria para el buffer
    char *buffer = (char *)malloc(file_size);
    if (buffer == NULL) {
        return 405;
    }

    // Leer el contenido del archivo en el buffer
    if (fread(buffer, sizeof(char), file_size, file) != file_size) {
        free(buffer);
        return 406;
    }

    // Manejar la opción de avance y retroceso
    if (opcion == 188 && (*referencia) < TOTAL_MARCOS) {
        (*referencia) += 96;
    } else if (opcion == 189 && (*referencia) >= 96) {
        (*referencia) -= 96;
    }

    // Imprimir el contenido del buffer en el formato específico
    int x = 210; // el eje x en la pantalla
    int encabezado = 34; // de donde empieza a imprimir en la pantalla
    int j = 0; // salto de línea para imprimir las filas
    int final = (*referencia) + 96; // 96 para mostrar 6 marcos
    int row = encabezado; // fila inicial
    int col = x; // columna inicial

    // Imprimir en el formato especificado
    for (long i = (*referencia); i < final; i += 16) {
        for (int offset = 0; offset < 16; offset++) {
            if (i + offset < file_size) {
                mvprintw(row + offset, col, "[%04lX] %-16s", i + offset, &buffer[(i + offset) * 32]);
            } else {
                mvprintw(row + offset, col, "[%04lX]", i + offset);
            }
        }
        col += 20; // mover a la siguiente columna
        if (col > 500) { // ajustar según sea necesario para no salir de la pantalla
            col = x;
            row += 18; // ajustar según sea necesario para las filas
        }
    }

    // Limpiar y liberar memoria
    refresh();
    free(buffer);
    return 0; // todo salió bien
}

void ImprimirDatosTMS(int opcion, int *referencia){
    if(opcion == 663 && (*referencia) < TOTAL_MARCOS){ //tms hacia delante
        (*referencia) += 16;
    }else if(opcion == 664 && (*referencia) >=16){
        (*referencia) -= 16;
    }

    int x = 250; // el eje x en la pantalla
    int encabezado = 5; // de donde empieza a imprimir en la pantalla
    int j = 0;//salto de linea para imprimir las filas
    int final = (*referencia) + 16; //96 para mostrar 6 marcos
    for (long i = (*referencia); i < final; i++){
        mvprintw(encabezado+j, x, "             ");
        mvprintw(encabezado+j, x, "[%03X]-%d", i, Mapa[i]);
        j++;

    }
    refresh();

}



void InicializarMapa(){
    for (int i = 0; i < TOTAL_MARCOS; i++) {
    Mapa[i] = 0;
}
}

int main(void) {
    struct PCB *listos = NULL; // Lista de nodos en espera por ser ejecutados
    struct PCB *terminados = NULL; //lista de nodos terminados
    struct PCB *nuevos = NULL; // lista de nodos nuevos
    struct PCB *ejecucion = malloc(sizeof(struct PCB));//nodo unico en ejecucion
    int quantum = 0;
    
    
    
    char comando[60] = {0};//comando o cadena donde se leera lo escrito
    int posicion_de_char = 0;//posicion de un char en la cadena (j)
    int recibe_valores = 0;//recibe el retorno de atiende shell
    int nivel_de_prompt = 0;//en que altura se escribe el prompt (y)
    int bandera = 0;//para saber si el archivo se puede seguir leyendo con normalidad
    int ejecucion_a_comandos = 0;//variable que recibe valores del retorno mostrar_respuesta_a_comandos
    int programa_cargado = 0;//variable de control para realizar pull y push
    int retorno_kill = 0;//este sirve para  recibir el valor retornado por kill
    char archivo_valido[200];//variable que recibe valores de la variable global archivo
    int UltimoPuntoEnSwap = 0;
    int UltimoPuntoEnTMS = 0;

    
    
    initscr();
    prints(&nivel_de_prompt, comando);
    prints_procesador();
    prints_titulos();
    ZonaSwap();
    MensajesAlCrearSwap(CrearArhivoSwap());
    MensajesAlCrearSwap(abrirSwap()); 
    ZonaTMS();
    InicializarMapa();

    
    while (recibe_valores != 10) {
        

        recibe_valores = atiende_shell(comando, &posicion_de_char, &nivel_de_prompt);
        ejecucion_a_comandos = mostrar_respuesta_a_comandos(recibe_valores, comando, &posicion_de_char, &nivel_de_prompt);
        ImprimirDatosSwap(ejecucion_a_comandos, &UltimoPuntoEnSwap);
        ImprimirDatosTMS(ejecucion_a_comandos, &UltimoPuntoEnTMS);
        imprimir_ejecion(ejecucion, 140, 3);
        imprimir_listos(listos, 82, 35);
        imprimir_terminados(terminados, 140, 35);
        imprimir_nuevos(nuevos, 1, 56);
        printData(contador_usuarios, UltimoPuntoEnSwap, UltimoPuntoEnTMS);

        manejar_procesos(&listos, &terminados, &ejecucion, &nuevos,  &bandera, &programa_cargado, ejecucion_a_comandos, archivo_valido, retorno_kill);
        if (bandera==1){//si bandera es 1 entonces podemos seguir leyendo el archivo
            usleep(global_sleep); //espera para ver cada lectura del archivo
            leer_swap(&ejecucion, &bandera, ejecucion->programa, &quantum, &programa_cargado, &listos);        
            }
        

        contador_de_usuarios(&listos, &ejecucion, &contador_usuarios);
        if (contador_usuarios == 0){
            W = 0;
        }
        Actualizar_W();

        
        
        }

    



    endwin();
    printf("Antes de liberar recursos: ejecucion=%p, listos=%p, terminados=%p, listos=%p\n", (void*)ejecucion, (void*)listos, (void*)terminados, (void*)nuevos);
    librerar_recursos(ejecucion);
    liberar_listos(&listos);
    liberar_lista_terminados(&terminados);
    liberar_nuevos(&nuevos);
    printf("Después de liberar recursos: ejecucion=%p, listos=%p, terminados=%p, listos=%p\n", (void*)ejecucion, (void*)listos, (void*)terminados, (void*)nuevos);
    cerrarSwap();

    return 0;
}
