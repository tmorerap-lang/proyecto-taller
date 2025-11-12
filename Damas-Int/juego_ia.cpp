#include "juego.h"
#include "tablero.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <limits>

void ejecutarJuegoIA() {
    std::srand((unsigned)std::time(nullptr));
    Pieza tablero[tamaño][tamaño];
    inicializarTablero(tablero);

    Jugador turno = BLANCAS; 
    int capturasBlancas = 0, capturasNegras = 0;
    bool salir = false;

    while (!salir) {
        imprimirTablero(tablero);
        std::cout << "\nTurno de " << nombreJugador(turno) << "\n";
        std::cout << "Capturas -> Blancas: " << capturasBlancas << "  Negras: " << capturasNegras << "\n";

        if (turno == BLANCAS) {
            int fi, ci, ff, cf;
            while (true) {
                std::cout << "Mover desde (fila columna) o 0 para salir: ";
                if (!(std::cin >> fi)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "Entrada inválida. Ingresa números.\n";
                    continue;
                }
                if (fi == 0) { salir = true; break; }
                std::cout << "Mover hacia (fila columna): ";
                if (!(std::cin >> ff)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "Entrada inválida. Ingresa números.\n";
                    continue;
                }
                if (!(std::cin >> ci >> cf)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "Entrada inválida. Formato: fi ci ff cf\n";
                    continue;
                }
                if (fi < 1 || fi > tamaño || ci < 1 || ci > tamaño || ff < 1 || ff > tamaño || cf < 1 || cf > tamaño) {
                    std::cout << "Coordenadas fuera de rango. Deben estar entre 1 y " << tamaño << ".\n";
                    continue;
                }
                bool capturaOb = existeCaptura(tablero, turno);
                int dr = ff - fi;
                int dc = cf - ci;
                if (capturaOb && (std::abs(dr) != 2 || std::abs(dc) != 2)) {
                    std::cout << "Hay captura obligatoria: debes realizar un salto.\n";
                    continue;
                }
                if (!moverPieza(tablero, fi, ci, ff, cf, turno, capturasBlancas, capturasNegras)) {
                    std::cout << "Movimiento inválido. Intenta otra vez.\n";
                    continue;
                }
                break;
            }
            if (salir) break;
        } else {
            std::vector<std::tuple<int,int,int,int,bool>> opciones; 
            for (int f = 1; f <= tamaño; ++f) {
                for (int c = 1; c <= tamaño; ++c) {
                    if (tablero[f-1][c-1].dueño != NEGRAS) continue;
                    for (int tr = 1; tr <= tamaño; ++tr) {
                        for (int tc = 1; tc <= tamaño; ++tc) {
                            // prueba en copia
                            Pieza copia[tamaño][tamaño];
                            for (int i=0;i<tamaño;++i) for (int j=0;j<tamaño;++j) copia[i][j]=tablero[i][j];
                            int capB = 0, capN = 0;
                            if (moverPieza(copia, f, c, tr, tc, NEGRAS, capB, capN)) {
                                bool isCap = (capN > 0);
                                opciones.emplace_back(f,c,tr,tc,isCap);
                            }
                        }
                    }
                }
            }

            if (opciones.empty()) {
                std::cout << "IA no tiene movimientos válidos. Fin de la partida.\n";
                break;
            }

            std::vector<std::tuple<int,int,int,int,bool>> caps;
            for (auto &t : opciones) if (std::get<4>(t)) caps.push_back(t);

            auto &pool = (caps.empty() ? opciones : caps);
            int idx = std::rand() % pool.size();
            int fi = std::get<0>(pool[idx]);
            int ci = std::get<1>(pool[idx]);
            int ff = std::get<2>(pool[idx]);
            int cf = std::get<3>(pool[idx]);

            if (moverPieza(tablero, fi, ci, ff, cf, NEGRAS, capturasBlancas, capturasNegras)) {
                std::cout << "IA mueve de (" << fi << "," << ci << ") a (" << ff << "," << cf << ")\n";
            } else {
                std::cout << "IA intentó mover y falló. Saltando turno.\n";
            }
        }

        turno = cambiarTurno(turno);
    }

    std::cout << "\nPartida finalizada. Capturas totales -> Blancas: " << capturasBlancas << "  Negras: " << capturasNegras << "\n";
    std::cout << "Presione Enter para volver al menú...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}