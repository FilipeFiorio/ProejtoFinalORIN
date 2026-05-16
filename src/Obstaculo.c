#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Obstaculo.h"


void desenharObstaculo(Obstaculo *obstaculo) {
    
    if(obstaculo->textura == NULL) {
        DrawRectangleRec(obstaculo->ret, obstaculo->cor);
        return;
    }

    DrawTexturePro(
        *obstaculo->textura,
        obstaculo->fonte,
        obstaculo->ret,
        (Vector2) {0},
        0.0f,
        WHITE
    );
}

void destruirObstaculo(Obstaculo *obstaculo) {

    free(obstaculo);
    
}