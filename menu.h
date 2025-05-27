#include <iostream>
using namespace std;
//elaborado por Eduardo Rodriguez

void limpiarBufferEntrada() {
    cin.ignore(256, '\n');
}

int obtenerOpcionMenu(int min, int max) {
    int opcion;
    while (true) {
        cout << "Selecciona una opcion: ";
        if (cin >> opcion) {
            if (opcion >= min && opcion <= max) {
                limpiarBufferEntrada();
                return opcion;
            } else {
                cout << "Opcion invalida. Por favor, ingresa un numero entre " << min << " y " << max << "." << endl;
            }
        } else {
            cout << "Entrada invalida. Por favor, ingresa un numero." << endl;
            cin.clear();
            limpiarBufferEntrada();
        }
    }
}

int main() {
    int op = 0;

    cout << "\nBienvenido a MAGNATE" << endl;

    while (op != 3) {
        cout << "\n____Menu del juego____" << endl;
        cout << "1. Empezar a jugar" << endl;
        cout << "2. Informacion del juego" << endl;
        cout << "3. Salir" << endl;
        cout << "______________________" << endl;

        op = obtenerOpcionMenu(1, 3);

        if (op == 1) {
            cout << "El juego comenzara en breve..." << endl;
            cout << "Ahora si comencemos... (Aqui iria la logica del juego)" << endl;
            cout << "\nPresiona ENTER para volver al menu...";
            cin.get();
        } else if (op == 2) {
            cout << "Como jugar MAGNATE ?" << endl;
            cout << "El juego es medio confuso, aqui te dejo un video: https://youtu.be/JtS65buhDPU" << endl;
            cout << "\nPresiona ENTER para volver al menu...";
            cin.get();
        } else if (op == 3) {
            cout << "Gracias por jugar, hasta luego..." << endl;
        } else {
            cout << "Opcion no reconocida." << endl;
        }
    }

    return 0;
}
