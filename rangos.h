#include <iostream> 
#include "rondas.h"
//elaborado por Eduardo Rodriguez

using namespace std;

// sumar puntos a un jugador
void sumarPuntos(Jugador& jugador, int puntosASumar) {
    if (puntosASumar > 0) {
        jugador.puntos_acumulados += puntosASumar;
        cout << jugador.nombre << " ha ganado " << puntosASumar << " puntos. Total: " << jugador.puntos_acumulados << endl;
    } else {
        cout << "No se pueden sumar puntos negativos." << endl;
    }
}

//funcion para copiar cada rango
void copiarRango(char destino[10], const char origen[10]) {
    int i = 0;
    while (i < 9) {
        destino[i] = origen[i];
        if (origen[i] == '\0') break;
        i++;
    }
    destino[i] = '\0';
}

//funcion para asignar los rangos 
void asignarRango(struct Jugador* jugador, int puntosPorRondaActual) {
    char rangoMagnate[10] = "Magnate";
    char rangoRico[10] = "Rico";
    char rangoPobre[10] = "Pobre";
    char rangoMendigo[10] = "Mendigo";
    int puntosMagnate = 30;
    int puntosRico = 20;
    int puntosPobre = 10;
    if (jugador->puntos_acumulados >= puntosMagnate) {
        copiarRango(jugador->rango, rangoMagnate);
    } else if (jugador->puntos_acumulados >= puntosRico) {
        copiarRango(jugador->rango, rangoRico);
    } else if (jugador->puntos_acumulados >= puntosPobre) {
        copiarRango(jugador->rango, rangoPobre);
    } else {
        copiarRango(jugador->rango, rangoMendigo);
    }
}

void mostrarEstadoJugador(const Jugador &jugador) {
    cout << "Jugador: " << jugador.nombre
                << ", Puntos: " << jugador.puntos_acumulados
                << ", Rango: " << jugador.rango << endl;
    }
