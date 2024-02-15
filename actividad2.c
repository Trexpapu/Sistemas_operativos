#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include<unistd.h>
#include "kbhit.h"

char archivo[100];
int leer_archivo(char archivo[100]){
    FILE *n_archivo;
    char linea[100];
    n_archivo = fopen(archivo, "r");
    // Verifica si el archivo se abrió correctamente
    if (n_archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }
    // Lee y muestra cada línea del archivo
    while (fgets(linea, sizeof(linea), n_archivo) != NULL) {
        usleep(1000000);
        printf("Prompt>\n%s", linea);
    }

    // Cierra el archivo
    fclose(n_archivo);
    return 0;

}
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



int atiende_shell(char *comando, int *j) {

    if (kbhit()) {
        // Read user input, only one character at a time
        comando[*j] = getchar();
        
        if(comando[*j] == '\n'){
            comando[*j] = '\0';
            return validar_comandos(comando);


        }else{
            (*j)++;
        }
        printf("Prompt> %s", comando);
        printf("\n");
    }
    return 0;
}

int main(void) {
    char comando[60] = {0}; // Initialize the array with zeros
    int j = 0;
    int x = 0;
    printf("Prompt> %s", comando);
    printf("\n");
    while(x!=10){
        x = atiende_shell(comando, &j);
        if(x==200){
            memset(comando, 0, sizeof(comando));
            j=0;
            leer_archivo(archivo);
            printf("\n");
        }
        if(x==405){
            printf("No existe el archivo ingresado\n");
            memset(comando, 0, sizeof(comando));
            j=0;
            printf("Prompt> %s", comando);
            printf("\n");
        }
        if(x==410){
            printf("Error uso correcto del comando es load file\n");
            memset(comando, 0, sizeof(comando));
            j=0;
            printf("Prompt> %s", comando);
            printf("\n");
        }
        if(x==400){
            printf("Comando invalido\n");
            memset(comando, 0, sizeof(comando));
            j=0;
            printf("Prompt> %s", comando);
            printf("\n");
        }
        
    }

    return 0;

}

