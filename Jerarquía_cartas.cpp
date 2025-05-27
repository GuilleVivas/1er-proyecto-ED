#include <iostream>

using namespace std;

const char PICAS = 'P';
const char CORAZONES = 'C';
const char DIAMANTES = 'D';
const char TREBOLES = 'T';
const char JOKER = 'J';

struct Carta {
    int valor_numerico;
    char palo;

    Carta(int valor, char p) : valor_numerico(valor), palo(p) {}


    void mostrar_carta() {
        if (valor_numerico >= 3 && valor_numerico <= 10) {
            cout << valor_numerico;
        } else if (valor_numerico == 11) {
            cout << 'J';
        } else if (valor_numerico == 12) {
            cout << 'Q';
        } else if (valor_numerico == 13) {
            cout << 'K';
        } else if (valor_numerico == 14) {
            cout << 'A';
        } else if (valor_numerico == 15) {
            cout << '2';
        } else if (valor_numerico == 0 && palo == JOKER) {
            cout << "Joker";
        } else {
            cout << "Valor Inválido";
        }
        cout << palo;
    }
};

struct NodoCarta {
    Carta carta;
    NodoCarta* siguiente;

    NodoCarta(Carta c) : carta(c), siguiente(nullptr) {}
};

struct PilaCartas {
    NodoCarta* cima;

    PilaCartas() : cima(nullptr) {}

    bool isVoid() {
        return cima == nullptr;
    }

    void push(Carta carta) {
        NodoCarta* nuevoNodo = new NodoCarta(carta);
        nuevoNodo->siguiente = cima;
        cima = nuevoNodo;
    }

    Carta* top() {
        if (!isVoid()) {
            return &(cima->carta);
        }
        return nullptr;
    }

    Carta pop() {
        if (!isVoid()) {
            NodoCarta* temp = cima;
            Carta carta = cima->carta;
            cima = cima->siguiente;
            delete temp;
            return carta;
        }
        return Carta(-1, ' ');
    }

    void mostrar_pila() {
        NodoCarta* actual = cima;
        cout << "Mano: ";
        while (actual != nullptr) {
            actual->carta.mostrar_carta();
            if (actual->siguiente != nullptr) {
                cout << ", ";
            }
            actual = actual->siguiente;
        }
        cout << endl;
    }

    ~PilaCartas() {
        NodoCarta* actual = cima;
        while (actual != nullptr) {
            NodoCarta* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        cima = nullptr;
    }
};

int obtener_valor_jerarquico(int rango, int palo) {
    if (rango == 13) { // Joker
        return 20;
    }
    if (rango == 12) { // 2
        return 19;
    }
    if (rango == 11) { // As
        return 18;
    }
    // Para las cartas del 3 (rango 0) al Rey (rango 10), su valor jerárquico puede ser su rango + un offset
    // para que el 3 tenga el valor más bajo.
    if (rango >= 0 && rango <= 10) { // 3 al Rey
        return rango; // Podemos usar el rango directamente como valor jerárquico base
    }
    return -1; // Valor inválido
}

bool es_mayor(int rango1, int palo1, int rango2, int palo2) {
    return obtener_valor_jerarquico(rango1, palo1) > obtener_valor_jerarquico(rango2, palo2);
}

int main() {
    PilaCartas manoJugador1;
    manoJugador1.push(Carta(3, PICAS));
    manoJugador1.push(Carta(14, CORAZONES));
    manoJugador1.push(Carta(7, DIAMANTES));
    manoJugador1.push(Carta(0, JOKER));

    PilaCartas manoJugador2;
    manoJugador2.push(Carta(5, TREBOLES));
    manoJugador2.push(Carta(10, PICAS));
    manoJugador2.push(Carta(15, CORAZONES));
    manoJugador2.push(Carta(11, DIAMANTES));

    cout << "Mano del Jugador 1:" << endl;
    manoJugador1.mostrar_pila();
    cout << "Mano del Jugador 2:" << endl;
    manoJugador2.mostrar_pila();
    cout << endl;

    cout << "** Ronda de Juego **" << endl;

    cout << "Turno del Jugador 1:" << endl;
    Carta* carta_jugador1 = manoJugador1.top();
    if (carta_jugador1 != nullptr) {
        cout << "Jugador 1 juega: ";
        carta_jugador1->mostrar_carta();
        cout << endl;
    }

    cout << "Turno del Jugador 2:" << endl;
    Carta* carta_jugador2 = manoJugador2.top();
    if (carta_jugador2 != nullptr) {
        cout << "Jugador 2 juega: ";
        carta_jugador2->mostrar_carta();
        cout << endl;
    }
    cout << endl;

    if (!manoJugador1.isVoid() && !manoJugador2.isVoid()) {
        cout << "** Comparando cartas jugadas **" << endl;
        Carta* carta1 = manoJugador1.top();
        Carta* carta2 = manoJugador2.top();

        cout << "Carta de Jugador 1: ";
        carta1->mostrar_carta();
        cout << endl;
        cout << "Carta de Jugador 2: ";
        carta2->mostrar_carta();
        cout << endl;

        if (es_mayor(carta1->valor_numerico, carta1->palo, carta2->valor_numerico, carta2->palo)) {
            cout << "¡La carta del Jugador 1 es mayor!" << endl;
        } else if (es_mayor(carta2->valor_numerico, carta2->palo, carta1->valor_numerico, carta1->palo)) {
            cout << "¡La carta del Jugador 2 es mayor!" << endl;
        } else {
            cout << "¡Las cartas tienen el mismo valor jerárquico!" << endl;
        }
    } else {
        cout << "Uno de los jugadores no tiene cartas para jugar." << endl;
    }
    cout << endl;

    cout << "** Cambio de Turno **" << endl;
    cout << "El turno ahora es para el siguiente jugador (en este ejemplo, conceptualmente)." << endl;

    return 0;
}