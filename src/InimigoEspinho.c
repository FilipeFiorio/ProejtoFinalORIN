#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoEspinho.h"
#include "ResourceManager.h"
#include "Animacao.h"


static void resolverColisaoInimigoMapaX(InimigoEspinho *i, Mapa *m);
static void resolverColisaoInimigoMapaY( InimigoEspinho *i, Mapa *m);
static bool verificarSeTemChao(InimigoEspinho *i, Mapa *m);

static void desenharAnimacaoInimigoEspinho(InimigoEspinho *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoEspinho(InimigoEspinho *inimigo);


static bool MOSTRAR_RETANGULO_COLISAO = false;

InimigoEspinho *criarInimigoEspinho(float x, float y, float largura, float altura, Color cor) {

    InimigoEspinho *novoInimigoEspinho = (InimigoEspinho*) malloc(sizeof(InimigoEspinho));

    novoInimigoEspinho->ret.x = x;
    novoInimigoEspinho->ret.y = y;
    novoInimigoEspinho->ret.width = largura;
    novoInimigoEspinho->ret.height = altura;
    novoInimigoEspinho->vel = (Vector2) {100, 0};
    novoInimigoEspinho->velMaxQueda = 600;
    novoInimigoEspinho->estaVivo = true;
    novoInimigoEspinho->noChao = false;
    novoInimigoEspinho->paraDireita = false;
    novoInimigoEspinho->cor = cor;
    novoInimigoEspinho->estado = INIMIGO_ESPINHO_ANDANDO;

    int quantidadeAnimacoes = 0;

    novoInimigoEspinho->animacaoAndando.quantidadeQuadros = 5;
    novoInimigoEspinho->animacaoAndando.quadroAtual = 0;
    novoInimigoEspinho->animacaoAndando.contadorTempoQuadro = 0;
    novoInimigoEspinho->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigoEspinho->animacaoAndando.executarUmaVez = false;
    novoInimigoEspinho->animacaoAndando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoEspinho->animacaoAndando, novoInimigoEspinho->animacaoAndando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoEspinho->animacaoAndando.quadros,
        novoInimigoEspinho->animacaoAndando.quantidadeQuadros,
        200,
        1,
        1,
        26, 
        28,
        false,
        1
    );

    novoInimigoEspinho->animacoes[INIMIGO_ESPINHO_ANDANDO] = &novoInimigoEspinho->animacaoAndando;
    quantidadeAnimacoes++;

    novoInimigoEspinho->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoEspinho;

}

void atualizarInimigoEspinho(InimigoEspinho *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {

        Animacao *animacaoAtual = getAnimacaoAtualInimigoEspinho(inimigo);
        atualizarAnimacao(animacaoAtual, delta);

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

void destruirInimigoEspinho(InimigoEspinho *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoEspinho(InimigoEspinho *inimigo) {

    if(inimigo->estaVivo) {
        if(inimigo->estado == INIMIGO_ESPINHO_ANDANDO) {
            QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoEspinho(inimigo);
            desenharAnimacaoInimigoEspinho(inimigo, quadro, WHITE);
        } 

    } 

}


// colisao no eixo x iguais ao jogador, porem ao colidir num obstáculo inverte a direção
static void resolverColisaoInimigoMapaX(InimigoEspinho *i, Mapa *m) {

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
static void resolverColisaoInimigoMapaY( InimigoEspinho *i, Mapa *m ) {

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

static bool verificarSeTemChao(InimigoEspinho *i, Mapa *m) {

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

static void desenharAnimacaoInimigoEspinho(InimigoEspinho *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {


        if(inimigo->estado == INIMIGO_ESPINHO_ANDANDO) {
            
            DrawTexturePro(
                rm.texturaInimigoEspinho,
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

static Animacao *getAnimacaoAtualInimigoEspinho(InimigoEspinho *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoEspinho(InimigoEspinho *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoEspinho(inimigo));
}
