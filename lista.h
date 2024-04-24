#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int P_count = 1;

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


// Función para crear un nuevo nodo de PCB y agregar el nombre del programa al inicio de la lista
void insert(struct PCB **cabeza, char *nombrePrograma, int Pbase, int user_id) {
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


void push(struct PCB **cabeza, struct PCB *ejecucion) {
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



int kill_push(struct PCB **cabeza, int pid, struct PCB **terminados){

    if (*cabeza == NULL){//verficamos si la lista donde buscamos esta vacia
        return -1;
    }

    //vemos si el pid esta en el primer nodo
    if ((*cabeza)->PID == pid){

        // Crear un nuevo nodo de PCB
            struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
                if(nuevoNodo == NULL){
                mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
                refresh();
                return 3;
                }

            nuevoNodo = *cabeza;
            nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL

            // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
            if (*terminados == NULL) {
                *terminados = nuevoNodo;
                return 0;
            }

            // Buscar el último nodo de la lista
            struct PCB *ultimo = *terminados;
            while (ultimo->sig != NULL) {
                ultimo = ultimo->sig;
            }

            // Enlazar el nuevo nodo después del último nodo
            ultimo->sig = nuevoNodo;


        //liberamos el nodo 
        struct PCB *temp = *cabeza;//referencia al nodo a eliminar
        *cabeza = (*cabeza)->sig; //avanzamos al siguiente nodo
        fclose(temp->programa);
        free(temp); // Libera la memoria del nodo eliminado
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
                mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
                refresh();
                return 3;
                }
            nuevoNodo = actual;
            nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL

            // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
            if (*terminados == NULL) {
                *terminados = nuevoNodo;
                return 0;
            }

            // Buscar el último nodo de la lista
            struct PCB *ultimo = *terminados;
            while (ultimo->sig != NULL) {
                ultimo = ultimo->sig;
            }

            // Enlazar el nuevo nodo después del último nodo
            ultimo->sig = nuevoNodo;

        fclose(actual->programa);
        free(actual); // Libera la memoria del nodo eliminado
        
    } else {
        return -1;
    }
    return 0;
}



int kill(struct PCB **cabeza, int pid) {
    // Verifica si la lista está vacía
    if (*cabeza == NULL) {
        return -1;
    }

    // Si el nodo a eliminar es el primer nodo
    if ((*cabeza)->PID == pid) {
        struct PCB *temp = *cabeza; // Guarda referencia al nodo a eliminar
        *cabeza = (*cabeza)->sig; // Avanza la cabeza al siguiente nodo
        free(temp); // Libera la memoria del nodo eliminado
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
        free(actual); // Libera la memoria del nodo eliminado
        
    } else {
        return -1;
    }

    return 0;
}




void prints_titulos(){
    mvprintw(34, 82, "Listos");
    mvprintw(34, 125, "Terminados");
    mvprintw(0, 150, "Ejecucion");
    refresh();
}



void imprimir_ejecion(struct PCB *cabeza, int x, int eje_y) {
    struct PCB *primerNodo = cabeza; // Guarda referencia al primer nodo
    
    mvprintw(eje_y, x, "                                                              -");
    
    if (primerNodo != NULL) {
        // Imprime los valores del primer nodo
        mvprintw(eje_y, x, "PID:%d, Nombre del programa:%s, UID:[%d]   ", primerNodo->PID, primerNodo->fileName, primerNodo->UID);
    }
    
    refresh(); // Refresca la pantalla
}


void imprimir_listos(struct PCB *cabeza, int x, int eje_y) {
    struct PCB *actual = cabeza; // Inicia desde la cabeza de la lista
    
    // Itera sobre todos los nodos de la lista
    for(int i = eje_y; i<=55; i++){
            mvprintw(i, x, "                                      -");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        mvprintw(eje_y, x, "PID:%d, Programa:%s, UID:[%d]", actual->PID, actual->fileName, actual->UID);
        
        // Avanza al siguiente nodo
        actual = actual->sig;
        eje_y++; // Incrementa la fila de impresión
    }
    refresh(); // Refresca la pantalla
}

void imprimir_terminados(struct PCB *cabeza, int x, int eje_y) {
    struct PCB *actual = cabeza; // Inicia desde la cabeza de la lista
    
    // Itera sobre todos los nodos de la lista
    for(int i = eje_y; i<=50; i++){
        mvprintw(i, x, "                                                              -");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        //mvprintw(eje_y, x, "PID:%d, Programa:%s, AX:%d, BX:%d, CX:%d, DX:%d, PC:%d, IR:%s", actual->PID, actual->fileName, actual->AX, actual->BX, actual->CX, actual->DX, actual->PC, actual->IR);
       mvprintw(eje_y, x, "PID:%d, Programa:%s, AX:%d, BX:%d, CX:%d, DX:%d, PC:%d, UID:%d, IR:%s, KCPUxU:%d, KCPU:%d, P:%d", actual->PID, actual->fileName, actual->AX, actual->BX, actual->CX, actual->DX, actual->PC, actual->UID, actual->IR, actual->KCPUxU, actual->KCPU, actual->P);

        
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

