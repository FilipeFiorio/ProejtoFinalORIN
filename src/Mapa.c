#include <stdlib.h>
#include "raylib/raylib.h"
#include "Tipos.h"
#include "Mapa.h"
#include "Obstaculo.h"

Mapa *carregarMapa(const char *caminhoArquivo) {

    Mapa *novoMapa = (Mapa*) malloc(sizeof(Mapa));

    novoMapa->elementos = NULL;
    novoMapa->quantidadeElementos = 0;
    novoMapa->tamanhoElemento = 50;
    novoMapa->linhas = 0;
    novoMapa->colunas = 0;

    char *dadosMapa = LoadFileText(caminhoArquivo);
    
    char *caractereAtual = dadosMapa;
    int linhaAtual = 0;
    int colunaAtual = 0;

    while (*caractereAtual != '\0') {

        if(*caractereAtual == '\n') {
            linhaAtual++;
            colunaAtual = 0;

            novoMapa->linhas = linhaAtual;
        } else {

            Color cor = BLACK;
            bool criar = true;

            switch (*caractereAtual) {
            case 'a':
                cor = ORANGE;
                break;
            case 'g':
                cor = GREEN;
                break;
            case 'p':
                cor = GRAY;
                break;
            case 'c':
                cor = BROWN;
                break;
            default:
                criar = false;
                break;
            }

            if (criar) {

                ElementoMapa *el = (ElementoMapa*) malloc(sizeof(ElementoMapa));

                el->obstaculo = (Obstaculo) {
                    .ret =  {
                        .x = novoMapa->tamanhoElemento * colunaAtual,
                        .y = novoMapa->tamanhoElemento * linhaAtual,
                        .width = novoMapa->tamanhoElemento,
                        .height = novoMapa->tamanhoElemento,
                    },
                    .cor = cor
                };
                el->proximo = NULL;

                if(novoMapa->elementos == NULL) {
                    novoMapa->elementos = el;
                } else {
                    el->proximo = novoMapa->elementos;
                    novoMapa->elementos = el;
                }
                novoMapa->quantidadeElementos++;

            }

            colunaAtual++;

            if(novoMapa->colunas < colunaAtual) {
                novoMapa->colunas = colunaAtual;
            }

            
            
        }
        caractereAtual++;

        
        
    }

    novoMapa->linhas++;

    UnloadFileText(dadosMapa);

    return novoMapa;

}

void destruirMapa(Mapa *m) {

    ElementoMapa *el = m->elementos;

    while (el != NULL) {
        ElementoMapa *temp = el;    
        el = el->proximo;
        free(temp);
    }
    
}

void desenharMapa(Mapa *m) {
    
    ElementoMapa *el = m->elementos;

    while (el != NULL){
        Obstaculo *o = &el->obstaculo;
        desenharObstaculo(o);
        el = el->proximo;
    }
    
}

int calcularLarguraMapa(Mapa *m) {

    return (int) (m->colunas * m->tamanhoElemento);

}

int calcularAlturaMapa(Mapa *m) {

    return (int) (m->linhas * m->tamanhoElemento);

}