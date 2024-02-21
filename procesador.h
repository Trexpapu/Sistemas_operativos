#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> // Para la función toupper
#include <stdlib.h>

struct PCB
{
    char IR[100];
    int AX;
    int BX;
    int CX;
    int DX;
    int PC;
};

void librerar_recursos(struct PCB *pcb) {
    free(pcb);
}




void toUPPER(char *cadena){
    for (int i = 0; cadena[i] != '\0'; i++) {
            cadena[i] = toupper(cadena[i]);
        }
}

void realizar_operaciones(int accion, int registro, int operador, struct PCB *pcb){
    if(accion==10){//es move
        if(registro==10){//ax
            if(operador == 1000000){//ax
                pcb->AX=pcb->AX;
            }else if(operador == 2000000){//bx
                pcb->AX=pcb->BX;
            }else if(operador == 300000){//cx
                pcb->AX=pcb->CX;
            }else if(operador == 400000){//dx
                pcb->AX=pcb->DX;
            }else{//es un valor numerico
                pcb->AX=operador;
            }


        }else if(registro==20){//bx
            if(operador == 1000000){//ax
                pcb->BX=pcb->AX;
            }else if(operador == 2000000){//bx
                pcb->BX=pcb->BX;
            }else if(operador == 300000){//cx
                pcb->BX=pcb->CX;
            }else if(operador == 400000){//dx
                pcb->BX=pcb->DX;
            }else{//es un valor numerico
                pcb->BX=operador;
            }

        }else if(registro==30){//cx
            if(operador == 1000000){//ax
                    pcb->CX=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->CX=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->CX=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->CX=pcb->DX;
                }else{//es un valor numerico
                    pcb->CX=operador;
                }

        }else if(registro==40){//dx
                if(operador == 1000000){//ax
                    pcb->DX=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->DX=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->DX=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->DX=pcb->DX;
                }else{//es un valor numerico
                    pcb->DX=operador;
                }

        }
    }else if(accion==20){//es add
        if(registro==10){//ax
            if(operador == 1000000){//ax
                pcb->AX+=pcb->AX;
            }else if(operador == 2000000){//bx
                pcb->AX+=pcb->BX;
            }else if(operador == 300000){//cx
                pcb->AX+=pcb->CX;
            }else if(operador == 400000){//dx
                pcb->AX+=pcb->DX;
            }else{//es un valor numerico
                pcb->AX+=operador;
            }


        }else if(registro==20){//bx
            if(operador == 1000000){//ax
                pcb->BX+=pcb->AX;
            }else if(operador == 2000000){//bx
                pcb->BX+=pcb->BX;
            }else if(operador == 300000){//cx
                pcb->BX+=pcb->CX;
            }else if(operador == 400000){//dx
                pcb->BX+=pcb->DX;
            }else{//es un valor numerico
                pcb->BX+=operador;
            }

        }else if(registro==30){//cx
            if(operador == 1000000){//ax
                    pcb->CX+=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->CX+=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->CX+=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->CX+=pcb->DX;
                }else{//es un valor numerico
                    pcb->CX+=operador;
                }

        }else if(registro==40){//dx
                if(operador == 1000000){//ax
                    pcb->DX+=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->DX+=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->DX+=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->DX+=pcb->DX;
                }else{//es un valor numerico
                    pcb->DX+=operador;
                }

        }

    }else if(accion==30){//sub
            if(registro==10){//ax
                if(operador == 1000000){//ax
                    pcb->AX-=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->AX-=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->AX-=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->AX-=pcb->DX;
                }else{//es un valor numerico
                    pcb->AX-=operador;
                }


        }else if(registro==20){//bx
                if(operador == 1000000){//ax
                    pcb->BX-=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->BX-=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->BX-=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->BX-=pcb->DX;
                }else{//es un valor numerico
                    pcb->BX-=operador;
                }

        }else if(registro==30){//cx
                if(operador == 1000000){//ax
                        pcb->CX-=pcb->AX;
                    }else if(operador == 2000000){//bx
                        pcb->CX-=pcb->BX;
                    }else if(operador == 300000){//cx
                        pcb->CX-=pcb->CX;
                    }else if(operador == 400000){//dx
                        pcb->CX-=pcb->DX;
                    }else{//es un valor numerico
                        pcb->CX-=operador;
                    }

        }else if(registro==40){//dx
                    if(operador == 1000000){//ax
                        pcb->DX-=pcb->AX;
                    }else if(operador == 2000000){//bx
                        pcb->DX-=pcb->BX;
                    }else if(operador == 300000){//cx
                        pcb->DX-=pcb->CX;
                    }else if(operador == 400000){//dx
                        pcb->DX-=pcb->DX;
                    }else{//es un valor numerico
                        pcb->DX-=operador;
                    }

        }
    }else if(accion==40){//es mul
            if(registro==10){//ax
                if(operador == 1000000){//ax
                    pcb->AX*=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->AX*=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->AX*=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->AX*=pcb->DX;
                }else{//es un valor numerico
                    pcb->AX*=operador;
                }


        }else if(registro==20){//bx
                if(operador == 1000000){//ax
                    pcb->BX*=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->BX*=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->BX*=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->BX*=pcb->DX;
                }else{//es un valor numerico
                    pcb->BX*=operador;
                }

        }else if(registro==30){//cx
                if(operador == 1000000){//ax
                        pcb->CX*=pcb->AX;
                    }else if(operador == 2000000){//bx
                        pcb->CX*=pcb->BX;
                    }else if(operador == 300000){//cx
                        pcb->CX*=pcb->CX;
                    }else if(operador == 400000){//dx
                        pcb->CX*=pcb->DX;
                    }else{//es un valor numerico
                        pcb->CX*=operador;
                    }

        }else if(registro==40){//dx
                    if(operador == 1000000){//ax
                        pcb->DX*=pcb->AX;
                    }else if(operador == 2000000){//bx
                        pcb->DX*=pcb->BX;
                    }else if(operador == 300000){//cx
                        pcb->DX*=pcb->CX;
                    }else if(operador == 400000){//dx
                        pcb->DX*=pcb->DX;
                    }else{//es un valor numerico
                        pcb->DX*=operador;
                    }

        }
    
    }else if(accion==50){//es div
        if(registro==10){//ax
                if(operador == 1000000){//ax
                    pcb->AX/=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->AX/=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->AX/=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->AX/=pcb->DX;
                }else{//es un valor numerico
                    pcb->AX/=operador;
                }


        }else if(registro==20){//bx
                if(operador == 1000000){//ax
                    pcb->BX/=pcb->AX;
                }else if(operador == 2000000){//bx
                    pcb->BX/=pcb->BX;
                }else if(operador == 300000){//cx
                    pcb->BX/=pcb->CX;
                }else if(operador == 400000){//dx
                    pcb->BX/=pcb->DX;
                }else{//es un valor numerico
                    pcb->BX/=operador;
                }

        }else if(registro==30){//cx
                if(operador == 1000000){//ax
                        pcb->CX/=pcb->AX;
                    }else if(operador == 2000000){//bx
                        pcb->CX/=pcb->BX;
                    }else if(operador == 300000){//cx
                        pcb->CX/=pcb->CX;
                    }else if(operador == 400000){//dx
                        pcb->CX/=pcb->DX;
                    }else{//es un valor numerico
                        pcb->CX/=operador;
                    }

        }else if(registro==40){//dx
                    if(operador == 1000000){//ax
                        pcb->DX/=pcb->AX;
                    }else if(operador == 2000000){//bx
                        pcb->DX/=pcb->BX;
                    }else if(operador == 300000){//cx
                        pcb->DX/=pcb->CX;
                    }else if(operador == 400000){//dx
                        pcb->DX/=pcb->DX;
                    }else{//es un valor numerico
                        pcb->DX/=operador;
                    }

        }
    }

}


int validar_registro(char *registro){
    if(strcmp(registro, "AX") == 0){
        return 10;
    }else if(strcmp(registro, "BX") == 0){
        return 20;
    }else if(strcmp(registro, "CX") == 0){
        return 30;
    }else if(strcmp(registro, "DX") == 0){
        return 40;
    }else{
        return 50;
    }


    return 0;
}

int validar_operador(char *operador){
        if (strcmp(operador, "MOV") == 0){
            return 10;
        }else if (strcmp(operador, "ADD") == 0){
            return 20;
        }else if (strcmp(operador, "SUB") == 0){
            return 30;
        }else if (strcmp(operador, "MUL") == 0){
            return 40;
        }else if (strcmp(operador, "DIV") == 0){
            return 50;
        }else{
            return 60;
        }
    return 0;
}

int EsDigito(char *numero){
    //validar si es un numero
        int digito=0;
        int negativo = 0;
        for(int i = 0;numero[i]!= '\0';i++){
            if(numero[i] == '-'){
                negativo = 1;
            }else if(isdigit(numero[i])){//verficamos si son digitos, si es asi mantener digito en cero
                digito=0;
            }else{//si no es asi cambiamos digito a 1 y rompemos el bucle for
                digito=1;
                break;
            }
        }
        if (digito==0){
            if(negativo==0){
                return atoi(numero);
            }else{
                return strtol(numero, NULL, 10);
            }
        }else{
            return 9999999;//si no es un registro ni un valor numerico 
        }
    return 0;
}

int validar_p2(char *p2){
    if(strcmp(p2, "AX") == 0){
        return 1000000;
    }else if(strcmp(p2, "BX") == 0){
        return 2000000;
    }else if(strcmp(p2, "CX") == 0){
        return 300000;
    }else if(strcmp(p2, "DX") == 0){
        return 400000;
    }else{
        return EsDigito(p2);
    }
    
    
    return 0;
}

void realizar_inc_o_dec(int opcion, int registro, struct PCB *pcb){
    if (opcion==1){
        if(registro==10){//ax
            pcb->AX++;
        }else if(registro==20){//bx
            pcb->BX++;
        }else if(registro==30){//cx
            pcb->CX++;
        }else if(registro==40){//dx
            pcb->DX++;
        }
    }else{
        if(registro==10){//ax
            pcb->AX--;
        }else if(registro==20){//bx
            pcb->BX--;
        }else if(registro==30){//cx
            pcb->CX--;
        }else if(registro==40){//dx
            pcb->DX--;
        }
    }
}

int validar_operaciones(const char *linea, struct PCB *pcb) {
    char operacion[10];
    char p1[10];
    char p2[20];
    char error[20];
    int registro=0;//registro
    int operador_de_3=0;//operador cuando se trata de mov, div, mul, add
    int valor_p2 = 0;//registro o numerico


    if (sscanf(linea, "%s %s %s %s", operacion, p1, p2, error) == 3){//si la linea tiene 3 strings
        // Convertir operacion a mayúsculas las palabras
        toUPPER(operacion);
        toUPPER(p1);
        toUPPER(p2);
        operador_de_3=validar_operador(operacion);//validamos la primera parte como mov, div...
        registro=validar_registro(p1);//validamos p1, es decir que sea un registro
        valor_p2 = validar_p2(p2);//validamos que p2 sea un registro o numero

        if (operador_de_3==60){
            return 405;
        }
        if (registro==50){
            return 405;
        }
        if (valor_p2==9999999){
            return 405;
        }
        if (operador_de_3==50 && valor_p2==0){//si la operaciones dividir y p2 es un cero
            return 405;
        }
        realizar_operaciones(operador_de_3, registro, valor_p2, pcb);
        
    }else if(sscanf(linea, "%s %s %s %s", operacion, p1, p2, error) == 2){//si es inc o dec
        toUPPER(operacion);
        toUPPER(p1);
        if (strcmp(operacion, "INC")==0){//si es inc
            registro=validar_registro(p1);//obtenemos a cual registro
            if (registro==50){
                return 405;
            }
            realizar_inc_o_dec(1, registro, pcb);//realizamos operaciones, 1 es para INC

        }else if(strcmp(operacion, "DEC")==0){//si es dec
            registro=validar_registro(p1);//obtenemos cual registro
            if (registro==50){
                return 405;
            }
            realizar_inc_o_dec(2, registro, pcb);//realizamos operaciones, 2 es para DEC

        }else{
            return 405;
        }
    }else if(sscanf(linea, "%s %s %s %s", operacion, p1, p2, error) == 1){
        toUPPER(operacion);
        if(strcmp(operacion, "END")==0){
            return 425;
        }
    }else{
        return 405;
    }

    return 0;
}

int leer_archivo(char archivo[100], struct PCB *pcb){
    pcb->AX=0;
    pcb->BX=0;
    pcb->CX=0;
    pcb->DX;
    pcb->PC=0;
    
    int error = 0;
    FILE *n_archivo;
    n_archivo = fopen(archivo, "r");
    // Lee y muestra cada línea del archivo
    while (fgets(pcb->IR, sizeof(pcb->IR), n_archivo) != NULL) {
        mvprintw(25, 100, "IR                                                                       ");
        mvprintw(25, 100, "IR: %s", pcb->IR);//imprimimos el IR, cada linea de archivo

        pcb->PC++;
        mvprintw(20, 100, "PC:         ");
        mvprintw(20, 100, "PC: %d", pcb->PC);//imprimimos el PC, contador de las lineas de archivo

        mvprintw(0, 100, "AX:                                                                       ");
        mvprintw(0, 100, "AX: %d", pcb->AX);

        mvprintw(5, 100, "BX:                                                                       ");
        mvprintw(5, 100, "BX: %d", pcb->BX);

        mvprintw(10, 100, "CX:                                                                       ");
        mvprintw(10, 100, "CX: %d", pcb->CX);

        mvprintw(15, 100, "DX:                                                                       ");
        mvprintw(15, 100, "DX: %d", pcb->DX);

        usleep(400000); //espera para ver cada lectura del archivo
        error = validar_operaciones(pcb->IR, pcb);
        if (error == 405){//error de sintaxis en el archivo
             
            return error;
        }else if(error == 425){//cuando se encuentra la palabra END en el archivo
            break;
        }
        refresh();
    }

    // Cierra el archivo
    fclose(n_archivo);
    refresh();
    return 0;

}

int verificar_archivo(char archivo[100]){
    FILE *n_archivo;
    char linea[100];
    n_archivo = fopen(archivo, "r");
    // Verifica si el archivo se abrió correctamente
    if (n_archivo == NULL) {
        return 800; //no se pudo abrir correctamente
    }
    fclose(n_archivo);

    return 0;
}

void prints_procesador(){
    mvprintw(0, 100, "AX: %d", 0);
    mvprintw(5, 100, "BX: %d", 0);
    mvprintw(10, 100, "CX: %d", 0);
    mvprintw(15, 100, "DX: %d", 0);
    mvprintw(20, 100, "PC: %d", 0);
    mvprintw(25, 100, "IR:");
    mvprintw(0,0,"");
    refresh();
}
