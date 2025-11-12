#ifndef TABLERO_H
#define TABLERO_H

#include <iostream>
#include <string>

const int tamaño=10;

enum Jugador {NINGUNO=0, BLANCAS, NEGRAS};

struct Pieza{
    Jugador dueño;
};

bool esCasillaOscura(int fila, int col);
void inicializarTablero(Pieza tablero[tamaño][tamaño]);
void imprimirTablero(Pieza tablero[tamaño][tamaño]);
bool moverPieza(Pieza tablero[tamaño][tamaño], int fr, int fc, int tr, int tc, Jugador turno, int &capturasBlancas, int &capturasNegras);
bool existeCaptura(Pieza tablero[tamaño][tamaño], Jugador turno);
Jugador cambiarTurno(Jugador actual);
std::string nombreJugador(Jugador j);

#endif