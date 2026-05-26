#include <stdlib.h>
#include "raylib/raylib.h"
#include "Jogador.h"
#include "Tipos.h"
#include "Inimigo.h"
#include "Mapa.h"

static void resolverColisaoJogadorMapaX(Jogador *j, Mapa *mapa);
static void resolverColisaoJogadorMapaY(Jogador *j, Mapa *mapa, float delta);
static void verificarColisaoJogadorItem(GameWorld *gw);
static void verificarColisaoJogadorInimigo(GameWorld *gw);

Jogador *criarJogador(float x, float y, float largura, float altura, Color cor) {

    Jogador *novoJogador = (Jogador*) malloc(sizeof(Jogador));

    novoJogador->ret = (Rectangle) {
        .x = x,
        .y = y,
        .width = largura,
        .height = altura,
    };
    novoJogador->vel = (Vector2) {0};
    novoJogador->cor = BLUE;

    novoJogador->vidas = 5;
    novoJogador->moedas = 0;

    novoJogador->velAndando = 225;
    novoJogador->velCorrendo = 325;
    novoJogador->velPulo = -425;
    novoJogador->velPuloCorrendo = -525;
    novoJogador->velMaxQueda = 600;

    novoJogador->noChao = false;

    return novoJogador;
}

void destruirJogador(Jogador *j) {
    if(j != NULL) {
        free(j);
    }
}

void entradaJogador(Jogador *j) {
    
    bool shiftApertado = IsKeyDown(KEY_LEFT_SHIFT);

    if(IsKeyDown(KEY_A)) {
        j->vel.x = shiftApertado ? -j->velCorrendo : -j->velAndando;
    } else if (IsKeyDown(KEY_D)) {
        j->vel.x = shiftApertado ? j->velCorrendo : j->velAndando;
    } else {
        j->vel.x = 0;
    }

    if(IsKeyPressed(KEY_SPACE) && j->noChao) {
        j->vel.y = shiftApertado ? j->velPuloCorrendo : j->velPulo;
        j->noChao = false;
    }

    j->noChao = false;
}

void atualizarJogador(Jogador *j, GameWorld *gw, float delta) {

    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorMapaX(gw->mapa->jogador, gw->mapa);

    j->vel.y += gw->gravidade * delta;

    if(j->vel.y > j->velMaxQueda) {
        j->vel.y = j->velMaxQueda;
    }

    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorMapaY(gw->mapa->jogador, gw->mapa, delta);

    verificarColisaoJogadorInimigo(gw);
    verificarColisaoJogadorItem(gw);
}

void desenharJogador(Jogador *j) {
    DrawRectangleRec(j->ret, j->cor);
}

/**
 * Agora, alem de checar a colisao do retangulo do jogador com o retangulo do obstaculo, verifica 
 * a largura e altura de do retangulo de colisao, se a altura for maior atualiza em y, caso contrario 
 * atualizza em x
 */
static void resolverColisaoJogadorMapaX(Jogador *j, Mapa *mapa) {

    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if (obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);

                if (retSobre.width <= retSobre.height) {
                    if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                        j->ret.x = o->ret.x - j->ret.width;
                    } else {
                        j->ret.x = o->ret.x + o->ret.width;
                    }
                    j->vel.x = 0;
                }
            }

        } else if (obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {

                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);

                if (retSobre.width <= retSobre.height) {
                    if (j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                        j->ret.x = o->ret.x - j->ret.width;
                    } else {
                        j->ret.x = o->ret.x + o->ret.width;
                    }
                    j->vel.x = 0;
                }
            }
        }

        el = el->proximo;
    }
}

static void resolverColisaoJogadorMapaY(Jogador *j, Mapa *mapa, float delta) {

    ElementoMapa *el = mapa->obstaculos;

    while (el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if (obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {
                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);

                if (retSobre.height < retSobre.width) {
                    if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                        j->ret.y = o->ret.y - j->ret.height;
                        j->noChao = true;
                    } else {
                        j->ret.y = o->ret.y + o->ret.height;
                    }
                    j->vel.y = 0;
                }
            }

        } else if (obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if (CheckCollisionRecs(j->ret, o->ret)) {
                Rectangle retSobre = GetCollisionRec(j->ret, o->ret);

                if (retSobre.height < retSobre.width) {
                    if (j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2) {
                        j->ret.y = o->ret.y - j->ret.height;
                        j->noChao = true;
                        
                        if (o->retornando) {
                            j->ret.x -= o->vel.x * delta;
                        } else {
                            j->ret.x += o->vel.x * delta;
                        }
                    } else {
                        j->ret.y = o->ret.y + o->ret.height;
                    }
                    j->vel.y = 0;
                }
            }
        }

        el = el->proximo;
    }
}

static void verificarColisaoJogadorItem(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    ElementoMapa *el = gw->mapa->itens;

    while(el != NULL) {

        Item *i = (Item*) el->objeto;

        if(CheckCollisionRecs(j->ret, i->ret) && i->ativo) {
            i->ativo = false;
            j->moedas++;
        }

        el = el->proximo;
    }

}

// Atualmente verifica apenas se há colisão pela esquerda, direita e por cima
// por cima apenas ve se a vel.x é maior que zero
// se for > 0 o jogador está em cima do inimigo

// Corrigir logica ao bater na parte inferiori  de um inimigo
static void verificarColisaoJogadorInimigo(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;
    ElementoMapa *el = gw->mapa->inimigos;

    while (el != NULL) {

        Inimigo *inimigo = (Inimigo*) el->objeto;

        if (inimigo->tipo == INIMIGO_NORMAL) {

            InimigoNormal *i = (InimigoNormal*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret) && i->estaVivo) {
                if (j->vel.y > 0) {
                    i->estaVivo = false;
                    j->vel.y = j->velPulo * 0.75f;
                } else {
                    if (j->ret.x + j->ret.width / 2 > i->ret.x + i->ret.width / 2) {
                        j->ret.x = i->ret.x + i->ret.width;
                    } else {
                        j->ret.x = i->ret.x - j->ret.width;
                    }
                    j->vidas--;
                }
                return;
            }

        } else if (inimigo->tipo == INIMIGO_DASH) {

            InimigoDash *i = (InimigoDash*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret) && i->estaVivo) {
                if (j->vel.y > 0) {
                    i->estaVivo = false;
                    j->vel.y = j->velPulo * 0.75f;
                } else {
                    if (j->ret.x + j->ret.width / 2 > i->ret.x + i->ret.width / 2) {
                        j->ret.x = i->ret.x + i->ret.width;
                    } else {
                        j->ret.x = i->ret.x - j->ret.width;
                    }
                    j->vidas--;
                }
                return;
            }

        } else if (inimigo->tipo == INIMIGO_VOADOR) {

            InimigoVoador *i = (InimigoVoador*) inimigo->objeto;

            if (CheckCollisionRecs(j->ret, i->ret) && i->estaVivo) {
                if (j->vel.y > 0) {
                    i->estaVivo = false;
                    j->vel.y = j->velPulo * 0.75f;
                } else {
                    if (j->ret.x + j->ret.width / 2 > i->ret.x + i->ret.width / 2) {
                        j->ret.x = i->ret.x + i->ret.width;
                    } else {
                        j->ret.x = i->ret.x - j->ret.width;
                    }
                    j->vidas--;
                }
                return;
            }

        }

        el = el->proximo;
    }

}