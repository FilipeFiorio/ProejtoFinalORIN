#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoFantasma.h"
#include "ResourceManager.h"
#include "Animacao.h"


static void desenharAnimacaoInimigoFantasma(InimigoFantasma *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoFantasma(InimigoFantasma *inimigo);

static void perseguirJogador(InimigoFantasma *inimigo, Jogador *jogador, float delta);

static bool MOSTRAR_RETANGULO_COLISAO = false;

InimigoFantasma *criarInimigoFantasma(float x, float y, float largura, float altura, Color cor) {

    InimigoFantasma *novoInimigoFantasma = (InimigoFantasma*) malloc(sizeof(InimigoFantasma));

    novoInimigoFantasma->ret.x = x;
    novoInimigoFantasma->ret.y = y;
    novoInimigoFantasma->ret.width = largura;
    novoInimigoFantasma->ret.height = altura;
    novoInimigoFantasma->vel = (Vector2) {125, 75};
    novoInimigoFantasma->estaVivo = true;
    novoInimigoFantasma->paraDireita = false;
    novoInimigoFantasma->cor = cor;
    novoInimigoFantasma->estado = INIMIGO_FANTASMA_PARADO;

    int quantidadeAnimacoes = 0;

    novoInimigoFantasma->animacaoVoando.quantidadeQuadros = 3;
    novoInimigoFantasma->animacaoVoando.quadroAtual = 0;
    novoInimigoFantasma->animacaoVoando.contadorTempoQuadro = 0;
    novoInimigoFantasma->animacaoVoando.pararNoUltimoQuadro = false;
    novoInimigoFantasma->animacaoVoando.executarUmaVez = false;
    novoInimigoFantasma->animacaoVoando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoFantasma->animacaoVoando, novoInimigoFantasma->animacaoVoando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoFantasma->animacaoVoando.quadros,
        novoInimigoFantasma->animacaoVoando.quantidadeQuadros,
        300,
        1,
        1,
        24, 
        20,
        false,
        1
    );

    novoInimigoFantasma->animacaoParado.quantidadeQuadros = 1;
    novoInimigoFantasma->animacaoParado.quadroAtual = 0;
    novoInimigoFantasma->animacaoParado.contadorTempoQuadro = 0;
    novoInimigoFantasma->animacaoParado.finalizada = false;
    novoInimigoFantasma->animacaoParado.executarUmaVez = false;
    novoInimigoFantasma->animacaoParado.pararNoUltimoQuadro = false;
    criarQuadroAnimacao(&novoInimigoFantasma->animacaoParado, novoInimigoFantasma->animacaoParado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoFantasma->animacaoParado.quadros,
        novoInimigoFantasma->animacaoParado.quantidadeQuadros,
        1000,
        1,
        22,
        24,
        20,
        false,
        1
    );

    novoInimigoFantasma->animacoes[INIMIGO_FANTASMA_VOANDO] = &novoInimigoFantasma->animacaoVoando;
    quantidadeAnimacoes++;

    novoInimigoFantasma->animacoes[INIMIGO_FANTASMA_PARADO] = &novoInimigoFantasma->animacaoParado;
    quantidadeAnimacoes++;

    novoInimigoFantasma->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoFantasma;

}

//Por ser um fantasma nao ha necessidade de implementar colisao
void atualizarInimigoFantasma(InimigoFantasma *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {

        Animacao *animacaoAtual = getAnimacaoAtualInimigoFantasma(inimigo);
        atualizarAnimacao(animacaoAtual, delta);

        perseguirJogador(inimigo, gw->mapa->jogador, delta);

        inimigo->ret.y += GetRandomValue(-20, 20) * delta;

        inimigo->paraDireita = (gw->mapa->jogador->ret.x > inimigo->ret.x);

    }


}

void destruirInimigoFantasma(InimigoFantasma *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoFantasma(InimigoFantasma *inimigo) {

    if(inimigo->estaVivo) {
        if(inimigo->estado == INIMIGO_FANTASMA_PARADO) {
            QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoFantasma(inimigo);
            desenharAnimacaoInimigoFantasma(inimigo, quadro, WHITE);        
        } else if(inimigo->estado == INIMIGO_FANTASMA_VOANDO) {
            QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoFantasma(inimigo);
            desenharAnimacaoInimigoFantasma(inimigo, quadro, WHITE);        
        }
    } 

}

static void desenharAnimacaoInimigoFantasma(InimigoFantasma *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {


        if(inimigo->estado == INIMIGO_FANTASMA_VOANDO) {
            
            DrawTexturePro(
                rm.texturaInimigoFantasma,
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
            
        } else if(inimigo->estado == INIMIGO_FANTASMA_PARADO) {
            
            DrawTexturePro(
                rm.texturaInimigoFantasma,
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

static Animacao *getAnimacaoAtualInimigoFantasma(InimigoFantasma *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoFantasma(InimigoFantasma *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoFantasma(inimigo));
}

static void perseguirJogador(InimigoFantasma *inimigo, Jogador *jogador, float delta) {

    //Fantasma a esquerda do jogador e jogador olhando para a esquerda
    if(jogador->ret.x < inimigo->ret.x && !jogador->paraDireita ) {

        inimigo->estado = INIMIGO_FANTASMA_VOANDO;
        inimigo->ret.x -= inimigo->vel.x * delta;

        if(jogador->ret.y > inimigo->ret.y) {

            inimigo->ret.y += inimigo->vel.y * delta;

        } else {

            inimigo->ret.y -= inimigo->vel.y * delta;

        }
        
        
    } else if(jogador->ret.x > inimigo->ret.x && jogador->paraDireita) {
        
        
        inimigo->estado = INIMIGO_FANTASMA_VOANDO;
        inimigo->ret.x += inimigo->vel.x * delta;
        
        if(jogador->ret.y > inimigo->ret.y) {

            inimigo->ret.y += inimigo->vel.y * delta;

        } else {

            inimigo->ret.y -= inimigo->vel.y * delta;

        }

    } else {

        inimigo->estado = INIMIGO_FANTASMA_PARADO;

    }


}
