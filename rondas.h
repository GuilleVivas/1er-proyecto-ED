#include <iostream>
using namespace std;

//declaracion de un nodo tipo carta
struct NodeCartas {
    int rangoCartas; //valores de las cartas (del 0 al 12, siendo 0 = 3, 1 = 4... 12 = 2, 13 = joker)
    int paloCartas; //0 = trebol, 1 = diamante, 2 = corazon, 3 = espada
    NodeCartas *next;
};

//declaracion de la lista de la cartas a parte para facilitar la cuenta de las cartas
struct listCartas {
    NodeCartas *head; //puntero a la primera carta del mazo
    int size; //para guardar el tamano de la mano de los jugadores
};

//caracteristicas de cada jugador
struct Jugador {
    listCartas mano; // mano del jugador
    bool turno; //saber si el jugador tuvo su turno
    char nombre[10]; //arreglo para guardar el nombre de los jugadores
    char rango[10]; //arreglo para guardar el rango del jugador
    int puntos_acumulados; //puntos del jugador
};

//primitivas adaptadas a la logica del juego
void push(listCartas *p, int rangoCartas, int paloCartas) {
    NodeCartas *newElement = new NodeCartas;
    newElement->rangoCartas = rangoCartas;
    newElement->paloCartas = paloCartas;
    newElement->next = p->head;
    p->head = newElement;
    p->size++;
}

NodeCartas* pop(listCartas *p) {
    if (!p || !(p->head)) return NULL;
    NodeCartas *aux = p->head;
    p->head = p->head->next;
    p->size--;
    aux->next = NULL;
    return aux;
}

//variables
listCartas mazo; //declaracion del mazo
Jugador jugadores[4]; //maximo de jugadores
NodeCartas *ultCarta = NULL; //ultima carta jugada
int jugadorActual = 0; //indice del jugador actual (para saber de quien es el turno)
int njugadores = 4; //numero total de jugadores

//funcion para intercambiar las cartas (se usara para ordenarlas)
void intercambiar (NodeCartas *a, NodeCartas *b){
    int auxRango = a -> rangoCartas; //auxiliares para no perder los valores del rango y palo de la primera carta
    int auxPalos = a -> paloCartas;
    a -> rangoCartas = b -> rangoCartas; 
    a -> paloCartas = b -> paloCartas;
    b -> rangoCartas = auxRango; //intercambio de los valores de las cartas
    b -> paloCartas = auxPalos;
}

//funcion para ordenar las cartas en las manos de los jugadores
void ordenarCartas (Jugador *p){
    if (p -> mano.size <= 1) return; //dejar la mano tal cual si el jugador no tiene cartas o tiene 1 sola
    int i = 0; //indice general
    NodeCartas *cartaActual; //punteros para saber cual es la carta apuntada y cual es la siguiente
    NodeCartas *sigCarta;
    while (i < p -> mano.size -1){ //para que el ciclo se repita hasta que todas las cartas esten ordenadas
        cartaActual = p -> mano.head; //la carta actual es la primera de la mano
        sigCarta = cartaActual -> next;
        int j = 0; //indice para recorrer la lista
        while (j < p -> mano.size - i - 1 && sigCarta != NULL){ //comparar cartas adyacentes
            if (cartaActual -> rangoCartas > sigCarta -> rangoCartas) intercambiar(cartaActual,sigCarta); //intercambiar si la carta tiene mayor rango que la que le sigue
            cartaActual = sigCarta;
            sigCarta = sigCarta -> next;
            j++;
        }
        i ++;
    }
}

//funcion para inicializar un mazo
void crear_mazo(){
    mazo.head = NULL;
    mazo.size = 0; //inicializamos el mazo en 0 para que este vacio
    int palo = 0;
    while (palo < 4){ //sabemos que el palo puede ser 0, 1, 2 y 3
        int rango = 0; //reiniciamos el rango de las cartas
        while (rango < 13){ //el rango va de 0 a 12
            push(&mazo, rango, palo);
            rango ++;
        }
        push(&mazo, 13, -1); //agregar los jokers COMMIT
        palo ++;
    }
}

//funcion para repartir cartas
void repartir() {
    int i = 0;
    //reiniciamos las manos de los jugadores para evitar usar cartas de rondas anteriores
    while (i < njugadores) {
        jugadores[i].mano.head = NULL;
        jugadores[i].mano.size = 0;
        i++;
    }
    i = 0;
    //empezamos a repartir las cartas
    while (mazo.head != NULL) {
        NodeCartas* carta = pop(&mazo);
        carta->next = jugadores[i].mano.head; //apuntamos la siguiente carta a la cabeza de la lista
        jugadores[i].mano.head = carta;
        jugadores[i].mano.size++; //incrementamos el tamano de la mano
        i++;
        if (i >= njugadores) i = 0; //volvemos al primer jugador al terminar de repartirles a todos
    }
    i = 0;
    //ordenamos las cartas de los jugadores
    while (i < njugadores) {
        ordenarCartas(&jugadores[i]);
        i++;
    }
}

//funcion que guarda los nombres de los jugadores y reinicia sus manos y tunos
void initJugadores() {
    char nombres[4][10] = {"Tu", "Jugador1", "Jugador2", "Jugador3"}; //arreglo de 4 arreglos de hasta 10 caracteres
    int i = 0; //indice para recorrer a todos los jugadores
    while (i < njugadores) {
        int j = 0; //indice que recorre c/letra del nombre
        while (j < 9) { //debe sobrar 1 caracter para poner el caracter nulo (\0)
            jugadores[i].nombre[j] = nombres[i][j]; //con esto copiamos caracter por caracter al arreglo de los nombres
            if (nombres[i][j] == '\0') break; //rompemos el ciclo cuando llegue al final del nombre
            j++;
        }
        jugadores[i].nombre[j] = '\0'; //aseguramos que el nombre termino
        // reiniciamos la mano del jugador  
        jugadores[i].mano.head = NULL;
        jugadores[i].mano.size = 0;
        jugadores[i].turno = false;
        i++; //pasamos al sig jugador
    }
}

//funcion para encontrar al jugador que empezara la primera ronda
int primerJugador(){
    int i = 0; //indice para recorrer a los jugadors
    int found = -1; //inicia en -1 (no encontrado) por defecto, es la variable que guardara el indice del jugador que tiene al 3 de diamantes
    NodeCartas *aux; //auxiliar para recorrer la mano de los jugadores
    while (i < njugadores && found == -1){ //ciclo que se repite mientras no se haya encontrado el 3 de diamantes (recorriendo a cada jugador)
        aux = jugadores[i].mano.head;
        while (aux && found == -1){ //recorre la mano de c/jugador (individualmente) mientras no se haya encontrado el 3 de diamantes
            if (aux -> rangoCartas == 0 && aux -> paloCartas == 1) found = i; //se encontro al jugador que tiene el 3 de diamantes (rango 0 y palo 1)
            aux = aux -> next; //si no se encuentra apunta a la sig carta
        }   
        i ++; //pasamos al sig jugador si no encuentra el 3 de diamantes
    }
    return found; //devuelve el indice del jugador que tiene el 3 de diamantes
}

//funcion para comparar el rango de los jugadores
int compararRangos (char *a, char *b){ //dos punteros a los arreglos (donde estaran los rangos de los jugadores)
    int i = 0; //indice para recorrer los arreglos 
    while (a[i] != '\0' && b[i] != '\0'){ //mientras ambos caracteres sean diferentes del caracter nulo
        if (a[i] != b[i]) return 0; //si hay alguna diferencia entonces son rangos diferentes
        i ++; //avanzamos el indice para pasar al siguiente caracter
    }
    return a[i] == b[i]; //devuelve 1 cuando son iguales
}

//funcion para encontrar al mendigo para que inicie la sig ronda
int encontrarMendigo() {
    char rangoMendigo[10] = "Mendigo";
    for (int i = 0; i < njugadores; i++) {
        if (compararRangos(jugadores[i].rango, rangoMendigo)) {
            return i; // devuelve el índice del jugador mendigo
        }
    }
    return -1; // si no se encuentra
}


//funcion para iniciar una ronda
void initRonda(){
    initJugadores();
    crear_mazo();
    repartir();


}