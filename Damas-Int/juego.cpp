#include "juego.h"
#include <iostream>
#include <limits>

void mostrarReglas() {
    std::cout << "\n=== REGLAS DEL JUEGO - DAMAS INTERNACIONALES ===\n";
    std::cout << "1. El tablero es de " << tamaño << "x" << tamaño << " y se juegan en casillas oscuras.\n";
    std::cout << "2. Las blancas se colocan en las 4 filas inferiores, las negras en las 4 filas superiores.\n";
    std::cout << "3. Las piezas se mueven diagonalmente una casilla hacia adelante (sin promover).\n";
    std::cout << "4. Para capturar se salta una pieza enemiga en diagonal (salto de 2 casillas).\n";
    std::cout << "5. No se permite mover hacia atrás hasta llegar a la última fila del oponente (promoción).\n";
    std::cout << "6. Al capturar, la pieza enemiga se elimina del tablero.\n";
    std::cout << "7. Gana el jugador que quede sin piezas el contrario o que lo deje sin movimientos válidos.\n";
    std::cout << "\nPresione Enter para volver al menú principal...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void ejecutarJuego() {
    Pieza tablero[tamaño][tamaño];
    Jugador turno = BLANCAS;
    bool salir = false;

    inicializarTablero(tablero);

    int capturasBlancas = 0;
    int capturasNegras = 0;

    while (!salir) {
        imprimirTablero(tablero);
        std::cout << "\nTurno de " << nombreJugador(turno) << "\n";
        std::cout << "Capturas -> Blancas: " << capturasBlancas << "  Negras: " << capturasNegras << "\n";

        int fi, ff, ci, cf;
        bool capturaObligatoria = existeCaptura(tablero, turno);

        while (true) {
            std::cout << "Mover desde (fila columna): ";
            if (!(std::cin >> fi >> ci)) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "Entrada inválida. Ingresa dos números enteros.\n";
                continue;
            }
            if (fi < 1 || fi > tamaño || ci < 1 || ci > tamaño) {
                std::cout << "Coordenadas fuera de rango. Deben estar entre 1 y " << tamaño << ".\n";
                continue;
            }

            std::cout << "Mover hacia (fila columna): ";
            if (!(std::cin >> ff >> cf)) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "Entrada inválida. Ingresa dos números enteros.\n";
                continue;
            }
            if (ff < 1 || ff > tamaño || cf < 1 || cf > tamaño) {
                std::cout << "Coordenadas fuera de rango. Deben estar entre 1 y " << tamaño << ".\n";
                continue;
            }

            int dr = ff - fi;
            int dc = cf - ci;
            int adr = std::abs(dr);
            int adc = std::abs(dc);

            if (capturaObligatoria && !(adr == 2 && adc == 2)) {
                std::cout << "Captura obligatoria: debes mover de forma que captures una pieza.\n";
                continue;
            }

            if (moverPieza(tablero, fi, ci, ff, cf, turno, capturasBlancas, capturasNegras)) {
                turno = cambiarTurno(turno);
                break; 
            } else {
                std::cout << "Movimiento inválido. Intente de nuevo.\n";
                capturaObligatoria = existeCaptura(tablero, turno);
                continue;
            }
        }

        int opcion;
        while (true) {
            std::cout << "\n--- Menú de partida ---\n";
            std::cout << "1. Continuar\n";
            std::cout << "2. Reiniciar\n";
            std::cout << "0. Salir al menú principal\n";
            std::cout << "Opción: ";
            if (!(std::cin >> opcion)) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "Entrada inválida. Ingresa un número (0,1,2).\n";
                continue;
            }
            if (opcion < 0 || opcion > 2) {
                std::cout << "Opción no válida. Elige 0, 1 o 2.\n";
                continue;
            }
            break;
        }

        if (opcion == 2) {
            while (true) {
                std::string resp;
                std::cout << "¿Seguro que desea reiniciar la partida? (s/n): ";
                if (!(std::cin >> resp)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "Entrada inválida. Responda 's' o 'n'.\n";
                    continue;
                }
                if (resp.size() == 0) continue;
                char conf = resp[0];
                if (conf == 's' || conf == 'S') {
                    inicializarTablero(tablero);
                    turno = BLANCAS;
                    std::cout << "\nPartida reiniciada.\n";
                    break;
                } else if (conf == 'n' || conf == 'N') {
                    std::cout << "Reinicio cancelado. Continúa la partida.\n";
                    break;
                } else {
                    std::cout << "Respuesta inválida. Use 's' o 'n'.\n";
                }
            }
        } else if (opcion == 0) {
            salir = true;
        }
    }
    std::cout << "\nResumen de la partida:\n";
    std::cout << "Fichas comidas por BLANCAS: " << capturasBlancas << "\n";
    std::cout << "Fichas comidas por NEGRAS: " << capturasNegras << "\n";
} 

void aplicacionJuego() { 
    int opcion = -1;

    while (opcion != 0) {
        std::cout << "\n== DAMAS INTERNACIONALES ==\n";
        std::cout << "1. Iniciar partida (2 jugadores)\n";
        std::cout << "2. Reglas\n";
        std::cout << "3. Jugar vs IA (aleatoria, prioriza capturas)\n";
        std::cout << "0. Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                ejecutarJuego();
                break;
            case 2:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                mostrarReglas();
                break;
            case 3:
                ejecutarJuegoIA();
                break;
            case 0:
                std::cout << "Saliendo del juego...\n";
                break;
            default:
                std::cout << "Opción inválida.\n";
            }
        }
    }