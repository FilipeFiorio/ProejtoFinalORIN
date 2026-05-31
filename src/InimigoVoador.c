#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoVoador.h"
#include "ResourceManager.h"
#include "Animacao.h"

static void resolverColisaoInimigoMapaX(InimigoVoador *i, Mapa *m);
static void resolverColisaoInimigoMapaY(InimigoVoador *i, Mapa *m);

static void desenharAnimacaoInimigoVoador(InimigoVoador *inimigo, QuadroAnimacao *quadro, Color tonalidade);
static Animacao *getAnimacaoAtualInimigoVoador(InimigoVoador *inimigo);

static bool MOSTRAR_RETANGULO_COLISAO = true;

InimigoVoador *criarInimigoVoador(float x, float y, float largura, float altura, Vector2 deslocamento, Vector2 vel, Color cor) {

    InimigoVoador *novoInimigoVoador = (InimigoVoador*) malloc(sizeof(InimigoVoador));

    novoInimigoVoador->ret.x = x;
    novoInimigoVoador->ret.y = y;
    novoInimigoVoador->ret.width = largura;
    novoInimigoVoador->ret.height = altura;
    novoInimigoVoador->deslocamento = deslocamento;
    novoInimigoVoador->cor = cor;
    novoInimigoVoador->vel = vel;
    novoInimigoVoador->posInicial = (Vector2) {x, y};
    novoInimigoVoador->estaVivo = true;
    novoInimigoVoador->retornando = false;

    //incializando animacoes;

    int quantidadeAnimacoes = 0;

    novoInimigoVoador->animacaoVoando.quantidadeQuadros = 2;
    novoInimigoVoador->animacaoVoando.quadroAtual = 0;
    novoInimigoVoador->animacaoVoando.contadorTempoQuadro = 0;
    novoInimigoVoador->animacaoVoando.executarUmaVez = false;
    novoInimigoVoador->animacaoVoando.pararNoUltimoQuadro = false;
    novoInimigoVoador->animacaoVoando.finalizada = false;
    novoInimigoVoador->estado = INIMIGO_VOADOR_VOANDO;
    criarQuadroAnimacao(&novoInimigoVoador->animacaoVoando, novoInimigoVoador->animacaoVoando.quantidadeQuadros);
    inicializarQuadroAnimacao(
        novoInimigoVoador->animacaoVoando.quadros,
        novoInimigoVoador->animacaoVoando.quantidadeQuadros,
        150,
        1,
        1,
        25,
        20,
        false,
        1
    );

    novoInimigoVoador->animacoes[INIMIGO_VOADOR_VOANDO] = &novoInimigoVoador->animacaoVoando;
    quantidadeAnimacoes++;

    novoInimigoVoador->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigoVoador;

}

void atualizarInimigoVoador(InimigoVoador *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {


        if(inimigo->estado == INIMIGO_VOADOR_VOANDO) {
            Animacao *animacaoAtual = getAnimacaoAtualInimigoVoador(inimigo);
            atualizarAnimacao(animacaoAtual, delta);

        }

        Vector2 alvo;
        
        if(!inimigo->retornando) {
            alvo = (Vector2) {
                inimigo->posInicial.x + inimigo->deslocamento.x,
                inimigo->posInicial.y + inimigo->deslocamento.y
            };
        } else {
            alvo = inimigo->posInicial;
        }
        
        if(inimigo->deslocamento.x != 0) {
            if(!inimigo->retornando) {
                inimigo->ret.x += inimigo->vel.x * delta;
                
                if(inimigo->ret.x >= alvo.x) { 
                    inimigo->ret.x = alvo.x; 
                    inimigo->retornando = true; 
                }
            } else {
                inimigo->ret.x -= inimigo->vel.x * delta;

                if(inimigo->ret.x <= alvo.x) { 
                    inimigo->ret.x = alvo.x; 
                    inimigo->retornando = false; 
                }
            }
        }

        if(inimigo->deslocamento.y != 0) {
            if(!inimigo->retornando) {
                inimigo->ret.y += inimigo->vel.y * delta;

                if(inimigo->ret.y >= alvo.y) { 
                    inimigo->ret.y = alvo.y; 
                    inimigo->retornando = true; 
                }
            } else {
                inimigo->ret.y -= inimigo->vel.y * delta;
                
                if(inimigo->ret.y <= alvo.y) { 
                    inimigo->ret.y = alvo.y; 
                    inimigo->retornando = false; 
                }
            }
        }
        
        resolverColisaoInimigoMapaX(inimigo, gw->mapa);
        resolverColisaoInimigoMapaY(inimigo, gw->mapa);

    }
    
}

void destruirInimigoVoador(InimigoVoador *inimigo) {

    if(inimigo != NULL) {
        for(int i = 0; i < inimigo->quantidadeAnimacoes; i++) {
            destruirQuadroAnimacao(inimigo->animacoes[i]);
        }
        free(inimigo);
    }

}

void desenharInimigoVoador(InimigoVoador *inimigo) {
    
    if(inimigo->estaVivo) {
        if(inimigo->estado == INIMIGO_VOADOR_VOANDO) {
            QuadroAnimacao *quadro = getQuadroAnimacaoAtualInimigoVoador(inimigo);
            desenharAnimacaoInimigoVoador(inimigo, quadro, WHITE);
        }
        
    }

}

static void resolverColisaoInimigoMapaY( InimigoVoador *i, Mapa *m ) {

    ElementoMapa *el = m->obstaculos;

    while ( el != NULL ) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if ( CheckCollisionRecs( i->ret, o->ret ) ) {
                if ( i->ret.y + i->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                    i->ret.y = o->ret.y - i->ret.height;
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
                } else { //não precisa
                    i->ret.y = o->ret.y + o->ret.height;
                }
                i->vel.y = 0;
            }
         }

        el = el->proximo;

    }

}

static void resolverColisaoInimigoMapaX(InimigoVoador *i, Mapa *m) {

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

static void desenharAnimacaoInimigoVoador(InimigoVoador *inimigo, QuadroAnimacao *quadro, Color tonalidade) {

    if(quadro != NULL) {

        DrawTexturePro(
            rm.texturaInimigoVoador,
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

        if(MOSTRAR_RETANGULO_COLISAO) {
            DrawRectangleRec(inimigo->ret, Fade(GREEN, 0.5f));
        }

    }
}

static Animacao *getAnimacaoAtualInimigoVoador(InimigoVoador *inimigo) {

    return inimigo->animacoes[inimigo->estado];
}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoVoador(InimigoVoador *inimigo) {

    return getQuadroAtualAnimacao(getAnimacaoAtualInimigoVoador(inimigo));

}
