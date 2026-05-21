#include <stdlib.h>
#include "raylib/raylib.h"
#include "ResourceManager.h"
#include "Tipos.h"
#include "Mapa.h"
#include "Obstaculo.h"
#include "Inimigo.h"

static void inserirObstaculo(Mapa *m, ElementoMapa *o);
static void inserirInimigo(Mapa *m, ElementoMapa *i);

Mapa *carregarMapa( const char *caminhoArquivo ) {

    Mapa *novoMapa = (Mapa*) malloc( sizeof( Mapa ) );
    novoMapa->obstaculos = NULL;
    novoMapa->quantidadeObstaculos = 0;
    novoMapa->inimigos = NULL;
    novoMapa->quantidadeInimigos = 0;
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
                            free(el); // evita leak para caracteres não tratados
                            el = NULL;
                            break;
                    }

                    if(el != NULL) {
                        inserirInimigo(novoMapa, el);
                    }

                } else {
                    free(el); // caractere desconhecido, descarta
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
    
}

void atualizarMapa(Mapa *m,GameWorld *gw, float delta ) {

    ElementoMapa *el = NULL;

    el = m->inimigos;
    while(el != NULL) {
        atualizarInimigo((Inimigo*) el->objeto, gw, delta);
        el = el->proximo;
    }

}

int calcularLarguraMapa(Mapa *m) {

    return (int) (m->colunas * m->tamanhoElemento);

}

int calcularAlturaMapa(Mapa *m) {

    return (int) (m->linhas * m->tamanhoElemento);

}

static void inserirObstaculo(Mapa *m, ElementoMapa *o) {

    if(m->obstaculos == NULL) {
        m->obstaculos = o;
    } else {
        o->proximo = m->obstaculos;
        m->obstaculos = o;
    }
    m->quantidadeObstaculos++;
}

static void inserirInimigo(Mapa *m, ElementoMapa *i) {

    if(m->inimigos == NULL) {
        m->inimigos = i;
    } else {
        i->proximo = m->inimigos;
        m->inimigos = i;
    }
    m->quantidadeInimigos++;
}