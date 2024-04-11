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
    int PID;
    char fileName[256];
    FILE *programa; //puntero a un archivo
    int UID; //User IDentifier (Identificador de usuario).
    int P; //Prioridad del proceso.
    int KCPU; //Contador de uso de CPU por proceso.
    int KCPUxU; //Contador de uso de CPU por usuario.
    struct PCB *sig; //siguiente estructura o nodo
    
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
        }else if(strcmp(operador, "INC") == 0){
            return 700;
        }else if(strcmp(operador, "DEC") == 0){
            return 800;
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
            return 9999999;//si no es un registro ni un valor numerico :(((((
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

int validar_operaciones_de_archivo(struct PCB *pcb) {
    char operacion[10];
    char p1[10];
    char p2[20];
    char error[20];
    int registro=0;//registro
    int operador=0;//operador cuando se trata de mov, div, mul, add
    int valor_p2 = 0;//registro o numerico


    if (sscanf(pcb->IR, "%s %s %s %s", operacion, p1, p2, error) == 3){//si la pcb->IR tiene 3 strings
        // Convertir operacion a mayúsculas las palabras
        toUPPER(operacion);
        toUPPER(p1);
        toUPPER(p2);

        operador = validar_operador(operacion);//validamos la primera parte como mov, div...
        registro = validar_registro(p1);//validamos p1, es decir que sea un registro
        valor_p2 = validar_p2(p2);//validamos que p2 sea un registro o numero


        //manejo de errores al validar las operaciones
        if(operador==700 || operador==800){
            return 501;//operacion de inc o dec con demasiados parametros
        }

        if (operador==60){
            return 401;// OPERACION DE 3 NO VALIDA, MUL, ADD, SUB, DIV, MOV
        }
        if (registro==50){
            return 402;//P1 REGISTRO NO RECONOCIDO
        }
        if (valor_p2==9999999){
            return 403;//NO ES REGISTRO NI NUMERO, P2 NO RECONOCIDO
        }
        if (operador==50 && valor_p2==0){//si la operaciones dividir y p2 es un cero
            return 404;//DIVISION ENTRE CERO
        }
        if (operador == 50){
            if(valor_p2 == 1000000){//division entre un registro que vale cero
                if(pcb->AX == 0){
                    return 404;//DIVISION ENTRE CERO
                }
            }else if(valor_p2 == 2000000){
                if(pcb->BX == 0){
                    return 404;//DIVISION ENTRE CERO
                }
            }else if(valor_p2 == 300000){
                if(pcb->CX == 0){
                    return 404;//DIVISION ENTRE CERO
                }
            }else if(valor_p2 == 400000){
                if(pcb->DX == 0){
                    return 404;//DIVISION ENTRE CERO
                }
            }
        }

        realizar_operaciones(operador, registro, valor_p2, pcb); //REALIZAMOS OPERACIONES
        
    }else if(sscanf(pcb->IR, "%s %s %s %s", operacion, p1, p2, error) == 2){//si es inc o dec
        toUPPER(operacion);
        toUPPER(p1);
        operador = validar_operador(operacion);

        if (operador==700){//si es inc
            registro=validar_registro(p1);//obtenemos a cual registro
            if (registro==50){
                return 402;//P1 REGISTRO NO RECONOCIDO
            }
            realizar_inc_o_dec(1, registro, pcb);//realizamos operaciones, 1 es para INC

        }else if(operador==800){//si es dec
            registro=validar_registro(p1);//obtenemos cual registro
            if (registro==50){
                return 402;//P1 REGISTRO NO RECONOCIDO
            }

            realizar_inc_o_dec(2, registro, pcb);//realizamos operaciones, 2 es para DEC

        }else if(operador==10 || operador == 20 || operador == 30 || operador == 40){
            return 502;//OPERACION de 3 con falta de parametros

        }else{
            return 401;//OPERACION DE DOS NO VALIDA, NO ES INC NI DEC
        }

    }else if(sscanf(pcb->IR, "%s %s %s %s", operacion, p1, p2, error) == 1){
        toUPPER(operacion);
        if(strcmp(operacion, "END")==0){
            return 405;//SE ENCONTRO LA PALABRA END
        }else{
            operador = validar_operador(operacion);
            if(operador==10 || operador == 20 || operador == 30 || operador == 40 || operador == 700 || operador == 800){
                return 503; //operador con falta de parametros 
            }
        }
    }else{
        return 406;//ERROR DE SINTAXIS LA LINEA TIENE instrucciones demasiado largas
    }

    return 0;
}

void impresionPCB(struct PCB *pcb){
    


        mvprintw(0, 85, "AX:     ");
        mvprintw(0, 85, "AX: %d", pcb->AX);

        mvprintw(5, 85, "BX:     ");
        mvprintw(5, 85, "BX: %d", pcb->BX);

        mvprintw(10, 85, "CX:    ");
        mvprintw(10, 85, "CX: %d", pcb->CX);

        mvprintw(15, 85, "DX:     ");
        mvprintw(15, 85, "DX: %d", pcb->DX);

        mvprintw(25, 85, "PID:   ");
        mvprintw(25, 85, "PID: %d", pcb->PID);

        mvprintw(25, 100, "FileName:                             ");
        mvprintw(25, 100, "FileName: %s", pcb->fileName);


        mvprintw(20, 100, "IR                  ");
        mvprintw(20, 100, "IR: %s", pcb->IR);//imprimimos el IR, cada linea de archivo

        mvprintw(20, 85, "PC:   ");
        mvprintw(20, 85, "PC: %d", pcb->PC);//imprimimos el PC, contador de las lineas de archivo

        mvprintw(15, 100, "UID:      ");
        mvprintw(15, 100, "UID: %d", pcb->UID);

        mvprintw(10, 100, "P:    ");
        mvprintw(10, 100, "P: %d", pcb->P);
        
        mvprintw(5, 100, "KCPU:     ");
        mvprintw(5, 100, "KCPU: %d", pcb->KCPU);


        mvprintw(0, 100, "KCPUxU:     ");
        mvprintw(0, 100, "KCPUxU: %d", pcb->KCPUxU);

    refresh();
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
        mvprintw(20, 100, "IR     ");

        mvprintw(20, 85, "PC:   ");

        mvprintw(0, 85, "AX:     ");

        mvprintw(5, 85, "BX:      ");

        mvprintw(10, 85, "CX:      ");

        mvprintw(15, 85, "DX:       ");

        mvprintw(25, 85, "PID:      ");

        mvprintw(15, 100, "UID:     ");

        mvprintw(10, 100, "P:     ");

        mvprintw(5, 100, "KCPU:     ");

        mvprintw(0, 100, "KCPUxU:     ");

        mvprintw(25, 100, "FileName:    ");
        mvprintw(0,0,"");
    refresh();
}
