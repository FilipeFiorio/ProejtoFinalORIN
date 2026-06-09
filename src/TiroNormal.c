#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "TiroNormal.h"
#include "ResourceManager.h"
#include "Animacao.h"

static void desenharAnimacaoTiroNormal(TiroNormal *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualTiroNormal(TiroNormal *tiro);

static bool MOSTRAR_RETANGULO_COLISAO = false;

TiroNormal *criarTiroNormal(InimigoPlanta*inimigo) {

    TiroNormal *novoTiro = (TiroNormal*) malloc(sizeof(TiroNormal));

    novoTiro->ret.y = inimigo->ret.y + 10;
    novoTiro->ret.height = 16;
    novoTiro->ret.width = 16;
    novoTiro->ret.x = inimigo->paraDireita? inimigo->ret.x + inimigo->ret.width :  inimigo->ret.x - novoTiro->ret.width;

    novoTiro->xIni = novoTiro->ret.x;

    novoTiro->cor = ORANGE;

    novoTiro->ativo = false;
    novoTiro->paraDireita = inimigo->paraDireita;
    
    novoTiro->velX = 150;
    novoTiro->distanciaMax = 500;

    novoTiro->estado = TIRO_VIAJANDO;

    int quantidadeAnimacoes = 0;

    novoTiro->animacaoViajando.quantidadeQuadros = 2;
    novoTiro->animacaoViajando.quadroAtual = 0;
    novoTiro->animacaoViajando.contadorTempoQuadro = 0;
    novoTiro->animacaoViajando.finalizada = false;
    novoTiro->animacaoViajando.executarUmaVez = false;
    novoTiro->animacaoViajando.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoTiro->animacaoViajando, novoTiro->animacaoViajando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoTiro->animacaoViajando.quadros,
        novoTiro->animacaoViajando.quantidadeQuadros,
        100,
        0,
        0,
        16,
        16,
        false,
        0
    );

    novoTiro->animacaoDispersado.quantidadeQuadros = 2;
    novoTiro->animacaoDispersado.quadroAtual = 0;
    novoTiro->animacaoDispersado.contadorTempoQuadro = 0;
    novoTiro->animacaoDispersado.finalizada = false;
    novoTiro->animacaoDispersado.executarUmaVez = true;
    novoTiro->animacaoDispersado.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoTiro->animacaoDispersado, novoTiro->animacaoDispersado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoTiro->animacaoDispersado.quadros,
        novoTiro->animacaoDispersado.quantidadeQuadros,
        200,
        0,
        32,
        16,
        16,
        false,
        0
    );

    novoTiro->animacoes[TIRO_VIAJANDO] = &novoTiro->animacaoViajando;
    quantidadeAnimacoes++;

    novoTiro->animacoes[TIRO_DISPERSADO] = &novoTiro->animacaoDispersado;
    quantidadeAnimacoes++;

    novoTiro->quantidadeAnimacoes = quantidadeAnimacoes;
    
    return novoTiro;

}

void atualizarTiroNormal(TiroNormal *tiro, GameWorld *gw, float delta) {

    if(tiro->ativo) {

        Animacao *animacaoAtual = getAnimacaoAtualTiroNormal(tiro);
        atualizarAnimacao(animacaoAtual, delta);

        if(tiro->estado == TIRO_DISPERSADO && animacaoAtual->finalizada) {
            tiro->ativo = false;
            tiro->ret.x = tiro->xIni;
        }

        tiro->ret.x += tiro->paraDireita? tiro->velX * delta : -tiro->velX * delta;

        if(tiro->paraDireita) {
            if(tiro->ret.x >= tiro->xIni + tiro->distanciaMax ) {
                tiro->estado = TIRO_DISPERSADO;
            }
        } else {
            if(tiro->ret.x <= tiro->xIni - tiro->distanciaMax) {
                tiro->estado = TIRO_DISPERSADO;
            }
        }

    }

}

void destruirTiroNormal(TiroNormal *tiro) {

    if(tiro != NULL) {
        for(int i = 0; i < tiro->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(tiro->animacoes[i]);
        }
        free(tiro);
    }

}

void desenharTiroNormal(TiroNormal *tiro) {
    
    if(tiro->estado == TIRO_VIAJANDO) {
        QuadroAnimacao *quadro = getQuadroAnimacaoAtualTiroNormal(tiro);
        desenharAnimacaoTiroNormal(tiro, quadro, WHITE);
    } else if(tiro->estado == TIRO_DISPERSADO) {
        QuadroAnimacao *quadro = getQuadroAnimacaoAtualTiroNormal(tiro);
        desenharAnimacaoTiroNormal(tiro, quadro, WHITE);
    }
    
}

static void desenharAnimacaoTiroNormal(TiroNormal *tiro, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {


        if(tiro->estado == TIRO_VIAJANDO) {
            
            DrawTexturePro(
                rm.texturaTiro,
                (Rectangle) {
                    quadro->fonte.x,
                    quadro->fonte.y,
                    tiro->paraDireita ? -quadro->fonte.width : quadro->fonte.width,
                    quadro->fonte.height
                },
                tiro->ret,
                (Vector2) {0},
                0.0f,
                tonalidade
            );

        } else if (tiro->estado == TIRO_DISPERSADO){
            
            DrawTexturePro(
                rm.texturaTiro,
                (Rectangle) {
                    quadro->fonte.x,
                    quadro->fonte.y,
                    quadro->fonte.width,
                    quadro->fonte.height
                },
                tiro->ret,
                (Vector2) {0},
                0.0f,
                tonalidade
            );

        }

       if ( MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(tiro->ret, Fade(GREEN, 0.5f));
        }

    }

}

static Animacao *getAnimacaoAtualTiroNormal(TiroNormal *tiro) {

    return tiro->animacoes[tiro->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualTiroNormal(TiroNormal *tiro) {
    return getQuadroAtualAnimacao(getAnimacaoAtualTiroNormal(tiro));
}