#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Inimigo.h"

static void resolverColisaoinimigoMapaX(Inimigo *i, Mapa *m);
static void resolverColisaoInimigoMapaY( Inimigo *i, Mapa *m);

Inimigo *criarInimigo(float x, float y, float largura, float altura, Color cor) {

    Inimigo *novoInimigo = (Inimigo*) malloc(sizeof(Inimigo));

    novoInimigo->ret.x = x;
    novoInimigo->ret.y = y;
    novoInimigo->ret.width = largura;
    novoInimigo->ret.height = altura;

    novoInimigo->vel.x = 150;
    novoInimigo->vel.y = 0;
    novoInimigo->velMaxQueda = 600;
    
    novoInimigo->cor = cor;

    novoInimigo->estaVivo = true;

    return novoInimigo;

}

void atualizarInimigo(Inimigo *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {
        inimigo->ret.x += inimigo->vel.x * delta;
        resolverColisaoinimigoMapaX(inimigo, gw->mapa);

        inimigo->vel.y += gw->gravidade * delta;

        inimigo->ret.y += inimigo->vel.y * delta;
        resolverColisaoInimigoMapaY(inimigo, gw->mapa);
    }

}

void destruirInimigo(Inimigo *inimigo) {

    if(inimigo != NULL) {
        free(inimigo);
        inimigo = NULL;
    }
    
}

void desenharInimigo(Inimigo *inimigo) {

    if(inimigo->estaVivo) {
        DrawRectangleRec(inimigo->ret, inimigo->cor);
    }

}

// colisao no eixo x iguais ao jogador, porem ao colidir num obstáculo inverte a direção
static void resolverColisaoinimigoMapaX(Inimigo *i, Mapa *m) {

    ElementoMapa *el = m->obstaculos;

    while(el != NULL) {

        Obstaculo *o = (Obstaculo*) el->objeto;

        if(CheckCollisionRecs(i->ret, o->ret)) {
            if(i->ret.x + i->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                i->ret.x = o->ret.x - i->ret.width;
            } else {
                i->ret.x = o->ret.x + o->ret.width;
            }
                
            i->vel.x = -i->vel.x;
            
        }
        

        el = el->proximo;

    }


}

// todo: verificar se há chao para continuar andando, se nao houver inverter direção
static void resolverColisaoInimigoMapaY( Inimigo *i, Mapa *m ) {

    ElementoMapa *el = m->obstaculos;

    while ( el != NULL ) {

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( i->ret, o->ret ) ) {
            if ( i->ret.y + i->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                i->ret.y = o->ret.y - i->ret.height;
            } else { //não precisa
                i->ret.y = o->ret.y + o->ret.height;
            }
            i->vel.y = 0;
        }

        el = el->proximo;

    }

}