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

Obstaculo *criarObstaculo(Rectangle ret, Rectangle fonte, Texture2D *textura, Color cor) {

    Obstaculo *novoObstaculo = (Obstaculo*) malloc(sizeof(Obstaculo));

    novoObstaculo->ret = ret;
    novoObstaculo->fonte = fonte;
    novoObstaculo->textura = textura;   
    novoObstaculo->cor = cor;

    return novoObstaculo;
}

void destruirObstaculo(Obstaculo *obstaculo) {

    free(obstaculo);
    
}