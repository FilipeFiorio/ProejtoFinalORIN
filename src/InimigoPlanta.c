#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoPlanta.h"
#include "TiroNormal.h"
#include "ResourceManager.h"
#include "Animacao.h"

static void atirar(InimigoPlanta *inimigo, GameWorld *gw, float delta);

static void desenharAnimacaoInimigoPlanta(InimigoPlanta *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoPlanta(InimigoPlanta *inimigo);

static bool MOSTRAR_RETANGULO_COLISAO = false;

InimigoPlanta *criarInimigoPlanta(float x, float y, float largura, float altura, Color cor) {

    InimigoPlanta *novoInimigoPlanta = (InimigoPlanta*) malloc(sizeof(InimigoPlanta));

    novoInimigoPlanta->ret.x = x;
    novoInimigoPlanta->ret.y = y;
    novoInimigoPlanta->ret.width = largura;
    novoInimigoPlanta->ret.height = altura;
    novoInimigoPlanta->coolDownTiro = 0;
    novoInimigoPlanta->paraDireita = false;
    novoInimigoPlanta->cor = cor;
    novoInimigoPlanta->estado = INIMIGO_PLANTA_PARADO;

    novoInimigoPlanta->tiro = NULL;

    int quantidadeAnimacoes = 0;

    novoInimigoPlanta->animacaoParado.quantidadeQuadros = 1;
    novoInimigoPlanta->animacaoParado.quadroAtual = 0;
    novoInimigoPlanta->animacaoParado.contadorTempoQuadro = 0;
    novoInimigoPlanta->animacaoParado.pararNoUltimoQuadro = false;
    novoInimigoPlanta->animacaoParado.executarUmaVez = false;
    novoInimigoPlanta->animacaoParado.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPlanta->animacaoParado, novoInimigoPlanta->animacaoParado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPlanta->animacaoParado.quadros,
        novoInimigoPlanta->animacaoParado.quantidadeQuadros,
        1000,
        0,
        0,
        35, 
        35,
        false,
        0
    );
    
    novoInimigoPlanta->animacaoAtirando.quantidadeQuadros = 6;
    novoInimigoPlanta->animacaoAtirando.quadroAtual = 0;
    novoInimigoPlanta->animacaoAtirando.contadorTempoQuadro = 0;
    novoInimigoPlanta->animacaoAtirando.pararNoUltimoQuadro = false;
    novoInimigoPlanta->animacaoAtirando.executarUmaVez = true;
    novoInimigoPlanta->animacaoAtirando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPlanta->animacaoAtirando, novoInimigoPlanta->animacaoAtirando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPlanta->animacaoAtirando.quadros,
        novoInimigoPlanta->animacaoAtirando.quantidadeQuadros,
        200,
        0,
        35,
        35, 
        35,
        false,
        0
    );

    novoInimigoPlanta->animacoes[INIMIGO_PLANTA_PARADO] = &novoInimigoPlanta->animacaoParado;
    quantidadeAnimacoes++;
    
    novoInimigoPlanta->animacoes[INIMIGO_PLANTA_ATIRANDO] = &novoInimigoPlanta->animacaoAtirando;
    quantidadeAnimacoes++;

    novoInimigoPlanta->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoPlanta;

}

void atualizarInimigoPlanta(InimigoPlanta *inimigo, GameWorld *gw, float delta) {


    Animacao *animacaoAtual = getAnimacaoAtualInimigoPlanta(inimigo);
    atualizarAnimacao(animacaoAtual, delta);

    inimigo->coolDownTiro += delta * 1000;

    Rectangle ret = {
        inimigo->ret.x - 400,
        inimigo->ret.y - inimigo->ret.width,
        850,
        100
    };

    if(CheckCollisionRecs(ret, gw->mapa->jogador->ret) && inimigo->coolDownTiro >= 4000) {            
        
        inimigo->estado = INIMIGO_PLANTA_ATIRANDO;

        if(animacaoAtual->quadroAtual == 3) {
            atirar(inimigo, gw, delta);
        }
 
        if(animacaoAtual->finalizada) {
            inimigo->coolDownTiro = 0;
            inimigo->animacaoAtirando.finalizada = false;
        }

    
    } else {
        inimigo->estado = INIMIGO_PLANTA_PARADO;
    }

    if(inimigo->tiro != NULL) {
        atualizarTiroNormal(inimigo->tiro, gw, delta);

        if(!inimigo->tiro->ativo) {
            destruirTiroNormal(inimigo->tiro);
            inimigo->tiro = NULL;
        }
    }

    inimigo->paraDireita = inimigo->ret.x < gw->mapa->jogador->ret.x;

}

void destruirInimigoPlanta(InimigoPlanta *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }

        if(inimigo->tiro != NULL) {
            destruirTiroNormal(inimigo->tiro);
        }

        free(inimigo);
    }

}

void desenharInimigoPlanta(InimigoPlanta *inimigo) {

    QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoPlanta(inimigo);
    desenharAnimacaoInimigoPlanta(inimigo, quadro, WHITE);

    if(inimigo->tiro != NULL) {
        desenharTiroNormal(inimigo->tiro);
    }


}

static void desenharAnimacaoInimigoPlanta(InimigoPlanta *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {


        if(inimigo->estado == INIMIGO_PLANTA_PARADO || inimigo->estado == INIMIGO_PLANTA_ATIRANDO) {
            
            DrawTexturePro(
                rm.texturaInimigoPlanta,
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

        } 

       if ( MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(inimigo->ret, Fade(GREEN, 0.5f));
        }

    }

}

static Animacao *getAnimacaoAtualInimigoPlanta(InimigoPlanta *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoPlanta(InimigoPlanta *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoPlanta(inimigo));
}

static void atirar(InimigoPlanta *inimigo, GameWorld *gw,float delta) {

    if(inimigo->tiro == NULL) {
        inimigo->tiro = criarTiroNormal(inimigo);
        inimigo->tiro->ativo = true;
    }

}
