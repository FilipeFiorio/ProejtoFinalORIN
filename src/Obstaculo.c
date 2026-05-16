#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Obstaculo.h"

void desenharObstaculo(Obstaculo *obstaculo) {
    DrawRectangleRec(obstaculo->ret, obstaculo->cor);
}

void desenharObstaculos(Obstaculo *obstaculos, int quantidade) {

    for ( int i = 0; i < quantidade; i++) {
        desenharObstaculo(&obstaculos[i]);
    }
    
}