#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoPlantaGelo.h"
#include "TiroGelo.h"
#include "ResourceManager.h"
#include "Animacao.h"

static void atirar(InimigoPlantaGelo *inimigo, GameWorld *gw, float delta);

static void desenharAnimacaoInimigoPlantaGelo(InimigoPlantaGelo *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoPlantaGelo(InimigoPlantaGelo *inimigo);

static bool MOSTRAR_RETANGULO_COLISAO = false;

InimigoPlantaGelo *criarInimigoPlantaGelo(float x, float y, float largura, float altura, Color cor) {

    InimigoPlantaGelo *novoInimigoPlantaGelo = (InimigoPlantaGelo*) malloc(sizeof(InimigoPlantaGelo));

    novoInimigoPlantaGelo->ret.x = x;
    novoInimigoPlantaGelo->ret.y = y;
    novoInimigoPlantaGelo->ret.width = largura;
    novoInimigoPlantaGelo->ret.height = altura;
    novoInimigoPlantaGelo->coolDownTiro = 0;
    novoInimigoPlantaGelo->paraDireita = false;
    novoInimigoPlantaGelo->cor = cor;
    novoInimigoPlantaGelo->estado = INIMIGO_PLANTA_PARADO;

    novoInimigoPlantaGelo->tiro = NULL;

    int quantidadeAnimacoes = 0;

    novoInimigoPlantaGelo->animacaoParado.quantidadeQuadros = 1;
    novoInimigoPlantaGelo->animacaoParado.quadroAtual = 0;
    novoInimigoPlantaGelo->animacaoParado.contadorTempoQuadro = 0;
    novoInimigoPlantaGelo->animacaoParado.pararNoUltimoQuadro = false;
    novoInimigoPlantaGelo->animacaoParado.executarUmaVez = false;
    novoInimigoPlantaGelo->animacaoParado.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPlantaGelo->animacaoParado, novoInimigoPlantaGelo->animacaoParado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPlantaGelo->animacaoParado.quadros,
        novoInimigoPlantaGelo->animacaoParado.quantidadeQuadros,
        1000,
        0,
        0,
        35, 
        35,
        false,
        0
    );
    
    novoInimigoPlantaGelo->animacaoAtirando.quantidadeQuadros = 6;
    novoInimigoPlantaGelo->animacaoAtirando.quadroAtual = 0;
    novoInimigoPlantaGelo->animacaoAtirando.contadorTempoQuadro = 0;
    novoInimigoPlantaGelo->animacaoAtirando.pararNoUltimoQuadro = false;
    novoInimigoPlantaGelo->animacaoAtirando.executarUmaVez = true;
    novoInimigoPlantaGelo->animacaoAtirando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPlantaGelo->animacaoAtirando, novoInimigoPlantaGelo->animacaoAtirando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPlantaGelo->animacaoAtirando.quadros,
        novoInimigoPlantaGelo->animacaoAtirando.quantidadeQuadros,
        200,
        0,
        35,
        35, 
        35,
        false,
        0
    );

    novoInimigoPlantaGelo->animacoes[INIMIGO_PLANTA_PARADO] = &novoInimigoPlantaGelo->animacaoParado;
    quantidadeAnimacoes++;
    
    novoInimigoPlantaGelo->animacoes[INIMIGO_PLANTA_ATIRANDO] = &novoInimigoPlantaGelo->animacaoAtirando;
    quantidadeAnimacoes++;

    novoInimigoPlantaGelo->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoPlantaGelo;

}

void atualizarInimigoPlantaGelo(InimigoPlantaGelo *inimigo, GameWorld *gw, float delta) {


    Animacao *animacaoAtual = getAnimacaoAtualInimigoPlantaGelo(inimigo);
    atualizarAnimacao(animacaoAtual, delta);

    inimigo->coolDownTiro += delta * 1000;

    Rectangle ret = {
        inimigo->ret.x - 400,
        inimigo->ret.y - inimigo->ret.width,
        850,
        100
    };

    if(CheckCollisionRecs(ret, gw->mapa->jogador->ret) && inimigo->coolDownTiro >= 2500) {            
        
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
        atualizarTiroGelo(inimigo->tiro, gw, delta);

        if(!inimigo->tiro->ativo) {
            destruirTiroGelo(inimigo->tiro);
            inimigo->tiro = NULL;
        }
    }

    inimigo->paraDireita = inimigo->ret.x < gw->mapa->jogador->ret.x;

}

void destruirInimigoPlantaGelo(InimigoPlantaGelo *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }

        if(inimigo->tiro != NULL) {
            destruirTiroGelo(inimigo->tiro);
        }

        free(inimigo);
    }

}

void desenharInimigoPlantaGelo(InimigoPlantaGelo *inimigo) {

    QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoPlantaGelo(inimigo);
    desenharAnimacaoInimigoPlantaGelo(inimigo, quadro, WHITE);

    if(inimigo->tiro != NULL) {
        desenharTiroGelo(inimigo->tiro);
    }


}

static void desenharAnimacaoInimigoPlantaGelo(InimigoPlantaGelo *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {


        if(inimigo->estado == INIMIGO_PLANTA_PARADO || inimigo->estado == INIMIGO_PLANTA_ATIRANDO) {
            
            DrawTexturePro(
                rm.texturaInimigoPlantaGelo,
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

static Animacao *getAnimacaoAtualInimigoPlantaGelo(InimigoPlantaGelo *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoPlantaGelo(InimigoPlantaGelo *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoPlantaGelo(inimigo));
}

static void atirar(InimigoPlantaGelo *inimigo, GameWorld *gw,float delta) {

    if(inimigo->tiro == NULL) {
        inimigo->tiro = criarTiroGelo(inimigo);
        inimigo->tiro->ativo = true;
    }

}
