#include <iostream>

using namespace std;

// Declaración de un nodo tipo carta
struct NodeCartas {
    int rangoCartas; // Valores de las cartas (0=3, 1=4... 12=2, 13=joker)
    int paloCartas;  // 0=trebol, 1=diamante, 2=corazon, 3=espada, -1=joker
    NodeCartas *next;
};

// Declaración de la lista de las cartas
struct listCartas {
    NodeCartas *head; // Puntero a la primera carta
    int size;         // Tamaño de la mano
};

// Características de cada jugador
struct Jugador {
    listCartas mano;   // Mano del jugador
    bool turno;        // Si el jugador tuvo su turno
    char nombre[10];   // Nombre del jugador
};

// Primitivas adaptadas
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

// Variables globales
Jugador jugadores[4];
int njugadores = 4;
int jugadorActual = 0; // Indice del jugador actual

// Función para crear un mazo (lista enlazada)
void crear_mazo(listCartas *mazo) {
    mazo->head = NULL;
    mazo->size = 0;
    for (int palo = 0; palo < 4; ++palo) {
        for (int rango = 0; rango < 13; ++rango) {
            push(mazo, rango, palo);
        }
    }
    // Añadir los dos comodines
    push(mazo, 13, -1);
    push(mazo, 13, -1);
}

// Función para repartir las cartas (manualmente)
void repartir() {
    listCartas mazo_completo;
    crear_mazo(&mazo_completo);

    for (int i = 0; i < njugadores; ++i) {
        jugadores[i].mano.head = NULL;
        jugadores[i].mano.size = 0;
    }

    // Simplemente repartir en orden para este ejemplo sin barajar complejo
    int jugador_index = 0;
    while (mazo_completo.head != NULL) {
        NodeCartas* carta = pop(&mazo_completo);
        carta->next = jugadores[jugador_index].mano.head;
        jugadores[jugador_index].mano.head = carta;
        jugadores[jugador_index].mano.size++;
        jugador_index = (jugador_index + 1) % njugadores;
    }

    for (int i = 0; i < njugadores; ++i) {
        ordenarCartas(&jugadores[i]);
    }
}

// Función para intercambiar cartas (ordenar)
void intercambiar(NodeCartas *a, NodeCartas *b) {
    int auxRango = a->rangoCartas;
    int auxPalos = a->paloCartas;
    a->rangoCartas = b->rangoCartas;
    a->paloCartas = b->paloCartas;
    b->rangoCartas = auxRango;
    b->paloCartas = auxPalos;
}

// Función para ordenar las cartas
void ordenarCartas(Jugador *p) {
    if (p->mano.size <= 1) return;
    int i = 0;
    NodeCartas *cartaActual = p->mano.head;
    while (i < p->mano.size - 1) {
        NodeCartas *j_ptr = p->mano.head;
        for (int j = 0; j < p->mano.size - i - 1; ++j) {
            if (j_ptr->rangoCartas > j_ptr->next->rangoCartas) {
                intercambiar(j_ptr, j_ptr->next);
            }
            j_ptr = j_ptr->next;
        }
        i++;
    }
}

// Función para inicializar jugadores
void initJugadores() {
    char nombres[4][10] = {"Tu", "Jugador1", "Jugador2", "Jugador3"};
    for (int i = 0; i < njugadores; ++i) {
        for (int j = 0; j < 9; ++j) {
            jugadores[i].nombre[j] = nombres[i][j];
            if (nombres[i][j] == '\0') break;
        }
        jugadores[i].nombre[9] = '\0';
        jugadores[i].mano.head = NULL;
        jugadores[i].mano.size = 0;
        jugadores[i].turno = false;
    }
}

// Función para encontrar al primer jugador (3 de diamantes)
int primerJugador() {
    for (int i = 0; i < njugadores; ++i) {
        NodeCartas *aux = jugadores[i].mano.head;
        while (aux) {
            if (aux->rangoCartas == 0 && aux->paloCartas == 1) return i;
            aux = aux->next;
        }
    }
    return -1;
}

// Función para iniciar una ronda
void initRonda() {
    initJugadores();
    repartir();
    int primer = primerJugador();
    if (primer != -1) {
        jugadorActual = primer;
    }
}

// Función para obtener el valor jerárquico de una carta
int obtener_valor_jerarquico(int rango) {
    if (rango == 13) return 20; // Joker
    if (rango == 12) return 19; // 2
    if (rango == 11) return 18; // As
    if (rango >= 0 && rango <= 10) return rango; // 3 al K
    return -1;
}

// Función para comparar dos cartas por su valor jerárquico
bool es_mayor(int rango1, int rango2) {
    return obtener_valor_jerarquico(rango1) > obtener_valor_jerarquico(rango2);
}

// Función para mostrar una carta
void mostrar_carta(int rango) {
    if (rango >= 0 && rango <= 7) {
        cout << rango + 3;
    } else if (rango == 8) {
        cout << 'J';
    } else if (rango == 9) {
        cout << 'Q';
    } else if (rango == 10) {
        cout << 'K';
    } else if (rango == 11) {
        cout << 'A';
    } else if (rango == 12) {
        cout << '2';
    } else if (rango == 13) {
        cout << "Joker";
    } else {
        cout << "Inv";
    }
}

// Función para mostrar la mano completa de un jugador con palos
void mostrar_mano_completa(int jugador_index) {
    cout << jugadores[jugador_index].nombre << ": ";
    NodeCartas *actual = jugadores[jugador_index].mano.head;
    while (actual) {
        mostrar_carta(actual->rangoCartas);
        char palo_char;
        switch (actual->paloCartas) {
            case 0: palo_char = 'T'; break;
            case 1: palo_char = 'D'; break;
            case 2: palo_char = 'C'; break;
            case 3: palo_char = 'P'; break;
            case -1: palo_char = 'J'; break;
            default: palo_char = '?'; break;
        }
        cout << palo_char << (actual->next ? ", " : "");
        actual = actual->next;
    }
    cout << " (" << jugadores[jugador_index].mano.size << " cartas)" << endl;
}

int main() {
    initRonda();

    cout << "** Manos de los Jugadores **" << endl;
    for (int i = 0; i < njugadores; ++i) {
        mostrar_mano_completa(i);
    }
    cout << endl;

    cout << "** Turno de " << jugadores[jugadorActual].nombre << " **" << endl;
    cout << jugadores[jugadorActual].nombre << ", ¿qué quieres jugar? (single, par, trio, poker)" << endl;
    // Aquí iría la lógica para que el jugador seleccione qué jugar.

    return 0;
}