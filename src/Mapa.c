#include <stdlib.h>
#include "raylib/raylib.h"
#include "ResourceManager.h"
#include "Tipos.h"
#include "Mapa.h"
#include "Obstaculo.h"
#include "Inimigo.h"
#include "Item.h"

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

                    el->objeto = criarObstaculo(
                        (Rectangle) {
                            .x = colunaAtual * novoMapa->tamanhoElemento,
                            .y = linhaAtual * novoMapa->tamanhoElemento,
                            .width = novoMapa->tamanhoElemento,
                            .height = novoMapa->tamanhoElemento
                        },
                        (Rectangle) {
                            .x = 1 + (novoMapa->tamanhoElemento + 1) * deslocamento,
                            .y = 1,
                            .width = novoMapa->tamanhoElemento,
                            .height = novoMapa->tamanhoElemento
                        },
                        &rm.texturaTerreno,
                        GRAY
                    );
                    el->tipo = ELEMENTO_MAPA_OBSTACULO;
                    inserirObstaculo(novoMapa, el);

                } else if('1' <= *caractereAtual && *caractereAtual <= '9') {

                    Inimigo *inimigo = NULL;

                    switch(*caractereAtual) {
                        case '1':
                            inimigo = criarInimigo(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                RED
                            );
                            el->objeto = inimigo;
                            el->tipo = ELEMENTO_MAPA_INIMIGO;
                            break;
                        default:
                            free(el);
                            break;
                    }
                    
                    inserirInimigo(novoMapa, el);

                } else if('a' <= *caractereAtual && *caractereAtual <= 'z') {

                    Item *item = NULL;

                    switch(*caractereAtual) {
                        case 'a':
                            item = criarItem(
                                colunaAtual * novoMapa->tamanhoElemento,
                                linhaAtual * novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                novoMapa->tamanhoElemento,
                                YELLOW
                            );
                            el->objeto = item;
                            el->tipo = ELEMENTO_MAPA_ITEM;
                            break;
                        default:
                            free(el);
                            break;
                    }

                    inserirItem(novoMapa, el);
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
   //fazer 
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