#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoPedra.h"
#include "ResourceManager.h"
#include "Animacao.h"

static bool verificarToqueChao(Mapa *m, InimigoPedra *i);
static bool jogadorEmbaixo(Jogador *j, InimigoPedra *i);

static void desenharAnimacaoInimigoPedra(InimigoPedra *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoPedra(InimigoPedra *inimigo);

static bool MOSTRAR_RETANGULO_COLISAO = 0;

InimigoPedra *criarInimigoPedra(float x, float y, float largura, float altura, Color cor) {

    InimigoPedra *novoInimigoPedra = (InimigoPedra*) malloc(sizeof(InimigoPedra));

    novoInimigoPedra->ret.x = x;
    novoInimigoPedra->ret.y = y;
    novoInimigoPedra->posInicial = (Vector2) {x, y};
    novoInimigoPedra->ret.width = largura;
    novoInimigoPedra->ret.height = altura;
    novoInimigoPedra->vel = (Vector2) {0, 500};
    novoInimigoPedra->velAtual = 0;
    novoInimigoPedra->retornando = false;
    novoInimigoPedra->cor = cor;
    novoInimigoPedra->estado = INIMIGO_PEDRA_PARADO;

    int quantidadeAnimacoes = 0;

    novoInimigoPedra->animacaoParado.quantidadeQuadros = 1;
    novoInimigoPedra->animacaoParado.quadroAtual = 0;
    novoInimigoPedra->animacaoParado.contadorTempoQuadro = 0;
    novoInimigoPedra->animacaoParado.pararNoUltimoQuadro = false;
    novoInimigoPedra->animacaoParado.executarUmaVez = false;
    novoInimigoPedra->animacaoParado.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPedra->animacaoParado, novoInimigoPedra->animacaoParado.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPedra->animacaoParado.quadros,
        novoInimigoPedra->animacaoParado.quantidadeQuadros,
        1000,
        1,
        1,
        34, 
        40,
        false,
        1
    );
    novoInimigoPedra->animacaoOlhoEsquerdo.quantidadeQuadros = 1;
    novoInimigoPedra->animacaoOlhoEsquerdo.quadroAtual = 0;
    novoInimigoPedra->animacaoOlhoEsquerdo.contadorTempoQuadro = 0;
    novoInimigoPedra->animacaoOlhoEsquerdo.pararNoUltimoQuadro = false;
    novoInimigoPedra->animacaoOlhoEsquerdo.executarUmaVez = false;
    novoInimigoPedra->animacaoOlhoEsquerdo.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPedra->animacaoOlhoEsquerdo, novoInimigoPedra->animacaoOlhoEsquerdo.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPedra->animacaoOlhoEsquerdo.quadros,
        novoInimigoPedra->animacaoOlhoEsquerdo.quantidadeQuadros,
        1000,
        1,
        42,
        34, 
        40,
        false,
        1
    );

    novoInimigoPedra->animacaoOlhoDireito.quantidadeQuadros = 1;
    novoInimigoPedra->animacaoOlhoDireito.quadroAtual = 0;
    novoInimigoPedra->animacaoOlhoDireito.contadorTempoQuadro = 0;
    novoInimigoPedra->animacaoOlhoDireito.pararNoUltimoQuadro = false;
    novoInimigoPedra->animacaoOlhoDireito.executarUmaVez = false;
    novoInimigoPedra->animacaoOlhoDireito.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPedra->animacaoOlhoDireito, novoInimigoPedra->animacaoOlhoDireito.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPedra->animacaoOlhoDireito.quadros,
        novoInimigoPedra->animacaoOlhoEsquerdo.quantidadeQuadros,
        1000,
        1,
        83,
        34, 
        40,
        false,
        1
    );

    novoInimigoPedra->animacaoDescendo.quantidadeQuadros = 1;
    novoInimigoPedra->animacaoDescendo.quadroAtual = 0;
    novoInimigoPedra->animacaoDescendo.contadorTempoQuadro = 0;
    novoInimigoPedra->animacaoDescendo.pararNoUltimoQuadro = false;
    novoInimigoPedra->animacaoDescendo.executarUmaVez = false;
    novoInimigoPedra->animacaoDescendo.finalizada = false;
    criarQuadroAnimacao(&novoInimigoPedra->animacaoDescendo, novoInimigoPedra->animacaoDescendo.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoPedra->animacaoDescendo.quadros,
        novoInimigoPedra->animacaoDescendo.quantidadeQuadros,
        1000,
        1,
        124,
        34, 
        40,
        false,
        1
    );

    novoInimigoPedra->animacoes[INIMIGO_PEDRA_PARADO] = &novoInimigoPedra->animacaoParado;
    quantidadeAnimacoes++;

    novoInimigoPedra->animacoes[INIMIGO_PEDRA_OLHO_ESQUERDO] = &novoInimigoPedra->animacaoOlhoEsquerdo;
    quantidadeAnimacoes++;

    novoInimigoPedra->animacoes[INIMIGO_PEDRA_OLHO_DIREITO] = &novoInimigoPedra->animacaoOlhoDireito;
    quantidadeAnimacoes++;

    novoInimigoPedra->animacoes[INIMIGO_PEDRA_DESCENDO] = &novoInimigoPedra->animacaoDescendo;
    quantidadeAnimacoes++;
    
    novoInimigoPedra->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoPedra;

}

void atualizarInimigoPedra(InimigoPedra *inimigo, GameWorld *gw, float delta) {

    Animacao *animacaoAtual = getAnimacaoAtualInimigoPedra(inimigo);
    atualizarAnimacao(animacaoAtual, delta);

    inimigo->ret.y += inimigo->velAtual * delta;

    Rectangle retJogador = gw->mapa->jogador->ret;
    Rectangle ret = {
        inimigo->ret.x - 400,
        inimigo->ret.y,
        900,
        2000
    };

    if(verificarToqueChao(gw->mapa, inimigo)) {
        
        inimigo->velAtual = -inimigo->vel.y / 3;
        inimigo->estado = INIMIGO_PEDRA_PARADO;
        inimigo->retornando = true;
        
    }
    
    if(inimigo->ret.y <= inimigo->posInicial.y && inimigo->retornando) {
        inimigo->ret.y = inimigo->posInicial.y;
        inimigo->velAtual = 0;
        inimigo->retornando = false;
    }

    if(CheckCollisionRecs(ret, retJogador) && !inimigo->retornando) {

        if(jogadorEmbaixo(gw->mapa->jogador, inimigo)) {
            inimigo->estado = INIMIGO_PEDRA_DESCENDO;
            inimigo->velAtual = inimigo->vel.y;
        } 
        
        if(inimigo->estado != INIMIGO_PEDRA_DESCENDO) {
            if(retJogador.x < inimigo->ret.x) {
                inimigo->estado = INIMIGO_PEDRA_OLHO_ESQUERDO;
            } else if(retJogador.x > inimigo->ret.x) {
                inimigo->estado = INIMIGO_PEDRA_OLHO_DIREITO;
            }
        }
    } else {
        inimigo->estado = INIMIGO_PEDRA_PARADO;
    }
  
}

void destruirInimigoPedra(InimigoPedra *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoPedra(InimigoPedra *inimigo) {

    QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoPedra(inimigo);
    desenharAnimacaoInimigoPedra(inimigo, quadro, WHITE);

}

static void desenharAnimacaoInimigoPedra(InimigoPedra *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {

        DrawTexturePro(
            rm.texturaInimigoPedra,
              (Rectangle) {
                quadro->fonte.x,
                quadro->fonte.y,
                quadro->fonte.width,
                quadro->fonte.height
            },
            inimigo->ret,
            (Vector2) {0},
            0.0f,
            tonalidade
        );


       if ( MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(inimigo->ret, Fade(GREEN, 0.5f));
        }

    }

}

static Animacao *getAnimacaoAtualInimigoPedra(InimigoPedra *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoPedra(InimigoPedra *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoPedra(inimigo));
}

static bool verificarToqueChao(Mapa *m, InimigoPedra* i) {

    ElementoMapa* el = m->obstaculos;

    while(el != NULL) {

        Obstaculo* obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if(CheckCollisionRecs(i->ret, o->ret)) {
                return true;
            }
            
        } else if(obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if(CheckCollisionRecs(i->ret, o->ret)) {
                return true;
            }

        } else if(obs->tipo == OBSTACULO_GELO) {

            ObstaculoGelo *o = (ObstaculoGelo*) obs->objeto;

            if(CheckCollisionRecs(o->ret, i->ret)) {
                return true;
            }
        }

        el = el->proximo;

    }

    return false;

}

static bool jogadorEmbaixo(Jogador *j, InimigoPedra* i) {


    Rectangle ret = {
        i->ret.x,
        i->ret.y + i->ret.height,
        i->ret.width,
        20000  //pegar todo mapa verticalmente
    };

    return CheckCollisionRecs(j->ret, ret);

}

