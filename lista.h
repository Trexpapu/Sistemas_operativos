#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int contador_usuarios = 0;
int P_count = 1;

#define TOTAL_MARCOS  4096 // 65536 instrucciones / 16 instrucciones por marco de la swap
int TMS[TOTAL_MARCOS]; // TMS
int tamano = sizeof(TMS) / sizeof(TMS[0]);

FILE *file;
const char *filename = "SWAP.bin";



//seccion de funciones y datos para la ram
int indiceReloj = 0;
#define TOTAL_MARCOS_RAM 16//256 instrucciones de 32 bytes... 256 / 16 ......es lo mimso que (TOTAL_MARCOS_RAM / (16*32))
typedef struct {
    int pid;        // PID del proceso que ocupa el marco
    int referencia; // Bandera de referencia (1 si ha sido referenciado, 0 si no)
} MemoryFrame;

// TMM
MemoryFrame TMM[TOTAL_MARCOS_RAM];

#define TAMANO_RAM 8192 // 256 instrucciones * 32 bytes cada una
char RAM[TAMANO_RAM];





int EspacioEnRAM(){
    for(int i = 0; i< TOTAL_MARCOS_RAM; i++){ 
        if(TMM[i].pid == 0){
            return 200;
        }
    }
    return 0;

}



void CargarRAM(struct PCB **pcb, int MarcoPC, int marcoLibre) {
    int DesplazamientoSWAP = (*pcb)->TMP.SWAP[MarcoPC] * 32 * 16;
    fseek(file, DesplazamientoSWAP, SEEK_SET);
    if (fread(&RAM[marcoLibre * 32 * 16], 32, 16, file) != 16) {
        perror("Error leyendo desde SWAP");
        // Manejo de error adecuado, tal vez marcando el marco como no válido
    }
}


int agregarHermanosRAM(struct PCB **nuevoNodo, struct PCB **listos, struct PCB **ejecucion){
    int uid = (*nuevoNodo)->UID;
    char fileName[256];
    strcpy(fileName, (*nuevoNodo)->fileName);

    //buscar en listos
    struct PCB *actual = *listos;
    while(actual != NULL){
        if(actual->UID == uid && strcmp(actual->fileName, fileName) == 0){
            for(int i = 0; i< actual->TmpSize; i++){
                (*nuevoNodo)->TMP.RAM[i] = actual->TMP.RAM[i];
                (*nuevoNodo)->TMP.presencia[i] = actual->TMP.presencia[i];
            }

            return 200;
        }
        
        actual = actual->sig;
    }
    //buscar en ejecucion
    struct PCB *actual2 = *ejecucion;
    if(actual2 != NULL){
        if(actual2->UID == uid && strcmp(actual2->fileName, fileName) == 0){
            for(int i = 0; i< actual2->TmpSize; i++){
                (*nuevoNodo)->TMP.RAM[i] = actual2->TMP.RAM[i];
                (*nuevoNodo)->TMP.presencia[i] = actual2->TMP.presencia[i];
            }

            return 200;
        }
    }


    return 0;
}


int CargarSiPresenciasIguales(struct PCB **ejecucion, struct PCB **listos){
    int uid = (*ejecucion)->UID;
    char fileName[256];
    strcpy(fileName, (*ejecucion)->fileName);
    struct PCB *actual = *listos;
    while(actual != NULL){
        if(actual->UID == uid && strcmp(actual->fileName, fileName) == 0){
            for(int i = 0; i< actual->TmpSize; i++){
                if(actual->TMP.presencia[i] != (*ejecucion)->TMP.presencia[i]){
                    return 200;
                }
            }
            
        }
        
        actual = actual->sig;
    }

    return 0;
}
void copiarRAMyPresenciaEnListosNoReloj(struct PCB **nodo_alterado, struct PCB **listos){
    int uid = (*nodo_alterado)->UID;
    char fileName[256];
    strcpy(fileName, (*nodo_alterado)->fileName);
    struct PCB *actual = *listos;
    while(actual != NULL){
        if(actual->UID == uid && strcmp(actual->fileName, fileName) == 0){
            for(int i = 0; i<actual->TmpSize; i++){
                actual->TMP.RAM[i] = (*nodo_alterado)->TMP.RAM[i];
                actual->TMP.presencia[i] = (*nodo_alterado)->TMP.presencia[i];
            }
        }


        actual = actual->sig;
    }
}

void copiarRAMyPresenciaEnListosSiReloj(struct PCB **nodo_alterado, struct PCB **listos, struct PCB **ejecucion){
    
    int uid = (*nodo_alterado)->UID;
    char fileName[256];
    strcpy(fileName, (*nodo_alterado)->fileName);
    //buscamos los hermanos en listos
    struct PCB *actual = *listos;
    while(actual != NULL){
        if(actual->UID == uid && strcmp(actual->fileName, fileName) == 0){
            for(int i = 0; i<actual->TmpSize; i++){
                actual->TMP.RAM[i] = (*nodo_alterado)->TMP.RAM[i];
                actual->TMP.presencia[i] = (*nodo_alterado)->TMP.presencia[i];
            }
        }


        actual = actual->sig;
    }
    //para ejecucion
    struct PCB *actual2 = *ejecucion;
    if(actual2 != NULL){
        if(actual2->UID == uid && strcmp(actual2->fileName, fileName) == 0){
            for(int i = 0; i<actual2->TmpSize; i++){
                actual2->TMP.RAM[i] = (*nodo_alterado)->TMP.RAM[i];
                actual2->TMP.presencia[i] = (*nodo_alterado)->TMP.presencia[i];
            }
        }
    }
}

int BuscarHermanosTMM(struct PCB **ejecucion, struct PCB **listos, int *hermanoPID) {
    struct PCB *actual = *listos;
    struct PCB *referencia = *ejecucion;

    if (referencia != NULL) {
        while (actual != NULL) { // buscar en cada nodo
            if (referencia->UID == actual->UID && strcmp(referencia->fileName, actual->fileName) == 0) {
                // una vez encontrado el hermano, buscar si su PID está cargado en la TMM

                for (int i = 0; i < TOTAL_MARCOS_RAM; i++) {
                    if (actual->PID == TMM[i].pid) { // si al menos un PID de este hermano está cargado, retornar ese PID

                        *hermanoPID = actual->PID;
                        return 200;
                    }
                }
            }
            actual = actual->sig;
        }
    }
    

    return 0; // no se encontró ningún hermano cargado en TMM
}


void AgregarPIDEnTMM(struct PCB **ejecucion,  int marco, struct PCB **listos) {
    for (int i = 0; i < TOTAL_MARCOS_RAM; i++) {
        if (TMM[i].pid == 0) {
            // verificar si un hermano está cargado
            int hermanoPID = 0;
            int res = BuscarHermanosTMM(ejecucion, listos, &hermanoPID);
            
            if (res != 200) {
                TMM[i].pid = (*ejecucion)->PID;
            } else {
                
                TMM[i].pid = hermanoPID;
            }

            TMM[i].referencia = 1;
            (*ejecucion)->TMP.RAM[marco] = i;
            (*ejecucion)->TMP.presencia[marco] = 1;
            CargarRAM(ejecucion, marco, i);
            copiarRAMyPresenciaEnListosNoReloj(ejecucion, listos);

            return;
        }
    }
}











void cambiarPIDEnTMM(struct PCB **ejecucion, struct PCB **listos, int marco) {
    //buscamos si el que estamos desalojando esta en listos
    int encontrado = 0;
    //desalojamos

    //vemos si alteramos algo en algun nodo en listos
    struct PCB *actual = *listos;
    while (actual != NULL){
        
        if(actual->PID == TMM[indiceReloj].pid){//encontramos el pid del nodo que vamos a desalojar
            for(int i = 0; i < actual->TmpSize; i++){//recorremos todos los marcos del nodo
                if(actual->TMP.RAM[i] == indiceReloj){ // buscamos el marco en ram que sea igual al marco que encontramos en la tmm
                    actual->TMP.RAM[i] = -1;//desalojamos el marco en la tmp
                    actual->TMP.presencia[i] = 0;//marcamos presencia en la tmp en cero
                    encontrado = 1;

                }
            }

            //reflejmaos el cambio para todos los hermanos
            if(encontrado == 1){
                copiarRAMyPresenciaEnListosSiReloj(&actual, listos, ejecucion);
            }
            
        }

        actual = actual->sig;
        //desalojamos al marco correspondiente de cada hermano
        
    }


    //si no esta en listos podria estar en ejecucion
    if(encontrado == 0){
        if((*ejecucion) != NULL){
            if((*ejecucion)->PID == TMM[indiceReloj].pid){
                for(int i = 0; i < (*ejecucion)->TmpSize; i++){
                    if((*ejecucion)->TMP.RAM[i] == indiceReloj){
                        (*ejecucion)->TMP.RAM[i] = -1;
                        (*ejecucion)->TMP.presencia[i] = 0;
                        encontrado = 1;
                    }
                }
            }
            //reflejmaos el cambio para todos los hermanos
            if(encontrado == 1){
               copiarRAMyPresenciaEnListosNoReloj(ejecucion, listos);
            }
        }
    }




    //agregar el nuevo marco
    TMM[indiceReloj].pid = 0;
    int hermanoPID = 0;
    int res = BuscarHermanosTMM(ejecucion, listos, &hermanoPID);
            
    if (res != 200) {
        TMM[indiceReloj].pid = (*ejecucion)->PID;
    }else {
            
        TMM[indiceReloj].pid = hermanoPID;
     }
    
    (*ejecucion)->TMP.presencia[marco] = 1;
    (*ejecucion)->TMP.RAM[marco] = indiceReloj;
    CargarRAM(ejecucion, marco, indiceReloj);

    copiarRAMyPresenciaEnListosNoReloj(ejecucion, listos);

}



void AlgoritmoReloj(struct PCB **ejecucion, struct PCB **listos, int marco){
    while(true) {
        if (indiceReloj >= TOTAL_MARCOS_RAM) {
            indiceReloj = 0;
        }

        if (TMM[indiceReloj].referencia == 1){
            TMM[indiceReloj].referencia = 0;
        } else {
            
            cambiarPIDEnTMM(ejecucion, listos, marco);
            TMM[indiceReloj].referencia = 1;
            indiceReloj++;
            break; // Salir del bucle una vez que se ha cambiado el PID
        }
        
        
        indiceReloj++;
        
    }
}


int HeredarHermanosTMM(struct PCB **listos, char *fileName, int UID, struct PCB **ejecucion, int opcion) {
    if (opcion == 0) { // Buscar en listos y ejecucion
        struct PCB *actual = *listos;
        while (actual != NULL) {
            if (UID == actual->UID && strcmp(fileName, actual->fileName) == 0) { // En efecto son procesos hermanos
                for (int i = 0; i < actual->TmpSize; i++) { // Buscamos en cada marco de la RAM
                    if (actual->TMP.presencia[i] == 1) { // Solo si el marco está presente
                        int marcoRAM = actual->TMP.RAM[i];
                        TMM[marcoRAM].pid = actual->PID;
                    }
                }
                return 200;
            }
            actual = actual->sig;
        }
        

        struct PCB *referencia = *ejecucion;
        
        if (referencia != NULL) {
            if (UID == (referencia)->UID && strcmp(fileName, (referencia)->fileName) == 0) { // En efecto son procesos hermanos
                for (int i = 0; i < (referencia)->TmpSize; i++) { // Buscamos en cada marco de la RAM
                    if ((referencia)->TMP.presencia[i] == 1) { // Solo si el marco está presente
                        int marcoRAM = (referencia)->TMP.RAM[i];
                        TMM[marcoRAM].pid = (referencia)->PID;
                    }
                }
                return 200;
            }
        }
    } else if (opcion == 1) { // Buscar solo en listos
        struct PCB *actual = *listos;
        while (actual != NULL) {
            if (UID == actual->UID && strcmp(fileName, actual->fileName) == 0) { // En efecto son procesos hermanos
                for (int i = 0; i < actual->TmpSize; i++) { // Buscamos en cada marco de la RAM
                    if (actual->TMP.presencia[i] == 1) { // Solo si el marco está presente
                        int marcoRAM = actual->TMP.RAM[i];
                        TMM[marcoRAM].pid = actual->PID;
                    }
                }
                return 200;
            }

            actual = actual->sig;
        }
    }
    
    return 0;
}

int quitarPIDTMM(int pid){
    for(int i = 0; i< TOTAL_MARCOS_RAM;i++){
        if(TMM[i].pid == pid){
            TMM[i].pid = 0;
            TMM[i].referencia = 0;
            
        }
    }
    return 0;
}

void agregarTMM(struct PCB **ejecucion, struct PCB **listos,   int marco){
     if (EspacioEnRAM() == 200){
        
            AgregarPIDEnTMM(ejecucion, marco, listos);
        

     }//si no hay espacio en la ram activa algoritmo de reloj
     else{
       
        
            AlgoritmoReloj(ejecucion, listos,  marco);
        
     }
     


}

void InicializarTMM(){
    for (int i = 0; i < TOTAL_MARCOS_RAM; i++) {
        TMM[i].pid = 0;        // PID = -1 para indicar que está libre
        TMM[i].referencia = 0;  // Referencia = 0 para indicar que no ha sido referenciado
    }
}

void InicializarRAM(){
    memset(RAM, 0, sizeof(RAM));

}


void ImprimirDatosTMM(int opcion, int *referencia){
    mvprintw(1, 270, "TMM");
    mvprintw(3, 270, "Marco-PID-Referenciado-Indice");
    if(opcion == 2003 && ((*referencia)+16) < TOTAL_MARCOS_RAM){ //tms hacia delante
        (*referencia) += 16;
    }else if(opcion == 2004 && (*referencia) >=16){
        (*referencia) -= 16;
    }

    int x = 270; // el eje x en la pantalla
    int encabezado = 5; // de donde empieza a imprimir en la pantalla
    int j = 0;//salto de linea para imprimir las filas
    int final = (*referencia) + 16; //96 para mostrar 6 marcos
    for (long i = (*referencia); i < final; i++){
        mvprintw(encabezado+j, x, "             ");
        mvprintw(encabezado+j, x, "[%03X]-%d-%d", i, TMM[i].pid, TMM[i].referencia);
        mvprintw(encabezado+j, x+20, " ");
            if(i == indiceReloj){
                mvprintw(encabezado+j, x+20, " ");
                mvprintw(encabezado+j, x+20, "*");
            }
        
        j++;

    }
    refresh();

}

void ImprimirZonaRAM(int *referencia, int opcion, int *marco) {
    int ejex = 210;
    int ejey = 52;
    mvprintw(ejey, ejex, "    ");
    mvprintw(ejey, ejex, "RAM");

    if (opcion == 2001 && (*marco) < TOTAL_MARCOS_RAM) { // Avance hacia delante
        (*referencia) += 64;
        (*marco) += 4;
    } else if (opcion == 2002 && (*marco) > 4) { // Retroceso
        (*referencia) -= 64;
        (*marco) -= 4;
    }

    int encabezado = 54;
    int final = (*referencia) + 64;
    int row = encabezado;
    int col = ejex;

    // Imprimir en el formato especificado
    for (int i = *referencia; i < final; i += 16) {
        for (int offset = 0; offset < 16; offset++) {
            if (i + offset < TOTAL_MARCOS_RAM) {
                if ((unsigned char)RAM[i + offset] != 0) {
                    mvprintw(row + offset, col, "[%04X] %-16s", i + offset, &RAM[(i + offset) * 32]);
                } else {
                   mvprintw(row + offset, col, "[%04X] %-16s", i + offset, &RAM[(i + offset) * 32]);
                }
            } else {
                mvprintw(row + offset, col, "[%04X] %-16s", i + offset, &RAM[(i + offset) * 32]);
            }
        }
        col += 20;
        if (col > 500) {
            col = ejex;
            row += 18;
        }
    }

    refresh();
}








//seccion de funciones y datos para la ram finalizada





void QuitarPidDelTMS(int pid){
    for (int i = 0; i < tamano; i++){
        if (TMS[i] == pid){
            TMS[i] = 0;
        }
    }
    refresh();
}

void AgregarTerminados(struct PCB **terminados, struct PCB *nuevoNodo){
     nuevoNodo->sig = NULL; // Asegurarse de que el nuevo nodo no apunte a ningún otro nodo

    if (*terminados == NULL) {
            *terminados = nuevoNodo; // Si la lista está vacía, el nuevo nodo se convierte en la terminados
    } else {
            struct PCB *ultimo = *terminados;
            while (ultimo->sig != NULL) {
                    ultimo = ultimo->sig;
                }
            ultimo->sig = nuevoNodo; // Enlazar el nuevo nodo después del último nodo
    }
}

int buscaHermanosHeredar(struct PCB **listos, int PID, char *fileName, int UID, int TmpSize, struct PCB ** ejecucion, int opcion){
   
   
    if (opcion == 0){ // para buscar en listos y ejecucion
        
        struct PCB *referencia = *listos;
        
            while (referencia != NULL){
                if(UID == referencia->UID && strcmp(fileName, referencia->fileName) == 0){//en efecto son procesos hermanos
                    int contadorCerosTemp = TmpSize;
                    int j = 0;
                    for (int i = 0; i < tamano; i++){
                        if(TMS[i] == PID){
                            TMS[i] = referencia->PID;
                            contadorCerosTemp--;
                            j++;
                            if (contadorCerosTemp <= 0){
                                
                                return 200;
                            }
                        }
                    }
                }
                referencia = referencia->sig;
            }
        
        struct PCB *referencia2 = *ejecucion;
            if(referencia2 != NULL){
            
                if(UID == referencia2->UID && strcmp(fileName, referencia2->fileName) == 0){//en efecto son procesos hermanos
                    int contadorCerosTemp = TmpSize;
                    int j = 0;
                    for (int i = 0; i < tamano; i++){
                        if(TMS[i] == PID){
                            TMS[i] = referencia2->PID;
                            contadorCerosTemp--;
                            j++;
                            if (contadorCerosTemp <= 0){
                                
                                return 200;
                            }
                        }
                    }
                }

            
            }
        


    }else if (opcion == 1){ //buscar solo en listos
        struct PCB *referencia = *listos;
            while (referencia != NULL){
                if(UID == referencia->UID && strcmp(fileName, referencia->fileName) == 0){//en efecto son procesos hermanos
                    int contadorCerosTemp = TmpSize;
                    int j = 0;
                    for (int i = 0; i < tamano; i++){
                        if(TMS[i] == PID){
                            TMS[i] = referencia->PID;
                            contadorCerosTemp--;
                            j++;
                            if (contadorCerosTemp <= 0){
                                
                                return 200;
                            }
                        }
                    }
                }
                referencia = referencia->sig;
            }
        

    }
    return 0;
}


int kill_push(struct PCB **listos, int pid, struct PCB **terminados, struct PCB **ejecucion, int Opcion){
    if (Opcion == 0){//condicion para buscar kill en listos
            if (*listos == NULL){ // Verifica si la lista donde buscamos está vacía
                return -1;
            }
        

        // Verifica si el PID está en el primer nodo
        if ((*listos)->PID == pid){
            

            // Crear un nuevo nodo de PCB
            struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
            if(nuevoNodo == NULL){
                mvprintw(40, 5, "Error no se pudo reservar más memoria...");
                refresh();
                return 3;
            }

            // Copia el primer nodo en el nuevo nodo
            *nuevoNodo = **listos;
            nuevoNodo->sig = NULL;
            int PID = (nuevoNodo->PID);
            char fileName[256];
            strcpy(fileName, (nuevoNodo->fileName));
            int UID = (nuevoNodo->UID);
            int TmpSize = (nuevoNodo->TmpSize);


            AgregarTerminados(terminados, nuevoNodo);

            // Elimina el primer nodo de la lista
            struct PCB *temp = *listos;
            *listos = (*listos)->sig;
            fclose(temp->programa);
            free(temp);

            if(buscaHermanosHeredar(listos, PID, fileName, UID, TmpSize, ejecucion, 0) != 200){ //no encontro hermanos asi que solo quitamos en el tms el pid del proceso
                //quitamos el pid del tms
                QuitarPidDelTMS(pid);
                
            }

            if(HeredarHermanosTMM(listos,fileName, UID, ejecucion, 0) != 200){
                //liberar TMM
                quitarPIDTMM(pid);
            }

            return 0;
        }

        // Busca el nodo con el PID dado
        struct PCB *actual = *listos;
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
            int PID = (nuevoNodo->PID);
            char fileName[256];
            strcpy(fileName, (nuevoNodo->fileName));
            int UID = (nuevoNodo->UID);
            int TmpSize = (nuevoNodo->TmpSize);


            AgregarTerminados(terminados, nuevoNodo);

            fclose(actual->programa);
            free(actual);

            if(buscaHermanosHeredar(listos, PID, fileName, UID, TmpSize, ejecucion, 0) != 200){ //no encontro hermanos asi que solo quitamos en el tms el pid del proceso
                //quitamos el pid del tms
                QuitarPidDelTMS(pid);
                
            }
            if(HeredarHermanosTMM(listos,fileName, UID, ejecucion, 0) != 200){
                //liberar TMM
                quitarPIDTMM(pid);
            }

        } else {
            return -1; // No se encontró el PID en la lista
        }
        return 0;
    
    }else if (Opcion == 1){//condicion para buscar kill en ejecucion

        if (*ejecucion == NULL){ // Verifica si la lista donde buscamos está vacía
                return -1; // no esta el pid
            }
        

        // Verifica si el PID está en el primer nodo
        if ((*ejecucion)->PID == pid){
            

            // Crear un nuevo nodo de PCB
            struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
            if(nuevoNodo == NULL){
                mvprintw(40, 5, "Error no se pudo reservar más memoria...");
                refresh();
                return 3;
            }

            // Copia el primer nodo en el nuevo nodo
            *nuevoNodo = **ejecucion;
            nuevoNodo->sig = NULL;
            int PID = (nuevoNodo->PID);
            char fileName[256];
            strcpy(fileName, (nuevoNodo->fileName));
            int UID = (nuevoNodo->UID);
            int TmpSize = (nuevoNodo->TmpSize);


            AgregarTerminados(terminados, nuevoNodo);

            // Elimina el primer nodo de la lista
            struct PCB *temp = *ejecucion;
            *ejecucion = (*ejecucion)->sig;
            fclose(temp->programa);
            free(temp);

            if(buscaHermanosHeredar(listos, PID, fileName, UID, TmpSize, ejecucion, 0) != 200){ //no encontro hermanos asi que solo quitamos en el tms el pid del proceso
                //quitamos el pid del tms
                QuitarPidDelTMS(pid);
                
            }
            if(HeredarHermanosTMM(listos,fileName, UID, ejecucion, 0) != 200){
                //liberar TMM
                quitarPIDTMM(pid);
            }

            return 0;
        }else{
            return -1; //no se encontro el pid
        }



    }


    
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



void re_insert(struct PCB **listos, struct PCB **ejecucion) {
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
    if(nuevoNodo == NULL){
        mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
        refresh();
        return;
    }
    *nuevoNodo = **ejecucion; // Copiar la estructura ejecucion en nuevoNodo
    nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL

    // Si la lista está vacía, el nuevo nodo se convierte en la cabeza
    if (*listos == NULL) {
        kill_ejecucion(ejecucion);
        *listos = nuevoNodo;
        refresh();
        return;
    }

    // Buscar el último nodo de la lista
    struct PCB *ultimo = *listos;
    while (ultimo->sig != NULL) {
        ultimo = ultimo->sig;
    }

    // Enlazar el nuevo nodo después del último nodo
    ultimo->sig = nuevoNodo;
    kill_ejecucion(ejecucion); 
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
    sleep(2);
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
        // Eliminar el salto de línea si existe
        linea[strcspn(linea, "\n")] = 0;

        // Verificar si la línea no está en blanco
        int is_blank = 1;
        for (int i = 0; i < strlen(linea); i++) {
            if (!isspace(linea[i])) {
                is_blank = 0;
                break;
            }
        }
        if (is_blank) {
            continue;
        }

        // Calcular el marco y el desplazamiento
        int marco = PC / 16;
        int offset = PC % 16;
        //int DRS = (marco << 4) | offset;
        int marco_swap = nodo->TMP.SWAP[marco];
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

int BuscaHermanosInsertar(struct PCB **listos, struct PCB *nuevoNodo, struct PCB **ejecucion){
    

    //buscamos primero en listos
    struct PCB *referencia = *listos;
    while (referencia != NULL){
        if(nuevoNodo->UID == referencia->UID && strcmp(nuevoNodo->fileName, referencia->fileName) == 0){
            int contadorTemp = nuevoNodo->TmpSize;
            int j = 0;
            for(int i = 0; i < tamano; i++){
                if(TMS[i] == referencia->PID){
                    nuevoNodo->TMP.SWAP[j] = i;
                    contadorTemp--;
                    j++;
                    if(contadorTemp <= 0){
                        return 200;
                    }
                }
            }

        }
        referencia = referencia->sig;
    }


    //buscamos en ejecucion
    struct PCB *referencia2 = *ejecucion;
    if (referencia2 != NULL){
        if(referencia2->UID == nuevoNodo->UID && strcmp(referencia2->fileName, nuevoNodo->fileName) == 0){
            int contadorTemp = nuevoNodo->TmpSize;
                    int j = 0;
                    for (int i = 0; i < tamano; i++){
                        if(TMS[i] == referencia2->PID){
                            nuevoNodo->TMP.SWAP[j] = i;
                            contadorTemp--;
                            j++;
                            if (contadorTemp <= 0){
                                return 200;
                            }
                        }
                    }

        }
    }

    return 0;
}

void AgregarListos(struct PCB **listos, struct PCB *nuevoNodo){
     nuevoNodo->sig = NULL; // Asegurarse de que el nuevo nodo no apunte a ningún otro nodo

    if (*listos == NULL) {
            *listos = nuevoNodo; // Si la lista está vacía, el nuevo nodo se convierte en la listos
    } else {
            struct PCB *ultimo = *listos;
            while (ultimo->sig != NULL) {
                    ultimo = ultimo->sig;
                }
            ultimo->sig = nuevoNodo; // Enlazar el nuevo nodo después del último nodo
    }
}



void AgregarPIDTMS_CargarSWAP(struct PCB *nuevoNodo){

     // Asignar direcciones reales TMP y marcar TMS
            int ContadorCeros2 = nuevoNodo->TmpSize;
            int j = 0;
            for (int i = 0; i < tamano; i++) {
                if (TMS[i] == 0) { // Si está libre
                    TMS[i] = nuevoNodo->PID; // Poner el PID en el TMS
                    nuevoNodo->TMP.SWAP[j] = i; // Poner las direcciones reales en el TMP
                    ContadorCeros2--;
                    j++;
                    if (ContadorCeros2 <= 0) {
                        break;
                    }
                }
            }

            CargarSwap(nuevoNodo);

}



int ChecarSwapLibre(int tmpSize){
    //hay disponible en la swap?
        for (int i = 0; i < tamano; i++){
            if(TMS[i] == 0){
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


void AgregarNuevos(struct PCB **nuevos, struct PCB *nuevoNodo){
     nuevoNodo->sig = NULL; // Asegurarse de que el nuevo nodo no apunte a ningún otro nodo

    if (*nuevos == NULL) {
            *nuevos = nuevoNodo; // Si la lista está vacía, el nuevo nodo se convierte en la nuevos
    } else {
            struct PCB *ultimo = *nuevos;
            while (ultimo->sig != NULL) {
                    ultimo = ultimo->sig;
                }
            ultimo->sig = nuevoNodo; // Enlazar el nuevo nodo después del último nodo
    }
}



int calcularLimitePC(char fileName[256]){
    FILE *programa = fopen(fileName, "r");


    char linea[256];
    int Limite_pc = 0;

    // Leer cada línea del archivo del programa
    while (fgets(linea, sizeof(linea), programa) != NULL){
        linea[strcspn(linea, "\n")] = 0;

        // Verificar si la línea no está en blanco
        int is_blank = 1;
        for (int i = 0; i < strlen(linea); i++) {
            if (!isspace(linea[i])) {
                is_blank = 0;
                break;
            }
        }
        if (is_blank) {
            continue;
        }else{
            Limite_pc++;
        }
        
    }
    fclose(programa);
    return Limite_pc;

}


// Función para crear un nuevo nodo de PCB y agregar el nombre del programa al inicio de la lista
int insert(struct PCB **listos, char *nombrePrograma, int Pbase, int user_id, struct PCB **nuevos, struct PCB **terminados, struct PCB ** ejecucion) {
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
    if(nuevoNodo == NULL){
        mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
        refresh();
        return 0;
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
    nuevoNodo->LimitePC = calcularLimitePC(nombrePrograma);
    

    nuevoNodo->TmpSize = CalulcarTMPSize(nuevoNodo->fileName); //calculando tmpsize

    //crear los arreglos de la TMP
     // Asignar memoria para los arreglos en TMPDATA
    nuevoNodo->TMP.SWAP = (int *)malloc(nuevoNodo->TmpSize * sizeof(int));
    nuevoNodo->TMP.RAM = (int *)malloc(nuevoNodo->TmpSize * sizeof(int)); // Suponiendo que RAM tiene un tamaño fijo de 16
    nuevoNodo->TMP.presencia = (int *)malloc(nuevoNodo->TmpSize * sizeof(int));
    for(int i = 0; i<nuevoNodo->TmpSize; i++){
        nuevoNodo->TMP.RAM[i] = -1;
        nuevoNodo->TMP.SWAP[i]= 0;
        nuevoNodo->TMP.presencia[i] = 0;
    }

    nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL


    

    if(nuevoNodo->TmpSize <= TOTAL_MARCOS){//si cabe en swap


        if(BuscaHermanosInsertar(listos, nuevoNodo, ejecucion) == 200){//si tiene hermanos
            agregarHermanosRAM(&nuevoNodo, listos, ejecucion);  
            
            AgregarListos(listos, nuevoNodo);
            



        }else{//no tiene hermanos

            if(ChecarSwapLibre(nuevoNodo->TmpSize) == 200){//hay espacio en la swap?
                AgregarPIDTMS_CargarSWAP(nuevoNodo);
                AgregarListos(listos, nuevoNodo);

            }else{
                AgregarNuevos(nuevos, nuevoNodo);
            }

        }

    }else{//no cabe, mandar directo a terminados

        AgregarTerminados(terminados, nuevoNodo);
        return nuevoNodo->PID;


    }

    return 0;
}


void push(struct PCB **terminados, struct PCB **ejecucion, struct PCB **listos) {
    int PID = (*ejecucion)->PID;
    char fileName[256];
    strcpy(fileName, (*ejecucion)->fileName);
    int UID = (*ejecucion)->UID;
    int TmpSize = (*ejecucion)->TmpSize;
    if(buscaHermanosHeredar(listos, PID, fileName, UID, TmpSize, ejecucion, 1) != 200){ //no encontro hermanos asi que solo quitamos en el tms el pid del proceso
            //quitamos el pid del tms
            QuitarPidDelTMS(PID);
                
    }
    if(HeredarHermanosTMM(listos,fileName, UID, ejecucion, 1) != 200){
                //liberar TMM
            quitarPIDTMM(PID);
    }
    // Crear un nuevo nodo de PCB
    struct PCB *nuevoNodo = (struct PCB*)malloc(sizeof(struct PCB));
    if(nuevoNodo == NULL){
        mvprintw(40, 5, "Error no se puedo reservar mas memoria...");
        refresh();
        return;
    }
    *nuevoNodo = *(*ejecucion); // Copiar la estructura ejecucion en nuevoNodo
    nuevoNodo->sig = NULL; // Establecer el siguiente del nuevo nodo como NULL

    AgregarTerminados(terminados, nuevoNodo);
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
        int marco = actual->PC / 16;
        mvprintw(eje_y, x, "KCPUxU:%d, KCPU:%d, P:%d, Marco:%d, IR:", actual->KCPUxU, actual->KCPU, actual->P, marco);
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

int contador_de_usuarios(struct PCB **listos, struct PCB **ejecucion){
    int contador_usuarios = 0;

    int totalUID = 256;
    int uid_unicos[totalUID];
    for (int i = 0; i < totalUID; i++){
        uid_unicos[i] = 0;
    }
    int usuarios_unicos = 0;
    int encontrado = 0;

    // Buscamos para listos
    struct PCB *actual = *listos;
    if (actual != NULL) {
        while (actual != NULL) {
            encontrado = 0;
            for (int i = 0; i < usuarios_unicos; i++) {
                if (uid_unicos[i] == actual->UID) {
                    encontrado = 1;
                    break;
                }
            }
            if (encontrado == 0 && usuarios_unicos < totalUID) {
                uid_unicos[usuarios_unicos] = actual->UID;
                usuarios_unicos++;
            }
            actual = actual->sig;
        }
    }

    // Buscamos para ejecucion
    struct PCB *referencia = *ejecucion;
    encontrado = 0;
    if (referencia != NULL) {
        for (int i = 0; i < usuarios_unicos; i++) {
            if (uid_unicos[i] == referencia->UID) {
                encontrado = 1;
                break;
            }
        }
        if (encontrado == 0 && usuarios_unicos < totalUID) {
            uid_unicos[usuarios_unicos] = referencia->UID;
            usuarios_unicos++;
        }
    }

    return usuarios_unicos;
}





void MeterNuevos_Listos(struct PCB **nuevos, struct PCB **listos, struct PCB **ejecucion) {
    if (nuevos == NULL || *nuevos == NULL) {
        return; // No hay nada que procesar si nuevos es NULL o la lista de nuevos está vacía
    }
    int MetioNuevosAListos = 0;

    struct PCB *actual = *nuevos;
    struct PCB *anterior = NULL;

    while (actual != NULL) {
        struct PCB *siguiente = actual->sig; // Guardar el siguiente nodo antes de modificar los punteros
        if (ChecarSwapLibre(actual->TmpSize) == 200) {
            // Agregar el nodo actual a la lista de listos
            AgregarPIDTMS_CargarSWAP(actual);
            AgregarListos(listos, actual);
            MetioNuevosAListos = 1;

            // Eliminar el nodo actual de la lista de nuevos
            if (anterior == NULL) {
                // Si actual es el primer nodo en la lista de nuevos
                *nuevos = siguiente;
            } else {
                // Si actual está en el medio o final de la lista
                anterior->sig = siguiente;
            }
            // Liberar el nodo actual de la lista de nuevos
            actual->sig = NULL;
        } else {
            // Avanzar los punteros
            anterior = actual;
        }
        actual = siguiente; // Mover al siguiente nodo

    }

        if (MetioNuevosAListos == 1) {
            struct PCB *actual2 = *nuevos;
            struct PCB *anterior2 = NULL;
            MetioNuevosAListos = 0;

            while (actual2 != NULL) {
                struct PCB *siguiente2 = actual2->sig; // Guardar el siguiente nodo antes de modificar los punteros
                if (BuscaHermanosInsertar(listos, actual2, ejecucion) == 200) { // Si tiene hermanos
                    // Agregar el nodo actual a la lista de listos
                    agregarHermanosRAM(&actual2, listos, ejecucion); 
                    AgregarListos(listos, actual2);

                    // Eliminar el nodo actual de la lista de nuevos
                    if (anterior2 == NULL) {
                        // Si actual2 es el primer nodo en la lista de nuevos
                        *nuevos = siguiente2;
                    } else {
                        // Si actual2 está en el medio o final de la lista
                        anterior2->sig = siguiente2;
                    }
                    // Liberar el nodo actual de la lista de nuevos
                    actual2->sig = NULL;
                } else {
                    // Avanzar los punteros
                    anterior2 = actual2;
                }
                actual2 = siguiente2; // Mover al siguiente nodo
            }
        }
    
}
