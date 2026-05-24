#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ObstaculoMovel.h"

ObstaculoMovel *criarObstaculoMovel(Rectangle ret, Rectangle fonte, Vector2 destino, Vector2 vel, Color cor, Texture2D *textura) {

    ObstaculoMovel *novoObstaculoMovel = (ObstaculoMovel*) malloc(sizeof(ObstaculoMovel));

    novoObstaculoMovel->ret = ret;
    novoObstaculoMovel->fonte = fonte;
    novoObstaculoMovel->destino = destino;
    novoObstaculoMovel->vel = vel;
    novoObstaculoMovel->cor = cor;
    novoObstaculoMovel->textura = textura;

    return novoObstaculoMovel;

}

void destruirObstaculoMovel(ObstaculoMovel *obstaculoMovel) {
    // Fazer dps
}

void desenharObstaculoMovel(ObstaculoMovel *obstaculoMovel) {

    if(obstaculoMovel->textura == NULL) {
        DrawRectangleRec(obstaculoMovel->ret, obstaculoMovel->cor);
        return;
    }

    DrawTexturePro(
        *obstaculoMovel->textura,
        obstaculoMovel->fonte,
        obstaculoMovel->ret,
        (Vector2) {0},
        0.0f,
        WHITE

    );

}

void atualizarObstaculoMovel(ObstaculoMovel *obstaculoMovel) {

}