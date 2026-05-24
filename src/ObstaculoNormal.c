#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ObstaculoNormal.h"


ObstaculoNormal *criarObstaculoNormal(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura) {

    ObstaculoNormal *novoObstaculoNormal = (ObstaculoNormal*) malloc(sizeof(ObstaculoNormal));

    novoObstaculoNormal->ret = ret;
    novoObstaculoNormal->fonte = fonte;
    novoObstaculoNormal->cor = cor;
    novoObstaculoNormal->textura = textura;
    novoObstaculoNormal->fonte;

    return novoObstaculoNormal;

}

void destruirObstaculoNormal(ObstaculoNormal *obstaculoNormal) {
    // Fazer dps
}

void desenharObstaculoNormal(ObstaculoNormal *obstaculoNormal) {

    if(obstaculoNormal->textura == NULL) {
        DrawRectangleRec(obstaculoNormal->ret, obstaculoNormal->cor);
        return;
    };

    DrawTexturePro(
        *obstaculoNormal->textura,
        obstaculoNormal->fonte,
        obstaculoNormal->ret,
        (Vector2) {0},
        0.0f,
        WHITE
    );

}