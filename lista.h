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

    struct PCB* primerNodo = *cabeza; // Guarda referencia al primer nodo

    // Avanza la cabeza al siguiente nodo
    *cabeza = primerNodo->sig;

    // Devuelve el nodo extraído antes de liberar la memoria
    return primerNodo;
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
    nuevoNodo->PID = ejecucion->PID;
    nuevoNodo->AX = ejecucion->AX;
    nuevoNodo->BX = ejecucion->BX;
    nuevoNodo->CX = ejecucion->CX;
    nuevoNodo->DX = ejecucion->DX;
    nuevoNodo->PC = ejecucion->PC;
    strcpy(nuevoNodo->fileName, ejecucion->fileName);
    strcpy(nuevoNodo->IR, ejecucion->IR);
    nuevoNodo->programa = ejecucion->programa;
    //nuevoNodo = ejecucion;
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




// Función para crear un nuevo nodo de PCB y agregar el nombre del programa al inicio de la lista
void insert(struct PCB **cabeza, char *nombrePrograma) {
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
    nuevoNodo->PID = ejecucion->PID;
    nuevoNodo->AX = ejecucion->AX;
    nuevoNodo->BX = ejecucion->BX;
    nuevoNodo->CX = ejecucion->CX;
    nuevoNodo->DX = ejecucion->DX;
    nuevoNodo->PC = ejecucion->PC;
    strcpy(nuevoNodo->fileName, ejecucion->fileName);
    strcpy(nuevoNodo->IR, ejecucion->IR);
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
            nuevoNodo->PID = (*cabeza)->PID;
            nuevoNodo->AX = (*cabeza)->AX;
            nuevoNodo->BX = (*cabeza)->BX;
            nuevoNodo->CX = (*cabeza)->CX;
            nuevoNodo->DX = (*cabeza)->DX;
            nuevoNodo->PC = (*cabeza)->PC;
            strcpy(nuevoNodo->fileName, (*cabeza)->fileName);
            strcpy(nuevoNodo->IR, (*cabeza)->IR);
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
            nuevoNodo->PID = actual->PID;
            nuevoNodo->AX = actual->AX;
            nuevoNodo->BX = actual->BX;
            nuevoNodo->CX = actual->CX;
            nuevoNodo->DX = actual->DX;
            nuevoNodo->PC = actual->PC;
            strcpy(nuevoNodo->fileName, actual->fileName);
            strcpy(nuevoNodo->IR, actual->IR);
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
    
    mvprintw(eje_y, x, "                                                ");
    
    if (primerNodo != NULL) {
        // Imprime los valores del primer nodo
        mvprintw(eje_y, x, "PID: %d, Nombre del programa: %s\n", primerNodo->PID, primerNodo->fileName);
    }
    
    refresh(); // Refresca la pantalla
}


void imprimir_listos(struct PCB *cabeza, int x, int eje_y) {
    struct PCB *actual = cabeza; // Inicia desde la cabeza de la lista
    
    // Itera sobre todos los nodos de la lista
    for(int i = eje_y; i<=55; i++){
        mvprintw(i, x, "                             ");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        mvprintw(eje_y, x, "PID:%d, Programa:%s\n", actual->PID, actual->fileName);
        
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
        mvprintw(i, x, "                                                              ");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        mvprintw(eje_y, x, "PID:%d, Programa:%s, AX:%d, BX:%d, CX:%d, DX:%d, PC:%d, IR:%s", actual->PID, actual->fileName, actual->AX, actual->BX, actual->CX, actual->DX, actual->PC, actual->IR);

        
        // Avanza al siguiente nodo
        actual = actual->sig;
        eje_y++; // Incrementa la fila de impresión
    }
    refresh(); // Refresca la pantalla
}


void contador_de_programas(struct PCB **listos, struct PCB **ejecucion, int *contador) {
    (*contador) = 0;

    // Crear una lista temporal para almacenar los nombres de archivo únicos
    char nombres_unicos[256][256]; // Suponemos que los nombres de archivo tienen como máximo 256 caracteres
    int num_nombres_unicos = 0;

    // Contar nombres de archivo únicos en ejecución
    if (*ejecucion != NULL) {
        struct PCB *actual_ejecucion = *ejecucion;
        while (actual_ejecucion != NULL) {
            int encontrado = 0;
            for (int i = 0; i < num_nombres_unicos; i++) {
                if (strcmp(nombres_unicos[i], actual_ejecucion->fileName) == 0) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(nombres_unicos[num_nombres_unicos], actual_ejecucion->fileName);
                num_nombres_unicos++;
            }
            actual_ejecucion = actual_ejecucion->sig;
        }
    }

    // Contar nombres de archivo únicos en listos
    if (*listos != NULL) {
        struct PCB *actual_listos = *listos;
        while (actual_listos != NULL) {
            int encontrado = 0;
            for (int i = 0; i < num_nombres_unicos; i++) {
                if (strcmp(nombres_unicos[i], actual_listos->fileName) == 0) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(nombres_unicos[num_nombres_unicos], actual_listos->fileName);
                num_nombres_unicos++;
            }
            actual_listos = actual_listos->sig;
        }
    }

    // Asignar el valor final al contador
    (*contador) = num_nombres_unicos;
}
