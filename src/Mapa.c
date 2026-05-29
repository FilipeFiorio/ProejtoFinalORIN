#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "ResourceManager.h"
#include "Tipos.h"
#include "Mapa.h"
#include "Obstaculo.h"
#include "ObstaculoNormal.h"
#include "ObstaculoMovel.h"
#include "ObstaculoChegada.h"
#include "Obstaculo.h"
#include "Inimigo.h"
#include "InimigoNormal.h"
#include "InimigoDash.h"
#include "InimigoVoador.h"
#include "Item.h"
#include "ItemMoeda.h"
#include "ItemMoedaEspecial.h"
#include "ItemVida.h"
#include "Jogador.h"

static void inserirObstaculo(Mapa *m, ElementoMapa *o);
static void inserirInimigo(Mapa *m, ElementoMapa *i);
static void inserirItem(Mapa *m, ElementoMapa *i);

Mapa *carregarMapa( const char *caminhoArquivo ) {

    Mapa *novoMapa = (Mapa*) malloc( sizeof( Mapa ) );

    novoMapa->obstaculos = NULL;
    novoMapa->quantidadeObstaculos = 0;

    novoMapa->inimigos = NULL;
    novoMapa->quantidadeInimigos = 0;

    novoMapa->itens = NULL;
    novoMapa->quantidadeItens = 0;

    novoMapa->jogador = NULL;
    
    novoMapa->tamanhoElemento = 48;
    novoMapa->linhas = 0;
    novoMapa->colunas = 0;
    
    char *dadosMapa = LoadFileText( caminhoArquivo );
    char *caractereAtual = dadosMapa;
    int linhaAtual = 0;
    int colunaAtual = 0;

    while ( *caractereAtual != '\0' ) {

        if ( *caractereAtual == '\n' ) {

            linhaAtual++;
            colunaAtual = 0;
            novoMapa->linhas = linhaAtual;

        } else {

            if(*caractereAtual != ' ') {

                ElementoMapa *el = (ElementoMapa*) malloc(sizeof(ElementoMapa));
                el->proximo = NULL;

                if('A' <= *caractereAtual && *caractereAtual <= 'Z') {

                    int deslocamento = *caractereAtual - 'A';

                    Obstaculo *obs = NULL;

                    //plataforma vertical
                    if(*caractereAtual == 'S') {

                        obs = criarObstaculo(OBSTACULO_MOVEL);
                        
                        obs->objeto = criarObstaculoMovel(
                            (Rectangle) {
                                .x = colunaAtual * novoMapa->tamanhoElemento,
                                .y = linhaAtual * novoMapa->tamanhoElemento,
                                .width = novoMapa->tamanhoElemento,
                                .height = novoMapa->tamanhoElemento
                            },
                            (Rectangle) {
                                1 + (novoMapa->tamanhoElemento + 1) * deslocamento,
                                1,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento
                            },
                            (Vector2) {0, (float) (novoMapa->tamanhoElemento * 5)},
                            (Vector2) {0, 50.0},
                            YELLOW,
                            &rm.texturaTerreno
                        );
                    } else if(*caractereAtual == 'T') {
                        
                        obs = criarObstaculo(OBSTACULO_MOVEL);
                        
                        obs->objeto = criarObstaculoMovel(
                            (Rectangle) {
                                .x = colunaAtual * novoMapa->tamanhoElemento,
                                .y = linhaAtual * novoMapa->tamanhoElemento,
                                .width = novoMapa->tamanhoElemento,
                                .height = novoMapa->tamanhoElemento
                            },
                            (Rectangle) {
                                1 + (novoMapa->tamanhoElemento + 1) * deslocamento,
                                1,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento
                            },
                            (Vector2) {(float) (novoMapa->tamanhoElemento * 7), 0},
                            (Vector2) {50.0, 0},
                            YELLOW,
                            &rm.texturaTerreno
                        );
                        
                    } else {
                        
                        obs = criarObstaculo(OBSTACULO_NORMAL);
                        
                        obs->objeto = criarObstaculoNormal(
                            (Rectangle) {
                                .x = colunaAtual * novoMapa->tamanhoElemento,
                                .y = linhaAtual * novoMapa->tamanhoElemento,
                                .width = novoMapa->tamanhoElemento,
                                .height = novoMapa->tamanhoElemento
                            },
                            (Rectangle) {
                                1 + (novoMapa->tamanhoElemento + 1) * deslocamento,
                                1,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento
                            }, 
                            YELLOW,
                            &rm.texturaTerreno                          
                        );
                        
                    }
                    
                    el->objeto = obs;
                    el->tipo = ELEMENTO_MAPA_OBSTACULO;

                    inserirObstaculo(novoMapa, el);

                } else if('1' <= *caractereAtual && *caractereAtual <= '9') {

                    Inimigo *inimigo = NULL;

                    switch(*caractereAtual) {
                        case '1':

                            inimigo = criarInimigo(INIMIGO_NORMAL);

                            inimigo->objeto = criarInimigoNormal(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                RED
                            );
                            el->objeto = inimigo;
                            el->tipo = ELEMENTO_MAPA_INIMIGO;
                            inserirInimigo(novoMapa, el);
                            break;

                        case '2':

                            inimigo = criarInimigo(INIMIGO_DASH);

                            inimigo->objeto = criarInimigoDash(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento * 2,
                                PURPLE
                            );
                            el->objeto = inimigo;
                            el->tipo = ELEMENTO_MAPA_INIMIGO;
                            inserirInimigo(novoMapa, el);
                            break;

                        case '3': //Inimigo voador na horizontal

                            inimigo = criarInimigo(INIMIGO_VOADOR);

                            inimigo->objeto = criarInimigoVoador(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                (Vector2) {novoMapa->tamanhoElemento * 3, 0},
                                (Vector2) {75, 0},
                                RED
                            );
                            el->objeto = inimigo;
                            el->tipo = ELEMENTO_MAPA_INIMIGO;
                            inserirInimigo(novoMapa, el);
                            break;

                        case '4': //Inimigo voador na vertical

                            inimigo = criarInimigo(INIMIGO_VOADOR);

                            inimigo->objeto = criarInimigoVoador(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                (Vector2) {0, novoMapa->tamanhoElemento * 3},
                                (Vector2) {0, 75},
                                RED
                            );
                            el->objeto = inimigo;
                            el->tipo = ELEMENTO_MAPA_INIMIGO;
                            inserirInimigo(novoMapa, el);
                            break;
                            
                        default:
                            free(el);
                            break;
                    }
                    

                } else if('a' <= *caractereAtual && *caractereAtual <= 'z') {

                    Item *item = NULL;

                    switch(*caractereAtual) {
                        case 'a':

                            item = criarItem(ITEM_MOEDA);

                            item->objeto = criarItemMoeda(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                YELLOW
                            );

                            el->objeto = item;
                            el->tipo = ELEMENTO_MAPA_ITEM;

                            inserirItem(novoMapa, el);

                            break;
                        case 'b':

                            item = criarItem(ITEM_MOEDA_ESPECIAL);

                            item->objeto = criarItemMoedaEspecial(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                ORANGE
                            );

                            el->objeto = item;
                            el->tipo = ELEMENTO_MAPA_ITEM;

                            inserirItem(novoMapa, el);

                            break;
                        case 'c':

                            item = criarItem(ITEM_VIDA);

                            item->objeto = criarItemVida(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                GREEN
                            );

                            el->objeto = item;
                            el->tipo = ELEMENTO_MAPA_ITEM;

                            inserirItem(novoMapa, el);

                            break;
                        default:
                            free(el);
                            break;
                    }


                } else if(*caractereAtual == '@') {

                    Jogador *jogador = criarJogador(
                        colunaAtual * novoMapa->tamanhoElemento,
                        linhaAtual * novoMapa->tamanhoElemento,
                        novoMapa->tamanhoElemento,
                        novoMapa->tamanhoElemento,
                        BLUE
                    );

                    if(novoMapa->jogador == NULL) {
                        novoMapa->jogador = jogador;
                    }

                }

            }

            colunaAtual++;
            if(novoMapa->colunas < colunaAtual) {
                novoMapa->colunas = colunaAtual;
            }

        }

        caractereAtual++;
    }

    novoMapa->linhas++;
    
    UnloadFileText( dadosMapa );

    return novoMapa;

}

void destruirMapa(Mapa *m) {
    
    if(m->jogador != NULL) {
        destruirJogador(m->jogador);
    }
   
    ElementoMapa *el = NULL;

    el = m->obstaculos;
    while(el != NULL) {
        destruirObstaculo((Obstaculo*) el->objeto);
        el = el->proximo;
    }

    el = m->inimigos;
    while(el != NULL) {
        desenharInimigo((Inimigo*) el->objeto);
        el = el->proximo;
    }

    el = m->itens;
    while(el != NULL) {
        destruirItem((Item*) el->objeto);
        el = el->proximo;
    }

}

void desenharMapa(Mapa *m) {
    
    ElementoMapa *el = NULL;

    el = m->obstaculos;
    
    while(el != NULL) {
        desenharObstaculo((Obstaculo*) el->objeto);
        el = el->proximo;
    }

    el = m->inimigos;
    while(el != NULL) {
        desenharInimigo((Inimigo*) el->objeto);
        el = el->proximo;
    }

    el = m->itens;
    while(el != NULL) {
        desenharItem((Item*) el->objeto);
        el = el->proximo;
    }

    if(m->jogador != NULL) {
        desenharJogador(m->jogador);
    }
    
}

void atualizarMapa(Mapa *m,GameWorld *gw, float delta ) {

    ElementoMapa *el = NULL;

    el = m->inimigos;
    while(el != NULL) {
        atualizarInimigo((Inimigo*) el->objeto, gw, delta);
        el = el->proximo;
    }

    el = m->itens;
    while(el != NULL) {
        atualizarItem((Item*) el->objeto, gw, delta);
        el = el->proximo;
    }

    el = m->obstaculos;
    while(el != NULL) {
        Obstaculo *o = (Obstaculo*) el->objeto;
        if(o->tipo == OBSTACULO_MOVEL) {
            atualizarObstaculoMovel((ObstaculoMovel*) o->objeto, gw, delta);
        }
        el = el->proximo;
    }

    if(m->jogador != NULL) {
        atualizarJogador(m->jogador, gw, delta);
        entradaJogador(m->jogador);
    }

}

int calcularLarguraMapa(Mapa *m) {

    return (int) (m->colunas * m->tamanhoElemento);

}

int calcularAlturaMapa(Mapa *m) {

    return (int) (m->linhas * m->tamanhoElemento);

}

static void inserirObstaculo(Mapa *m, ElementoMapa *obs) {

    if(m->obstaculos == NULL) {
        m->obstaculos = obs;
    } else {
        obs->proximo = m->obstaculos;
        m->obstaculos = obs;
    }
    m->quantidadeObstaculos++;
}

static void inserirInimigo(Mapa *m, ElementoMapa *ini) {

    if(m->inimigos == NULL) {
        m->inimigos = ini;
    } else {
        ini->proximo = m->inimigos;
        m->inimigos = ini;
    }
    m->quantidadeInimigos++;
}

static void inserirItem(Mapa *m, ElementoMapa *item) {

    if(m->itens == NULL) {
        m->itens = item;
    } else {
        item->proximo = m->itens;
        m->itens = item;
    }
    m->quantidadeItens++;
}