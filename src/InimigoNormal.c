#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoNormal.h"
#include "ResourceManager.h"
#include "Animacao.h"


static void resolverColisaoInimigoMapaX(InimigoNormal *i, Mapa *m);
static void resolverColisaoInimigoMapaY( InimigoNormal *i, Mapa *m);
static bool verificarSeTemChao(InimigoNormal *i, Mapa *m);

static void desenharAnimacaoInimigoNormal(InimigoNormal *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoNormal(InimigoNormal *inimigo);


static bool MOSTRAR_RETANGULO_COLISAO = true;

InimigoNormal *criarInimigoNormal(float x, float y, float largura, float altura, Color cor) {

    InimigoNormal *novoInimigoNormal = (InimigoNormal*) malloc(sizeof(InimigoNormal));

    novoInimigoNormal->ret.x = x;
    novoInimigoNormal->ret.y = y;
    novoInimigoNormal->ret.width = largura;
    novoInimigoNormal->ret.height = altura;
    novoInimigoNormal->vel = (Vector2) {100, 0};
    novoInimigoNormal->velMaxQueda = 600;
    novoInimigoNormal->estaVivo = true;
    novoInimigoNormal->noChao = false;
    novoInimigoNormal->paraDireita = false;
    novoInimigoNormal->cor = cor;
    novoInimigoNormal->estado = INIMIGO_NORMAL_ANDANDO;

    int quantidadeAnimacoes = 0;

    novoInimigoNormal->animacaoAndando.quantidadeQuadros = 4;
    novoInimigoNormal->animacaoAndando.quadroAtual = 0;
    novoInimigoNormal->animacaoAndando.contadorTempoQuadro = 0;
    novoInimigoNormal->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigoNormal->animacaoAndando.executarUmaVez = false;
    novoInimigoNormal->animacaoAndando.finalizada = false;
    criarQuadroAnimacao(&novoInimigoNormal->animacaoAndando, novoInimigoNormal->animacaoAndando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoNormal->animacaoAndando.quadros,
        novoInimigoNormal->animacaoAndando.quantidadeQuadros,
        200,
        1,
        1,
        21, 
        15,
        false,
        1
    );

    novoInimigoNormal->animacoes[INIMIGO_NORMAL_ANDANDO] = &novoInimigoNormal->animacaoAndando;
    quantidadeAnimacoes++;

    novoInimigoNormal->quantidadeAnimacoes = quantidadeAnimacoes;


    return novoInimigoNormal;

}

void atualizarInimigoNormal(InimigoNormal *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {

        if(inimigo->estado == INIMIGO_NORMAL_ANDANDO) {
            Animacao *animacaoAtual = getAnimacaoAtualInimigoNormal(inimigo);
            atualizarAnimacao(animacaoAtual, delta);

        }

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

void destruirInimigoNormal(InimigoNormal *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoNormal(InimigoNormal *inimigo) {

    if(inimigo->estaVivo) {
        if(inimigo->estado == INIMIGO_NORMAL_ANDANDO) {
            QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoNormal(inimigo);
            desenharAnimacaoInimigoNormal(inimigo, quadro, WHITE);
        }   

    }

}


// colisao no eixo x iguais ao jogador, porem ao colidir num obstáculo inverte a direção
static void resolverColisaoInimigoMapaX(InimigoNormal *i, Mapa *m) {

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
static void resolverColisaoInimigoMapaY( InimigoNormal *i, Mapa *m ) {

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

static bool verificarSeTemChao(InimigoNormal *i, Mapa *m) {

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

static void desenharAnimacaoInimigoNormal(InimigoNormal *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {

        DrawTexturePro(
            rm.texturaInimigoNormal,
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
            DrawRectangleRec(inimigo->ret, Fade(GREEN, 0.5f));
        }

    }

}

static Animacao *getAnimacaoAtualInimigoNormal(InimigoNormal *inimigo) {

    return inimigo->animacoes[inimigo->estado];

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoNormal(InimigoNormal *inimigo) {
    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoNormal(inimigo));
}
