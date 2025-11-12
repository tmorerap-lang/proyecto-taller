#include "tablero.h"
#include <cmath>
#include <iostream>

bool esCasillaOscura(int fila, int col) {
    return (fila + col) % 2 != 0;
}

void inicializarTablero(Pieza tablero[tamaño][tamaño]) {
    for (int f = 0; f < tamaño; f++) {
        for (int c = 0; c < tamaño; c++) {
            tablero[f][c].dueño = NINGUNO;
        }
    }

    for (int f = 0; f < 4; f++)
        for (int c = 0; c < tamaño; c++)
            if (esCasillaOscura(f, c))
                tablero[f][c].dueño = NEGRAS;

    for (int f = tamaño - 4; f < tamaño; f++)
        for (int c = 0; c < tamaño; c++)
            if (esCasillaOscura(f, c))
                tablero[f][c].dueño = BLANCAS;
}

void imprimirTablero(Pieza tablero[tamaño][tamaño]) {
    std::cout << "\n";

    for (int f = 0; f < tamaño; f++) {
        if(f+1 == 10) std::cout<<f+1<<"|";
        else std::cout << f + 1 << " |";

        for (int c = 0; c < tamaño; c++) {
            if (tablero[f][c].dueño == BLANCAS) std::cout << "B ";
            else if (tablero[f][c].dueño == NEGRAS) std::cout << "N ";
            else if(esCasillaOscura(f,c) == false)std::cout << "□ ";
            else std::cout<<". ";
        }
        std::cout << "|\n";
    }
    std::cout<<"   ";
    for (int c = 0; c < tamaño; c++) std::cout<< c + 1 << " ";

}

bool moverPieza( Pieza tablero[tamaño][tamaño], int fr, int fc, int tr, int tc, Jugador turno, int &capturasBlancas, int &capturasNegras) {
    fr--; fc--; tr--; tc--; // convertir a índice [0]
    if (fr < 0 || fr >= tamaño || fc < 0 || fc >= tamaño) return false;
    if (tr < 0 || tr >= tamaño || tc < 0 || tc >= tamaño) return false;

    if (tablero[fr][fc].dueño != turno) return false;
    if (tablero[tr][tc].dueño != NINGUNO) return false;
    if (!esCasillaOscura(tr, tc)) return false;

    int dr = tr - fr;
    int dc = tc - fc;
    int adr = std::abs(dr);
    int adc = std::abs(dc);

    bool puedeMoverAtras = false;
    if ((turno == BLANCAS && fr == 0) || (turno == NEGRAS && fr == tamaño - 1)) {
        puedeMoverAtras = true;
    }

    if (!puedeMoverAtras) {
        if (turno == BLANCAS && dr >= 0) return false; 
        if (turno == NEGRAS && dr <= 0) return false; 
    }

    if (adr == 1 && adc == 1) {
        tablero[tr][tc] = tablero[fr][fc];
        tablero[fr][fc].dueño = NINGUNO;
        return true;
    }

    if (adr == 2 && adc == 2) {
        int mr = fr + dr / 2; 
        int mc = fc + dc / 2; 
        if (mr < 0 || mr >= tamaño || mc < 0 || mc >= tamaño) return false;
        if (tablero[mr][mc].dueño != NINGUNO && tablero[mr][mc].dueño != turno) {
            tablero[tr][tc] = tablero[fr][fc];
            tablero[fr][fc].dueño = NINGUNO;
            tablero[mr][mc].dueño = NINGUNO;
            if (turno == BLANCAS) capturasBlancas++;
            else if (turno == NEGRAS) capturasNegras++;
            return true;
        }
    }

    return false;
}

bool existeCaptura(Pieza tablero[tamaño][tamaño], Jugador turno) {
    for (int f = 0; f < tamaño; ++f) {
        for (int c = 0; c < tamaño; ++c) {
            if (tablero[f][c].dueño != turno) continue;
            const int drs[4] = {2, 2, -2, -2};
            const int dcs[4] = {2, -2, 2, -2};
            for (int k = 0; k < 4; ++k) {
                int tr = f + drs[k];
                int tc = c + dcs[k];
                int mr = f + drs[k] / 2;
                int mc = c + dcs[k] / 2;
                if (tr < 0 || tr >= tamaño || tc < 0 || tc >= tamaño) continue;
                if (mr < 0 || mr >= tamaño || mc < 0 || mc >= tamaño) continue;
                if (!esCasillaOscura(tr, tc)) continue;
                if (tablero[tr][tc].dueño != NINGUNO) continue;
                if (tablero[mr][mc].dueño != NINGUNO && tablero[mr][mc].dueño != turno) {
                    return true;
                }
            }
        }
    }
    return false;
}


Jugador cambiarTurno(Jugador actual) {
    return (actual == BLANCAS) ? NEGRAS : BLANCAS;
}

std::string nombreJugador(Jugador j) {
    if (j == BLANCAS) return "BLANCAS";
    if (j == NEGRAS) return "NEGRAS";
    return "NINGUNO";
}