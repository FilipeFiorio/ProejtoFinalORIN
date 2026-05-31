#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoDash.h"
#include "ResourceManager.h"
#include "Animacao.h"

static void resolverColisaoInimigoMapaX(InimigoDash *i, Mapa *m);
static void resolverColisaoInimigoMapaY( InimigoDash *i, Mapa *m);
static bool verificarSeTemChao(InimigoDash *i, Mapa *m);
static float verificarDistanciaJogador(InimigoDash *i, Mapa *m);
static void dash(InimigoDash *i, float distancia);

static void desenharAnimacaoInimigoDash(InimigoDash *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoDash(InimigoDash *inimigo);

static bool MOSTRAR_RETANGULO_COLISAO = true;

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
    novoInimigoDash->paraDireita = false;
    novoInimigoDash->velMaxQueda = 600;
    novoInimigoDash->velXInicial = novoInimigoDash->vel.x;
    novoInimigoDash->estado = INIMIGO_DASH_ANDANDO;

    int quantidadeAnimacoes = 0;

    novoInimigoDash->animacoAndando.quantidadeQuadros = 2;
    novoInimigoDash->animacoAndando.quadroAtual = 0;
    novoInimigoDash->animacoAndando.contadorTempoQuadro = 0;
    novoInimigoDash->animacoAndando.executarUmaVez = false;
    novoInimigoDash->animacoAndando.pararNoUltimoQuadro = false;
    novoInimigoDash->animacoAndando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoDash->animacoAndando, novoInimigoDash->animacoAndando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoDash->animacoAndando.quadros,
        novoInimigoDash->animacoAndando.quantidadeQuadros,
        200,
        40, 
        1,
        28,
        37,
        false,
        1
    );

    novoInimigoDash->animacoes[INIMIGO_DASH_ANDANDO] = &novoInimigoDash->animacoAndando;
    quantidadeAnimacoes++;

    novoInimigoDash->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoDash;

}
void atualizarInimigoDash(InimigoDash *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {

        if(inimigo->estado == INIMIGO_DASH_ANDANDO) {
            Animacao *animacaoAtual = getAnimacaoAtualInimigoDash(inimigo);
            atualizarAnimacao(animacaoAtual, delta);

        }

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

        inimigo->paraDireita = inimigo->vel.x > 0;

    }

}

void destruirInimigoDash(InimigoDash *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoDash(InimigoDash *inimigo) {
    
    if(inimigo->estaVivo) {
        if(inimigo->estado == INIMIGO_DASH_ANDANDO) {
            QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoDash(inimigo);
            desenharAnimacaoInimigoDash(inimigo, quadro, WHITE);
        }
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



static void desenharAnimacaoInimigoDash(InimigoDash *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {

        DrawTexturePro(
            rm.texturaInimigoDash,
            (Rectangle) {
                quadro->fonte.x,
                quadro->fonte.y,
                inimigo->paraDireita ? -quadro->fonte.width : quadro->fonte.width,
                quadro->fonte.height
            },
            inimigo->ret,
            (Vector2) {0},
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(inimigo->ret ,Fade(GREEN, 0.5f));
        }

    }
}

static Animacao *getAnimacaoAtualInimigoDash(InimigoDash *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoDash(InimigoDash *inimigo) {

    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoDash(inimigo));

}