#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> // Para la función toupper
#include <stdlib.h>

int P_count = 1;

struct PCB* pull(struct PCB **cabeza, int *pid, char *file_Name) {
    // Verifica si la lista está vacía
    if (*cabeza == NULL) {
        return NULL; // Devuelve NULL si la lista está vacía
    }

    struct PCB* primerNodo = *cabeza; // Guarda referencia al primer nodo
    *pid = primerNodo->PID;
    strcpy(file_Name, primerNodo->fileName);

    // Avanza la cabeza al siguiente nodo
    *cabeza = primerNodo->sig;

    // Libera la memoria del nodo eliminado
    free(primerNodo);

    return primerNodo;
}

void liberar_lista(struct PCB **cabeza) {
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





// Función para crear un nuevo nodo de PCB y agregar el nombre del programa al inicio de la lista
void INSERT(struct PCB **cabeza, char *nombrePrograma) {
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
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


void push(struct PCB **cabeza, char *nombrePrograma, int pid) {
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
    nuevoNodo->PID = pid;
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



int kill(struct PCB **cabeza, int pid) {
    // Verifica si la lista está vacía
    if (*cabeza == NULL) {
        return 1;
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








void imprimirLista(struct PCB *cabeza) {
    struct PCB *actual = cabeza; // Inicia desde la cabeza de la lista
    int eje_y = 35; // Inicializar la eje_y de impresión
    
    // Itera sobre todos los nodos de la lista
    for(int i = 35; i<=60; i++){
        mvprintw(i, 100, "                                                                                                                 ");
    }
    while (actual != NULL) {
        // Imprime los valores del nodo actual
        mvprintw(eje_y, 100, "PID: %d, Nombre del programa: %s\n ", actual->PID, actual->fileName);
        
        // Avanza al siguiente nodo
        actual = actual->sig;
        eje_y++; // Incrementa la fila de impresión
    }
    refresh(); // Refresca la pantalla
}
