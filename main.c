#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"

int main(){
        //Se inicializa root y root_dir, directorio raiz
        Nodo* root = crear_nodo(NULL, "Directorio", "root");
        Directorio* root_dir = (Directorio*)malloc(sizeof(Directorio));
        strcpy(root_dir->nombre, "root");
        root_dir->hijos = crear_lista(10);
        root->contenido = root_dir;
        int exit = 1;
        while(exit == 1){
                //se declara variables para manejo de strings y seleccion de comandos
                char comando[400];
                char instruccion[10];
                char nombre[128];
                char contenido[256];
                int largo;
                printf("> ");
                fgets(comando, sizeof(comando), stdin);
                largo = sscanf(comando, "%s %s %255[^\n]", instruccion, nombre, contenido);

                if(strcmp(instruccion, "mkdir") == 0){
                        mkdir(root, nombre);
                }
                else if(strcmp(instruccion, "ls") == 0){
                        if(largo == 1) ls(root);
                        else ls_dir(root, nombre);
                }
                else if(strcmp(instruccion, "cd") == 0){
                        root = cd(root, nombre);
                }
                else if(strcmp(instruccion, "touch") == 0){
                        touch(root, nombre);
                }
                else if(strcmp(instruccion, "write") == 0){
                        write(root, nombre, contenido);
                }
                else if(strcmp(instruccion, "cat") == 0){
                        cat(root, nombre);
                }
                else if(strcmp(instruccion, "mapdir") == 0){
                        //se evalua parametro para ejecutar mapdir
                        if(strcmp(nombre, "ls") == 0) {
                                mapdir(root, ls_dir, contenido);
                        } else if (strcmp(nombre, "mkdir") == 0 ){
                                mapdir(root, mkdir, contenido);
                        } else if (strcmp(nombre, "touch") == 0){
                                mapdir(root, touch, contenido);
                        }
                }
                //caso 'exit' termina loop
                else if(strcmp(instruccion, "exit") == 0) exit = 0;
        }
        while(root->padre != NULL) root = cd(root, ".."); //volvemos a la raiz antes de finalizar para liberar memoria
        freeNodo(root);      
        return 0;
}