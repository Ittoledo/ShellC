#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"

/*
Descripcion de la funcion
    
    Parametros:
            a (int): Descripcion del parametro a
            b (int): Descripcion del parametro b
    
    Retorno:
            c (str): Descripcion del parametro c
*/

/*
Crea una nueva lista
    
    Parametros:
            largo_maximo_inicial (int): Largo inicial de la lista a crear
    
    Retorno:
            nueva_lista (Lista*): Puntero a lista nueva
*/

Lista* crear_lista(int largo_maximo_inicial) {
        Lista*  nueva_lista = (Lista*)malloc(sizeof(Lista));
        nueva_lista->largo_actual = 0;
        nueva_lista->largo_maximo = largo_maximo_inicial;
        nueva_lista->arreglo = (Nodo**)malloc(largo_maximo_inicial * sizeof(Nodo*));
        return nueva_lista;
}

/*
Inserta un nodo en una lista
    
    Parametros:
            lista (Lista*): Puntero a lista donde insertar nodo
            nodo (Nodo*): Puntero a nodo a insertar
    
    Retorno:
            void: void
*/

void insertar_lista(Lista* lista, Nodo* nodo) {
        //Checkear si alcanzamos el largo maximo de la lista
        if(lista->largo_actual >= lista->largo_maximo) {
                lista->largo_maximo = lista->largo_maximo * 2;
                lista->arreglo = (Nodo**)realloc(lista->arreglo, lista->largo_maximo * sizeof(Nodo*));
        }
        lista->arreglo[lista->largo_actual] = nodo;
        lista->largo_actual++;
}

/*
Busca un directorio de nombre 'nombre' en el directorio entregado (actual)
    
    Parametros:
            actual (Directorio*): Puntero a directorio donde buscar
            nombre (char*): Cadena con nombre a buscar
    
    Retorno:
            nodo (Nodo*): Puntero a nodo con directorio encontrado / en caso de no encontrar
                          Return Null.
*/

Nodo* buscar_directorio(Directorio* actual, char* nombre){
        //Caso si la lista esta vacia
        if(actual->hijos == NULL) return NULL;

        for(int i=0; i < actual->hijos->largo_actual; i++){
                Nodo* nodo = actual->hijos->arreglo[i];

                if(strcmp(((Directorio*)nodo->contenido)->nombre, nombre) == 0 && strcmp(nodo->tipo, "Directorio") == 0) return nodo;
        }

        //Caso en que no encuentre el directorio
        return NULL;
}

/*
Busca un arvhico de nombre 'nombre' en el directorio entregado (actual)
    
    Parametros:
            actual (Directorio*): Puntero a directorio donde buscar
            nombre (char*): Cadena con nombre a buscar
    
    Retorno:
            nodo (Nodo*): Puntero a nodo con archivo encontrado / en caso de no encontrar
                          Return Null.
*/


Nodo* buscar_archivo(Directorio* actual, char* nombre){
        //Caso en que la lista este vacia
        if(actual->hijos == NULL) return NULL;

        for(int i=0; i < actual->hijos->largo_actual; i++){
                Nodo* nodo = actual->hijos->arreglo[i];

                if(strcmp(((Archivo*)nodo->contenido)->nombre, nombre) == 0 && strcmp(nodo->tipo, "Archivo") == 0) return nodo;
        }
        //Caso en que no encuentre el archivo
        return NULL;
}

/*
Crea un nuevo nodo
    
    Parametros:
            padre (Nodo*): Puntero a nodo padre del nuevo nodo
            tipo (char*): Cadena con tipo de nodo (Archivo o Directorio)
            nombre (char*): Cadena con nombre del contenido
    
    Retorno:
            nuevo_nodo (Nodo*): Puntero a nuevo nodo creado
*/


Nodo* crear_nodo(Nodo* padre, char* tipo, char* nombre){
        Nodo* nuevo_nodo = (Nodo*)malloc(sizeof(Nodo));
        nuevo_nodo->padre = (struct Nodo*)padre;
        strcpy(nuevo_nodo->tipo, tipo);
        nuevo_nodo->contenido = NULL;
        return nuevo_nodo;
}

/*
Crea un directorio dentro del nodo entregado con nombre "nombre_directorio"
    
    Parametros:
            actual (Nodo*): Puntero a nodo donde crear directorio
            nombre_directorio (char*): Cadena con nombre de directorio a crear
    
    Retorno:
            void: void // agrega al arbol
*/


void mkdir(Nodo* actual, char* nombre_directorio){
        Directorio* nuevo_directorio = (Directorio*)malloc(sizeof(Directorio));
        strcpy(nuevo_directorio->nombre, nombre_directorio);
        nuevo_directorio->hijos = crear_lista(10);

        //Crea nuevo nodo
        Nodo* nuevo_nodo = crear_nodo(actual, "Directorio", nombre_directorio);
        nuevo_nodo->contenido = nuevo_directorio;

        //Insertar nodo nuevo en la lista del directorio
        insertar_lista(((Directorio*)actual->contenido)->hijos, nuevo_nodo);

}

/*
Cambia al directorio 'nombre_directorio' si es que existe
    
    Parametros:
            actual (Nodo*): Puntero a nodo donde buscar
            nombre_direcotrio (char*): Cadena con directorio a cambiar
    
    Retorno:
            nodo (Nodo*): Puntero a nodo con nuevo 'root', actualiza el directorio actual
*/

Nodo* cd(Nodo* actual, char* nombre_directorio) {
        //Caso en que parametro sea .., cambiamos a directorio anterior
        if (strcmp(nombre_directorio, "..") == 0) {
                if (actual->padre != NULL) {
                        actual = (Nodo*)actual->padre;
                        return actual;
                }
        }
        //Buscamos el directorio
        Nodo* nodo = buscar_directorio((Directorio*)actual->contenido, nombre_directorio);
        if(nodo == NULL) {
                printf("Error, directorio no existe\n");
        }
        else {
                actual = nodo;
        }
        return actual;
}

/*
Crea un archivo dentro del nodo actual
    
    Parametros:
            actual (Nodo*): Puntero a nodo donde crear archivo
            nombre_archivo (char*): Cadena con nombre de archivo a crear
    
    Retorno:
            void : void
*/

void touch(Nodo* actual, char* nombre_archivo){
        Archivo* nuevo_archivo = (Archivo*)malloc(sizeof(Archivo));
        strcpy(nuevo_archivo->nombre, nombre_archivo);
        
        Nodo* nuevo_nodo = crear_nodo(actual, "Archivo", nombre_archivo);
        nuevo_nodo->contenido = nuevo_archivo;

        //Inserta nuevo nodo en la lista del directorio actual
        insertar_lista(((Directorio*)actual->contenido)->hijos, nuevo_nodo);
}

/*
Escribe la cadena 'contenido' dentro del archivo 'nombre_archivo', dentro del nodo actual 
    
    Parametros:
            actual (Nodo*): Puntero a nodo donde buscar el archivo
            nombre_archivo (char*): Cadena con nombre de archivo a escribir contenido
            contenido (char*): Cadena con el contenido a escribir
    
    Retorno:
            void : void
*/


void write(Nodo* actual, char* nombre_archivo, char* contenido){
        Directorio* directorio = (Directorio*)actual->contenido;

        //Caso en que la lista sea vacia
        if(directorio->hijos == NULL) printf("\n");
        else {
                Nodo* nodo = buscar_archivo(directorio, nombre_archivo);
                //Caso en que el archivo no se encuentre
                if(nodo == NULL){
                        printf("Archivo no existe\n");
                        return;
                }
                strcpy(((Archivo*)nodo->contenido)->contenido, contenido);
        }
}

/*
Imprime por pantalla el contenido del archivo 'nombre_archivo', dentro del nodo actual
    
    Parametros:
            actual (Nodo*): Puntero a nodo donde buscar el archivo
            nombre_archivo (char*): Cadena con nombre de archivo
    
    Retorno:
            void : void
*/

void cat(Nodo* actual, char* nombre_archivo){
        Directorio* directorio = (Directorio*)actual->contenido;

        //Caso en que Lista de directorio sea null
        if(directorio->hijos == NULL) printf("Directorio Vacio \n");
        else{
                Nodo* nodo = buscar_archivo(directorio, nombre_archivo);

                //Caso en que archivo no se encuentre
                if(nodo == NULL) {
                        printf("Archivo no existe\n");
                        return;
                }
                printf("%s\n", ((Archivo*)nodo->contenido)->contenido);
        }
}

/*
Imprime por pantalla los nodos dentro de nodo actual
    
    Parametros:
            actual (Nodo*): Puntero a nodo a revisar
    
    Retorno:
            void : void
*/


void ls(Nodo* actual) {
        Directorio* directorio = (Directorio*)actual->contenido;

        //Caso en que lista de directorio sea null
        if (directorio->hijos == NULL) {
                printf("\n");
        } else {
                for (int i = 0; i < directorio->hijos->largo_actual; i++) {
                        Nodo* nodo = directorio->hijos->arreglo[i];
                        //Se imprime con './' en caso de directorio
                        if(strcmp(nodo->tipo, "Archivo") == 0) printf("%s\n", ((Archivo*)nodo->contenido)->nombre);
                        else printf("./ %s\n", ((Directorio*)nodo->contenido)->nombre);
                }
        }
}

/*
Imprime por pantalla los nodos dentro de 'nombre_directorio'
    
    Parametros:
            actual (Nodo*): Puntero a nodo donde buscar directorio entregado
            nombre_directorio (char*): Cadena con nombre de directorio a mostrar contenido
    
    Retorno:
            void : void
*/


void ls_dir(Nodo* actual, char* nombre_directorio){
        //Si el parametro es '.' se comporta como ls normal
        if(strcmp(nombre_directorio, ".") == 0) ls(actual);
        else {
                Directorio* dir = (Directorio*)actual->contenido;

                //Caso de lista vacia
                if(dir->hijos == NULL) return;
                for (int i = 0; i < dir->hijos->largo_actual; i++) {
                        Nodo* nodo = dir->hijos->arreglo[i];

                        //Si encontramos el directorio 'nombre_directorio' hacemos un ls sobre el
                        //y salimos del for para terminar la funcion.
                        if (strcmp(((Directorio*)nodo->contenido)->nombre, nombre_directorio) == 0 && strcmp(nodo->tipo, "Directorio") == 0) {
                                ls(nodo);
                                break;
                        }
                } 
        }
}

/*
Ejecuta una instruccion 'instruccion' con parametro_instruccion dentro del nodo actual

    Parametros:
            actual (Nodo*): Puntero a nodo donde ejecutar instruccion
            instruccion (void* (Nodo*, char*)): funcion a ejecutar
            parametro_instruccion (char*): parametro a entregar a instruccion
    
    Retorno:
            void: void
*/

void mapdir(Nodo* actual, void (*instruccion) (Nodo*, char*), char* parametro_instruccion){
    Directorio* dir = (Directorio*) actual->contenido;
    //Buscamos en la lista los nodos que sean directorio
    for (int i = 0; i < dir->hijos->largo_actual; i++) {
                Nodo* hijo = (dir->hijos->arreglo[i]);
                //Una vez encontrados ejecutamos la instruccion entregada sobre ellos
                if (strcmp(hijo->tipo, "Directorio") == 0) {
                        instruccion(hijo, parametro_instruccion);
                }
    }
}

/*
Libera la memoria de la estructura Nodo

    Parametros:
            nodo (Nodo*): Puntero a nodo a liberar
*/

void freeNodo(Nodo* nodo) {
        if(nodo == NULL) return;
        if(strcmp(nodo->tipo, "Archivo") == 0) freeArchivo(nodo->contenido); // Free sobre el archivo
        if(strcmp(nodo->tipo, "Directorio") == 0) freeDirectorio(nodo->contenido); //Free sobre el directorio
        free(nodo);
}

/*
Libera la memoria de la estructura Lista

    Parametros:
            lista (Lista*): Puntero a lista a liberar
*/

void freeLista(Lista* lista) {
        if (lista == NULL) {
                return;
        }

        for (int i = 0; i < lista->largo_actual; i++) {
                freeNodo(lista->arreglo[i]); // Free sobre cada nodo de la lista
        }
        if(lista->arreglo != NULL) free(lista->arreglo);
        free(lista); // Free de la lista
}

/*
Libera la memoria de la estructura Directorio

    Parametros:
            directorio (Directorio*): Puntero a directorio a liberar
*/

void freeDirectorio(Directorio* directorio) {
        if (directorio == NULL) {
                return;
        }
        if(directorio->hijos != NULL) freeLista(directorio->hijos); // Free sobre la lista del directorio
        free(directorio); // Free sobre el directorio
}

/*
Libera la memoria de la estructura archivo

    Parametros:
            archivo (Archivo*): Puntero a archivo a liberar
*/

void freeArchivo(Archivo* archivo) {
        if (archivo == NULL) {
                return;
        }
        free(archivo); // Free sobre el archivo
}
