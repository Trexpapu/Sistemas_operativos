#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int P_count = 1;
#define TOTAL_MARCOS  4096 // 65536 instrucciones / 16 instrucciones por marco

int Mapa[TOTAL_MARCOS]; // tms
int tamano = sizeof(Mapa) / sizeof(Mapa[0]);

FILE *file;
const char *filename = "SWAP.bin";


void QuitarPidDelTMS(int pid){
    for (int i = 0; i < tamano; i++){
        if (Mapa[i] == pid){
            Mapa[i] = 0;
        }
    }
}




int kill_push(struct PCB **cabeza, int pid, struct PCB **terminados){


    if (*cabeza == NULL){ // Verifica si la lista donde buscamos está vacía
        return -1;
    }
    QuitarPidDelTMS(pid);

    // Verifica si el PID está en el primer nodo
    if ((*cabeza)->PID == pid){

        // Crear un nuevo nodo de PCB
        struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
        if(nuevoNodo == NULL){
            mvprintw(40, 5, "Error no se pudo reservar más memoria...");
            refresh();
            return 3;
        }

        // Copia el primer nodo en el nuevo nodo
        *nuevoNodo = **cabeza;
        nuevoNodo->sig = NULL;

        // Si la lista de terminados está vacía, el nuevo nodo se convierte en la cabeza
        if (*terminados == NULL) {
            *terminados = nuevoNodo;
        } else {
            // Busca el último nodo de la lista
            struct PCB *ultimo = *terminados;
            while (ultimo->sig != NULL) {
                ultimo = ultimo->sig;
            }

            // Enlaza el nuevo nodo después del último nodo
            ultimo->sig = nuevoNodo;
        }

        // Elimina el primer nodo de la lista
        struct PCB *temp = *cabeza;
        *cabeza = (*cabeza)->sig;
        fclose(temp->programa);
        free(temp);

        return 0;
    }

    // Busca el nodo con el PID dado
    struct PCB *actual = *cabeza;
    struct PCB *anterior = NULL;

    while (actual != NULL && actual->PID != pid) {
        anterior = actual;
        actual = actual->sig;
    }

    // Si se encontró el nodo con el PID dado
    if (actual != NULL) {
        anterior->sig = actual->sig; // Elimina el nodo ajustando los enlaces

        // Crear un nuevo nodo de PCB
        struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
        if(nuevoNodo == NULL){
            mvprintw(40, 5, "Error no se pudo reservar más memoria...");
            refresh();
            return 3;
        }

        // Copia el nodo actual en el nuevo nodo
        *nuevoNodo = *actual;
        nuevoNodo->sig = NULL;

        // Si la lista de terminados está vacía, el nuevo nodo se convierte en la cabeza
        if (*terminados == NULL) {
            *terminados = nuevoNodo;
        } else {
            // Busca el último nodo de la lista
            struct PCB *ultimo = *terminados;
            while (ultimo->sig != NULL) {
                ultimo = ultimo->sig;
            }

            // Enlaza el nuevo nodo después del último nodo
            ultimo->sig = nuevoNodo;
        }

        fclose(actual->programa);
        free(actual);

    } else {
        return -1; // No se encontró el PID en la lista
    }
    return 0;
}



struct PCB* pull(struct PCB **cabeza) {
    // Verifica si la lista está vacía
    if (*cabeza == NULL) {
        return NULL; // Devuelve NULL si la lista está vacía
    }

    struct PCB* Nodo_minimo = *cabeza; // Guarda referencia al nodo con la menor prioridad
    struct PCB* previo = NULL; // Guarda referencia al nodo anterior al nodo mínimo
    struct PCB* actual = *cabeza; // Guarda referencia al nodo actual durante el recorrido

    // Encuentra el nodo con la menor prioridad
    int Prioridad_minima = Nodo_minimo->P;
    while (actual->sig != NULL){
        if(actual->sig->P < Prioridad_minima){
            Prioridad_minima = actual->sig->P;
            Nodo_minimo = actual->sig; // Actualiza el nodo con la menor prioridad
            previo = actual; // Actualiza el nodo anterior al nodo mínimo
        }
        actual = actual->sig;
    }

    // Si el nodo mínimo está en la cabeza de la lista, actualiza la cabeza
    if (previo == NULL) {
        *cabeza = Nodo_minimo->sig;
    } else {
        previo->sig = Nodo_minimo->sig;
    }

    // Devuelve el nodo extraído
    return Nodo_minimo;
}



void liberar_listos(struct PCB **cabeza) {
    struct PCB *actual = *cabeza;
    struct PCB *temp;

    while (actual != NULL) {
        temp = actual; // Guarda una referencia al nodo actual
        actual = actual->sig; // Avanza al siguiente nodo
        fclose(temp->programa); // Cierra el archivo asociado al nodo actual
        free(temp); // Libera la memoria del nodo actual
    }

    // Establece la cabeza de la lista como NULL
    *cabeza = NULL;
}


void liberar_nuevos(struct PCB **cabeza) {
    struct PCB *actual = *cabeza;
    struct PCB *temp;

    while (actual != NULL) {
        temp = actual; // Guarda una referencia al nodo actual
        actual = actual->sig; // Avanza al siguiente nodo
        fclose(temp->programa); // Cierra el archivo asociado al nodo actual
        free(temp); // Libera la memoria del nodo actual
    }

    // Establece la cabeza de la lista como NULL
    *cabeza = NULL;
}

void liberar_lista_terminados(struct PCB **cabeza) {
    struct PCB *actual = *cabeza;
    struct PCB *temp;

    while (actual != NULL) {
        temp = actual; // Guarda una referencia al nodo actual
        actual = actual->sig; // Avanza al siguiente nodo
        free(temp); // Libera la memoria del nodo actual
    }

    // Establece la cabeza de la lista como NULL
    *cabeza = NULL;
}


int kill_ejecucion(struct PCB **cabeza) {
    // Verifica si la lista está vacía
    if (*cabeza == NULL) {
        return -1;
    }

    // Liberar el nodo en cabeza y establecer cabeza como NULL
    free(*cabeza);
    *cabeza = NULL;

    return 0;
}


void re_insert(struct PCB **cabeza, struct PCB *ejecucion) {
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
    if(nuevoNodo == NULL){
        mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
        refresh();
        return;
    }
    *nuevoNodo = *ejecucion; // Copiar la estructura ejecucion en nuevoNodo
    nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL

    // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
        kill_ejecucion(&ejecucion); 
        refresh();
        return;
    }

    // Buscar el último nodo de la lista
    struct PCB *ultimo = *cabeza;
    while (ultimo->sig != NULL) {
        ultimo = ultimo->sig;
    }

    // Enlazar el nuevo nodo después del último nodo
    ultimo->sig = nuevoNodo;
    kill_ejecucion(&ejecucion); 
    refresh();
}


void actualizarKCPUxU(struct PCB **cabeza, int UID, int IncCPU){
    // Verifica si la lista está vacía
    if (*cabeza == NULL) {
        return; // Devuelve si la lista está vacía
    }
    struct PCB *Nodo = *cabeza;
    while (Nodo != NULL){
        if(Nodo->UID == UID){
            (Nodo->KCPUxU) += IncCPU;
        }
        Nodo = Nodo->sig;
    }
}

void Actualizar_planificacion(struct PCB **cabeza, int PBase, float W) {
    // Verifica si la lista está vacía
    if (*cabeza == NULL) {
        return; // Devuelve si la lista está vacía
    }
    struct PCB *Nodo = *cabeza;
    while (Nodo != NULL) {
        Nodo->KCPU /= 2;
        Nodo->KCPUxU /= 2;
        float tempP = PBase + (Nodo->KCPU) / 2.0 + (Nodo->KCPUxU) / (4.0 * W);//realizando operaciones con puros floats
        Nodo->P = (int)tempP; // Convierte el resultado a int
        Nodo = Nodo->sig; 
    }
}

// Función para cargar el contenido del programa en el archivo SWAP
void CargarSwap(struct PCB *nodo) {
    // Abre el archivo del programa en modo lectura
    char NombrePrograma[256];
    strcpy(NombrePrograma, nodo->fileName);
    FILE *programa = fopen(NombrePrograma, "r");
    if (programa == NULL) {
        return;
    }


    char linea[256];
    int PC = 0;

    // Leer cada línea del archivo del programa
    while (fgets(linea, sizeof(linea), programa) != NULL) {
        // Calcular el marco y el desplazamiento
        int marco = PC / 16;
        int offset = PC % 16;
        //int DRS = (marco << 4) | offset;
        int marco_swap = nodo->TMP[marco];
        int tamano_marco = 16;
        int desplazamiento_real = (marco_swap * tamano_marco + offset) * 32;

        // Dividir la línea en instrucciones de 32 caracteres
        int instrucciones = strlen(linea) / 32 + (strlen(linea) % 32 != 0);
        for (int i = 0; i < instrucciones; i++) {
            char instruccion[33]; // Buffer para una instrucción de 32 caracteres + terminador nulo
            strncpy(instruccion, linea + i * 32, 32); // Copiar la instrucción
            instruccion[32] = '\0'; // Agregar terminador nulo si es necesario
            int len = strlen(instruccion);
            if (len < 32) {
                // Completar la instrucción con caracteres nulos si es necesario
                memset(instruccion + len, '\0', 32 - len);
            }
            
            // Escribir la instrucción en el archivo de intercambio
            fseek(file, desplazamiento_real + i * 32, SEEK_SET);
            fwrite(instruccion, sizeof(char), 32, file);
        }

        PC++;
    }

    // Cerrar los archivos
    fclose(programa);
}

void AgregarNodoAListos_ManipulandoELTms(struct PCB **cabeza, struct PCB *nuevoNodo) {
    // Asignar direcciones reales TMP y marcar TMS
    int ContadorCeros2 = nuevoNodo->TmpSize;
    int j = 0;
    for (int i = 0; i < tamano; i++) {
        if (Mapa[i] == 0) { // Si está libre
            Mapa[i] = nuevoNodo->PID; // Poner el PID en el TMS
            nuevoNodo->TMP[j] = i; // Poner las direcciones reales en el TMP
            ContadorCeros2--;
            j++;
            if (ContadorCeros2 <= 0) {
                break;
            }
        }
    }

    // Añadir el nodo a la lista de listos
    nuevoNodo->sig = NULL; // Asegurarse de que el nuevo nodo no apunte a ningún otro nodo

    if (*cabeza == NULL) {
        *cabeza = nuevoNodo; // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
    } else {
        struct PCB *ultimo = *cabeza;
        while (ultimo->sig != NULL) {
            ultimo = ultimo->sig;
        }
        ultimo->sig = nuevoNodo; // Enlazar el nuevo nodo después del último nodo
    }
    CargarSwap(nuevoNodo);
}


int ChecarSwapLibre(int tmpSize){
    //hay disponible en la swap?
        for (int i = 0; i < tamano; i++){
            if(Mapa[i] == 0){
                tmpSize --;
                if(tmpSize <= 0){
                    return 200; //si llega menor o igual a cero si hay espacio
                }
            }
        }//despues del bucle si contadorceros es menor a cero si hay espacio en swap
    return 400;
    
}

int CalulcarTMPSize(char nombrePrograma[256]){
    //calcular tmpSize
    int tmpSize;
    int contador = 0;
    char buffer[1024]; // Tamaño del buffer para leer líneas
    FILE *archivo = fopen(nombrePrograma, "r");

    // Leer línea por línea hasta el final del archivo
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        contador++;
    }
    tmpSize = contador / 16;
    if (contador % 16 != 0) { // Verifica si hay un residuo
        tmpSize++; // Incrementa en uno si hay un residuo
    }
    fclose(archivo);
    //fin de calculo tmpSize
    return tmpSize;
}

// Función para crear un nuevo nodo de PCB y agregar el nombre del programa al inicio de la lista
void insert(struct PCB **cabeza, char *nombrePrograma, int Pbase, int user_id, struct PCB **nuevos, struct PCB **terminados) {
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
    if(nuevoNodo == NULL){
        mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
        refresh();
        return;
    }
    nuevoNodo->PID = P_count++;
    nuevoNodo->AX = 0;
    nuevoNodo->BX = 0;
    nuevoNodo->CX = 0;
    nuevoNodo->DX = 0;
    nuevoNodo->PC = 0;
    strcpy(nuevoNodo->fileName, nombrePrograma);
    strcpy(nuevoNodo->IR, "    ");
    nuevoNodo->programa = fopen(nuevoNodo->fileName, "r");
    nuevoNodo->P = Pbase;
    nuevoNodo->KCPU = 0;
    nuevoNodo->KCPUxU = 0;
    nuevoNodo->UID = user_id;



    nuevoNodo->TmpSize = CalulcarTMPSize(nuevoNodo->fileName); //calculando tmpsize
    nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL


    

    if(nuevoNodo->TmpSize <= 4096){//si cabe en swap
        //hay disponible en la swap?
        if(ChecarSwapLibre(nuevoNodo->TmpSize) == 200){//si hay espacio en swap actualmente
            
            AgregarNodoAListos_ManipulandoELTms(cabeza, nuevoNodo);


        }else{//no hay espacio en swap actualmente pasarlo a nuevos
            // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
            if (*nuevos == NULL) {
                *nuevos = nuevoNodo;
                return;
            }

            // Buscar el último nodo de la lista
            struct PCB *ultimo = *nuevos;
            while (ultimo->sig != NULL) {
                ultimo = ultimo->sig;
            }

            // Enlazar el nuevo nodo después del último nodo
            ultimo->sig = nuevoNodo;
        }
        



    }else{//no cabe, mandar directo a terminados

        // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
        if (*terminados == NULL) {
            *terminados = nuevoNodo;
            return;
        }

        // Buscar el último nodo de la lista
        struct PCB *ultimo = *terminados;
        while (ultimo->sig != NULL) {
            ultimo = ultimo->sig;
        }

        // Enlazar el nuevo nodo después del último nodo
        ultimo->sig = nuevoNodo;

        int ejey = 40;
        int ejex = 1;
        mvprintw(ejey, ejex, "                                                                 ");
        mvprintw(ejey, ejex, "No hay espacio suficiente en la SWAP");
        refresh();


    }
}


void push(struct PCB **cabeza, struct PCB *ejecucion) {
    QuitarPidDelTMS((ejecucion)->PID);
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
    if(nuevoNodo == NULL){
        mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
        refresh();
        return;
    }
    *nuevoNodo = *ejecucion; // Copiar la estructura ejecucion en nuevoNodo
    nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL

    // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
        return;
    }

    // Buscar el último nodo de la lista
    struct PCB *ultimo = *cabeza;
    while (ultimo->sig != NULL) {
        ultimo = ultimo->sig;
    }

    // Enlazar el nuevo nodo después del último nodo
    ultimo->sig = nuevoNodo;
}










void prints_titulos(){
    mvprintw(34, 82, "Listos");
    mvprintw(34, 140, "Terminados");
    mvprintw(0, 150, "Ejecucion");
    mvprintw(55, 1, "Nuevos");
    refresh();
}



void imprimir_nuevos(struct PCB *cabeza, int x, int y){
    struct PCB *actual = cabeza;
    // Itera sobre todos los nodos de la lista
    for(int i = y; i<=y+40; i++){
            mvprintw(i, x, "                                                                 -");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        mvprintw(y, x, "PID:%d, Programa:%s, UID:[%d]", actual->PID, actual->fileName, actual->UID);
        
        // Avanza al siguiente nodo
        actual = actual->sig;
        y++; // Incrementa la fila de impresión
    }
    refresh(); // Refresca la pantalla


}

void imprimir_ejecion(struct PCB *cabeza, int x, int eje_y) {
    struct PCB *primerNodo = cabeza; // Guarda referencia al primer nodo
    
    
    if (primerNodo != NULL) {
        // Imprime los valores del primer nodo
        mvprintw(eje_y, x, "PID:%d, Nombre del programa:%s, UID:[%d]   ", primerNodo->PID, primerNodo->fileName, primerNodo->UID);
    }
    
    refresh(); // Refresca la pantalla
    
}


void imprimir_listos(struct PCB *cabeza, int x, int eje_y) {
    struct PCB *actual = cabeza; // Inicia desde la cabeza de la lista
    
    // Itera sobre todos los nodos de la lista
    for(int i = eje_y; i<=eje_y+45; i++){
            mvprintw(i, x, "                                                        -");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        mvprintw(eje_y, x, "PID:%d, Programa:%s, UID:[%d], KCPUxU:%d, P:%d, KCPU:%d", actual->PID, actual->fileName, actual->UID, actual->KCPUxU, actual->P, actual->KCPU);
        
        // Avanza al siguiente nodo
        actual = actual->sig;
        eje_y++; // Incrementa la fila de impresión
    }
    refresh(); // Refresca la pantalla
}

void imprimir_terminados(struct PCB *cabeza, int x, int eje_y) {
    struct PCB *actual = cabeza; // Inicia desde la cabeza de la lista
    
    // Itera sobre todos los nodos de la lista
    for(int i = eje_y; i<=eje_y+45; i++){
        mvprintw(i, x, "                                                                 -");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        mvprintw(eje_y, x, "PID:%d, Programa:%s, AX:%d, BX:%d, CX:%d, DX:%d, PC:%d, UID:%d", actual->PID, actual->fileName, actual->AX, actual->BX, actual->CX, actual->DX, actual->PC, actual->UID);
        eje_y++;
        mvprintw(eje_y, x, "KCPUxU:%d, KCPU:%d, P:%d, IR:", actual->KCPUxU, actual->KCPU, actual->P);
        // Imprime los caracteres de IR uno a uno
        for (int i = 0; actual->IR[i] != '\0'; i++) {
            addch(actual->IR[i]);
        }

        // Avanza al siguiente nodo
        actual = actual->sig;
        eje_y++; // Incrementa la fila de impresión
    }
    refresh(); // Refresca la pantalla
}


void contador_de_usuarios(struct PCB **listos, struct PCB **ejecucion, int *contador) {
    (*contador) = 0;

    // Crear un array para almacenar los UID únicos
    int uid_unicos[256]; // Suponemos que hay como máximo 256 UID distintos
    int num_uid_unicos = 0;

    // Contar UID únicos en ejecución
    if (*ejecucion != NULL) {
        struct PCB *actual_ejecucion = *ejecucion;
        while (actual_ejecucion != NULL) {
            int encontrado = 0;
            for (int i = 0; i < num_uid_unicos; i++) {
                if (uid_unicos[i] == actual_ejecucion->UID) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                uid_unicos[num_uid_unicos] = actual_ejecucion->UID;
                num_uid_unicos++;
            }
            actual_ejecucion = actual_ejecucion->sig;
        }
    }

    // Contar UID únicos en listos
    if (*listos != NULL) {
        struct PCB *actual_listos = *listos;
        while (actual_listos != NULL) {
            int encontrado = 0;
            for (int i = 0; i < num_uid_unicos; i++) {
                if (uid_unicos[i] == actual_listos->UID) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                uid_unicos[num_uid_unicos] = actual_listos->UID;
                num_uid_unicos++;
            }
            actual_listos = actual_listos->sig;
        }
    }

    // Asignar el valor final al contador
    (*contador) = num_uid_unicos;
}

void MeterNuevos_Listos(struct PCB **nuevos, struct PCB **listos) {
    if (nuevos == NULL || *nuevos == NULL) {
        return; // No hay nada que procesar si nuevos es NULL o la lista de nuevos está vacía
    }

    struct PCB *actual = *nuevos;
    struct PCB *anterior = NULL;

    while (actual != NULL) {
        struct PCB *siguiente = actual->sig; // Guardar el siguiente nodo antes de modificar los punteros
        if (ChecarSwapLibre(actual->TmpSize) == 200) {
            // Agregar el nodo actual a la lista de listos
            AgregarNodoAListos_ManipulandoELTms(listos, actual);

            // Eliminar el nodo actual de la lista de nuevos
            if (anterior == NULL) {
                // Si actual es el primer nodo en la lista de nuevos
                *nuevos = siguiente;
            } else {
                // Si actual está en el medio o final de la lista
                anterior->sig = siguiente;
            }
            // No mover al siguiente nodo aún, ya que ya está establecido en 'siguiente'
        } else {
            // Avanzar los punteros
            anterior = actual;
        }
        actual = siguiente; // Mover al siguiente nodo
    }
}
