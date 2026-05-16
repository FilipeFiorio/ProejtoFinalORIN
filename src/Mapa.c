#include <stdlib.h>
#include "raylib/raylib.h"
#include "ResourceManager.h"
#include "Tipos.h"
#include "Mapa.h"
#include "Obstaculo.h"

Mapa *carregarMapa( const char *caminhoArquivo ) {

    // aloca um novo mapa
    Mapa *novoMapa = (Mapa*) malloc( sizeof( Mapa ) );
    novoMapa->elementos = NULL;
    novoMapa->quantidadeElementos = 0;
    novoMapa->tamanhoElemento = 48;
    novoMapa->linhas = 0;
    novoMapa->colunas = 0;
    
    // carrega dados do arquivo de texto
    char *dadosMapa = LoadFileText( caminhoArquivo );

    // marcadores para processamento do mapa
    char *caractereAtual = dadosMapa;
    int linhaAtual = 0;
    int colunaAtual = 0;

    // caractere atual marca inicialmente a primeira posição de dadosMapa
    // C-strings terminam em '\0', sendo assim, caminhamos caractere por 
    // caractere até o fim
    while ( *caractereAtual != '\0' ) {

        // fim de linha?
        if ( *caractereAtual == '\n' ) {

            linhaAtual++;
            colunaAtual = 0;

            novoMapa->linhas = linhaAtual;

        } else {

            bool criar = *caractereAtual != ' ';
            int deslocamento = *caractereAtual - 'A';

            if ( criar ) {

                ElementoMapa *el = (ElementoMapa*) malloc( sizeof( ElementoMapa ) );

                el->obstaculo = (Obstaculo) {
                    .ret = { 
                        .x = novoMapa->tamanhoElemento * colunaAtual, 
                        .y = novoMapa->tamanhoElemento * linhaAtual, 
                        .width = novoMapa->tamanhoElemento, 
                        .height = novoMapa->tamanhoElemento
                    },
                    .cor = GRAY,
                    .fonte = { 
                        1 + ( novoMapa->tamanhoElemento + 1 ) * deslocamento, 
                        1, 
                        novoMapa->tamanhoElemento,
                        novoMapa->tamanhoElemento
                    },
                    .textura = &rm.texturaTerreno
                };
                el->proximo = NULL;

                // inserção na lista
                if ( novoMapa->elementos == NULL ) {
                    novoMapa->elementos = el;
                } else {
                    el->proximo = novoMapa->elementos;
                    novoMapa->elementos = el;
                }
                novoMapa->quantidadeElementos++;

            }

            colunaAtual++;

            if ( novoMapa->colunas < colunaAtual ) {
                novoMapa->colunas = colunaAtual;
            }

        }

        caractereAtual++;

    }

    novoMapa->linhas++;
    
    // descarrega os dados
    UnloadFileText( dadosMapa );

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