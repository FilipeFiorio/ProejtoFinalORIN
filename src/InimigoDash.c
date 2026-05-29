#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoDash.h"
#include "ResourceManager.h"

static void resolverColisaoInimigoMapaX(InimigoDash *i, Mapa *m);
static void resolverColisaoInimigoMapaY( InimigoDash *i, Mapa *m);
static bool verificarSeTemChao(InimigoDash *i, Mapa *m);
static float verificarDistanciaJogador(InimigoDash *i, Mapa *m);
static void dash(InimigoDash *i, float distancia);

InimigoDash *criarInimigoDash(float x, float y, float largura, float altura, float tamanhoDash, Color cor) {

    InimigoDash *novoInimigoDash = (InimigoDash*) malloc(sizeof(InimigoDash));

    novoInimigoDash->ret.x = x;
    novoInimigoDash->ret.y = y;
    novoInimigoDash->ret.width = largura;
    novoInimigoDash->ret.height = altura;
    novoInimigoDash->vel = (Vector2) {100, 0};
    novoInimigoDash->cor = cor;
    novoInimigoDash->tamanhoDash = tamanhoDash;
    novoInimigoDash->estaVivo = true;
    novoInimigoDash->noChao = false;
    novoInimigoDash->velMaxQueda = 600;
    novoInimigoDash->velXInicial = novoInimigoDash->vel.x;
    novoInimigoDash->frameAtual = 1;
    novoInimigoDash->tempoFrame = 0.0f;

    return novoInimigoDash;

}
void atualizarInimigoDash(InimigoDash *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {

        dash(inimigo, verificarDistanciaJogador(inimigo, gw->mapa));
        
        inimigo->ret.x += inimigo->vel.x * delta;
        resolverColisaoInimigoMapaX(inimigo, gw->mapa);

        inimigo->vel.y += gw->gravidade * delta;

        inimigo->ret.y += inimigo->vel.y * delta;
        resolverColisaoInimigoMapaY(inimigo, gw->mapa);

        if(inimigo->vel.y > inimigo->velMaxQueda) {
            inimigo->vel.y = inimigo->velMaxQueda;
        }

        if(inimigo->noChao && !verificarSeTemChao(inimigo, gw->mapa)) {
            inimigo->vel.x = -inimigo->vel.x;
        }

        bool isDashing = (fabs(inimigo->vel.x) > fabs(inimigo->velXInicial) * 1.5f);
        if (isDashing) {
            inimigo->frameAtual = 0;
        } else {
            inimigo->tempoFrame += delta;
            if (inimigo->tempoFrame >= 0.15f) {
                inimigo->tempoFrame = 0.0f;
                inimigo->frameAtual = (inimigo->frameAtual == 1) ? 2 : 1;
            }
        }
    }

}

void destruirInimigoDash(InimigoDash *inimigo) {

    if(inimigo != NULL) {
        free(inimigo);
    }

}

void desenharInimigoDash(InimigoDash *inimigo) {
    
    if(inimigo->estaVivo) {
        float larguraFrame = (float) rm.texturaInimigoDash.width / 5.0f;
        Rectangle fonte = {
            .x = inimigo->frameAtual * larguraFrame,
            .y = 1,
            .width = larguraFrame,
            .height = 38
        };
        if (inimigo->vel.x > 0) {
            fonte.width = -fonte.width;
        }
        DrawTexturePro(
            rm.texturaInimigoDash,
            fonte,
            inimigo->ret,
            (Vector2) {0},
            0.0f,
            WHITE
        );
    }

}

static void resolverColisaoInimigoMapaX(InimigoDash *i, Mapa *m) {

    ElementoMapa *el = m->obstaculos;

    while(el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if(CheckCollisionRecs(i->ret, o->ret)) {
                if(i->ret.x + i->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    i->ret.x = o->ret.x - i->ret.width;
                } else {
                    i->ret.x = o->ret.x + o->ret.width;
                }
                    
                i->vel.x = -i->vel.x;
                
            }

        } else if(obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if(CheckCollisionRecs(i->ret, o->ret)) {
                if(i->ret.x + i->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    i->ret.x = o->ret.x - i->ret.width;
                } else {
                    i->ret.x = o->ret.x + o->ret.width;
                }
                    
                i->vel.x = -i->vel.x;
                
            }
            
        }

        el = el->proximo;

    }


}

// todo: verificar se há chao para continuar andando, se nao houver inverter direção
static void resolverColisaoInimigoMapaY( InimigoDash *i, Mapa *m ) {

    ElementoMapa *el = m->obstaculos;

    while ( el != NULL ) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if ( CheckCollisionRecs( i->ret, o->ret ) ) {
                if ( i->ret.y + i->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                    i->ret.y = o->ret.y - i->ret.height;
                    i->noChao = true;
                } else { //não precisa
                    i->ret.y = o->ret.y + o->ret.height;
                }
                i->vel.y = 0;
            }

        } else if(obs->tipo == OBSTACULO_MOVEL)  {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;
            
            if ( CheckCollisionRecs( i->ret, o->ret ) ) {
                if ( i->ret.y + i->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                    i->ret.y = o->ret.y - i->ret.height;
                    i->noChao = true;
                } else { //não precisa
                    i->ret.y = o->ret.y + o->ret.height;
                }
                i->vel.y = 0;
            }
         }

        el = el->proximo;

    }

}

static bool verificarSeTemChao(InimigoDash *i, Mapa *m) {

    Rectangle ret = {
        .x = (i->vel.x > 0) ? (i->ret.x + i->ret.width) : (i->ret.x) - 1,
        .y = i->ret.y + i->ret.height,
        .width = 1,
        .height = 1
    };

    ElementoMapa *el = m->obstaculos;

    while(el != NULL) {
        
        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;
            
            if(CheckCollisionRecs(ret, o->ret)) {
                return true;
            }

        } else if(obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;
            
            if(CheckCollisionRecs(ret, o->ret)) {
                return true;
            }
        }
        
        el = el->proximo;
    }

    return false;
}

static float verificarDistanciaJogador(InimigoDash *i, Mapa *m) {

    Rectangle ret = {
        .x = i->ret.x - 150,
        .y = i->ret.y,
        .width = (150 + i->ret.width + 150),
        .height = i->ret.height
    };

    Jogador *j = m->jogador;

    if(CheckCollisionRecs(ret, j->ret)) {
        return j->ret.x - i->ret.x;
    }

    return 0;

}

static void dash(InimigoDash *i, float distancia) {

    if(distancia != 0) {
        int direcao = (distancia > 0) ? 1 : -1;
        i->vel.x = direcao * i->velXInicial * 4;
    } else {
        int direcao = (i->vel.x >= 0) ? 1 : -1;
        i->vel.x = direcao * i->velXInicial;
    }
}