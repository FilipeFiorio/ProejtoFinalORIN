#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "ObstaculoMovel.h"

ObstaculoMovel *criarObstaculoMovel(Rectangle ret, Rectangle fonte, Vector2 deslocamento, Vector2 vel, Color cor, Texture2D *textura) {

    ObstaculoMovel *novoObstaculoMovel = (ObstaculoMovel*) malloc(sizeof(ObstaculoMovel));

    novoObstaculoMovel->ret = ret;
    novoObstaculoMovel->fonte = fonte;
    novoObstaculoMovel->deslocamento = deslocamento;
    novoObstaculoMovel->vel = vel;
    novoObstaculoMovel->cor = cor;
    novoObstaculoMovel->textura = textura;

    novoObstaculoMovel->posInicial = (Vector2) {novoObstaculoMovel->ret.x, novoObstaculoMovel->ret.y};

    novoObstaculoMovel->retornando = false;

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

void atualizarObstaculoMovel(ObstaculoMovel *obstaculoMovel, GameWorld *gw, float delta) {

    Vector2 alvo;

    if(!obstaculoMovel->retornando) {
        alvo = (Vector2) {
            obstaculoMovel->posInicial.x + obstaculoMovel->deslocamento.x,
            obstaculoMovel->posInicial.y + obstaculoMovel->deslocamento.y
        };
    } else {
        alvo = obstaculoMovel->posInicial;
    }

    if(obstaculoMovel->deslocamento.x != 0) {
        if(!obstaculoMovel->retornando) {
            obstaculoMovel->ret.x += obstaculoMovel->vel.x * delta;

            if(obstaculoMovel->ret.x >= alvo.x) { 
                obstaculoMovel->ret.x = alvo.x; 
                obstaculoMovel->retornando = true; 
            }
        } else {
            obstaculoMovel->ret.x -= obstaculoMovel->vel.x * delta;

            if(obstaculoMovel->ret.x <= alvo.x) { 
                obstaculoMovel->ret.x = alvo.x; 
                obstaculoMovel->retornando = false; 
            }
        }
    }

    if(obstaculoMovel->deslocamento.y != 0) {
        if(!obstaculoMovel->retornando) {
            obstaculoMovel->ret.y += obstaculoMovel->vel.y * delta;

            if(obstaculoMovel->ret.y >= alvo.y) { 
                obstaculoMovel->ret.y = alvo.y; 
                obstaculoMovel->retornando = true; 
            }
        } else {
            obstaculoMovel->ret.y -= obstaculoMovel->vel.y * delta;
            
            if(obstaculoMovel->ret.y <= alvo.y) { 
                obstaculoMovel->ret.y = alvo.y; 
                obstaculoMovel->retornando = false; 
            }
        }
    }
}