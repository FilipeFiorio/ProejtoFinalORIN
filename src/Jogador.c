#include <stdlib.h>
#include "raylib/raylib.h"
#include "Jogador.h"
#include "Tipos.h"
#include "Inimigo.h"
#include "Mapa.h"

static void resolverColisaoJogadorMapaX( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorMapaY( Jogador *j, Mapa *mapa );

Jogador *criarJogador(float x, float y, float largura, float altura, Color cor) {

    Jogador *novoJogador = (Jogador*) malloc(sizeof(Jogador));

    novoJogador->ret = (Rectangle) {
        .x = x,
        .y = y,
        .width = largura,
        .height = altura,
    };
    novoJogador->vel = (Vector2) {0};
    novoJogador->cor = BLUE;

    novoJogador->vidas = 5;
    novoJogador->moedas = 0;

    novoJogador->velAndando = 225;
    novoJogador->velCorrendo = 325;
    novoJogador->velPulo = -425;
    novoJogador->velPuloCorrendo = -525;
    novoJogador->velMaxQueda = 600;

    return novoJogador;
}

void destruirJogador(Jogador *j) {
    if(j != NULL) {
        free(j);
    }
}

void entradaJogador(Jogador *j) {
    
    bool shiftApertado = IsKeyDown(KEY_LEFT_SHIFT);

    if(IsKeyDown(KEY_A)) {
        j->vel.x = shiftApertado? -j->velCorrendo : -j->velAndando;
    } else if (IsKeyDown(KEY_D)) {
        j->vel.x = shiftApertado? j->velCorrendo : j->velAndando;
    } else {
        j->vel.x = 0;
    }

    if(IsKeyPressed(KEY_SPACE) && j->noChao) {
        j->vel.y = shiftApertado ? j->velPuloCorrendo : j->velPulo;
        j->noChao = false;
    }

    j->noChao = false;

}

void atualizarJogador(Jogador *j, GameWorld *gw, float delta) {

    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorMapaX(gw->jogador, gw->mapa);
    
    j->vel.y += gw->gravidade * delta;
    
    if(j->vel.y > j->velMaxQueda) {
        j->vel.y = j->velMaxQueda;  
    } 
    j->ret.y += j->vel.y * delta;   
    resolverColisaoJogadorMapaY(gw->jogador, gw->mapa);

}

void desenharJogador(Jogador *j) {
    DrawRectangleRec(j->ret, j->cor);
}

static void resolverColisaoJogadorMapaX( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

         Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( j->ret, o->ret ) ) {
            if ( j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2 ) {
                j->ret.x = o->ret.x - j->ret.width;
            } else {
                j->ret.x = o->ret.x + o->ret.width;
            }
            j->vel.x = 0;
        }

        el = el->proximo;

    }

}

static void resolverColisaoJogadorMapaY( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( j->ret, o->ret ) ) {
            if ( j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                j->ret.y = o->ret.y - j->ret.height;
                j->noChao = true;   
            } else {
                j->ret.y = o->ret.y + o->ret.height;
            }
            j->vel.y = 0;
        }

        el = el->proximo;

    }

}

