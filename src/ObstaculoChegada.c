#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ObstaculoChegada.h"


ObstaculoChegada *criarObstaculoChegada(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura) {

    ObstaculoChegada *novoObstaculoChegada = (ObstaculoChegada*) malloc(sizeof(ObstaculoChegada));

    novoObstaculoChegada->ret = ret;
    novoObstaculoChegada->fonte = fonte;
    novoObstaculoChegada->cor = cor;
    novoObstaculoChegada->textura = textura;
    novoObstaculoChegada->fonte = fonte;

    return novoObstaculoChegada;

}

void destruirObstaculoChegada(ObstaculoChegada *obstaculoChegada) {
    
    if(obstaculoChegada != NULL) {
        free(obstaculoChegada);
    }

}

void desenharObstaculoChegada(ObstaculoChegada *obstaculoChegada) {

    if(obstaculoChegada->textura == NULL) {
        DrawRectangleRec(obstaculoChegada->ret, obstaculoChegada->cor);
        return;
    };

    DrawTexturePro(
        *obstaculoChegada->textura,
        obstaculoChegada->fonte,
        obstaculoChegada->ret,
        (Vector2) {0},
        0.0f,
        WHITE
    );

}