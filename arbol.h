//Estructura del arbol para el uso de programa,
//predefinida por el enunciado de la tarea

typedef struct {
    struct Nodo* padre;
    char tipo[64];
    void* contenido;
}Nodo;

typedef struct {
    int largo_actual;
    int largo_maximo;
    Nodo** arreglo;
} Lista;

typedef struct {
    char nombre[128];
    Lista* hijos;
} Directorio;

typedef struct {
    char nombre[128];
    char contenido[256];
} Archivo;


// Aqui solo se declaran las funciones, se puede encontrar mayor descripcion y comentarios
// en el archivo 'arbol.c' donde estan definidas

Lista* crear_lista(int largo_maximo_inicial);
void insertar_lista(Lista* lista, Nodo* nodo);
Nodo* buscar_directorio(Directorio* actual, char* nombre);
Nodo* buscar_archivo(Directorio* actual, char* nombre);
Nodo* crear_nodo(Nodo* padre, char* tipo, char* nombre);
void mkdir(Nodo* actual, char* nombre_directorio);
Nodo* cd(Nodo* actual, char* nombre_directorio);
void touch(Nodo* actual, char* nombre_archivo);
void write(Nodo* actual, char* nombre_archivo, char* contenido);
void cat(Nodo* actual, char* nombre_archivo);
void ls(Nodo* actual);
void ls_dir(Nodo* actual, char* nombre_directorio);
void mapdir(Nodo* actual, void (*instruccion) (Nodo*, char*), char* parametro_instruccion);
void freeNodo(Nodo* nodo);
void freeLista(Lista* lista);
void freeDirectorio(Directorio* directorio);
void freeArchivo(Archivo* archivo);