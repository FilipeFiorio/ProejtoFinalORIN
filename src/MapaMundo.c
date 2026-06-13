#include <stdlib.h>
#include <stdio.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "MapaMundo.h"
#include "Mapa.h"
#include "ResourceManager.h"

MapaMundo *criarMapaMundo(int quantidadeFases) {

    MapaMundo *novoMapaMundo = (MapaMundo*) malloc( sizeof(MapaMundo));

    novoMapaMundo->quantidadeFases = quantidadeFases;

    novoMapaMundo->fases = (NodeMapa*) malloc(sizeof(NodeMapa) * novoMapaMundo->quantidadeFases);

    novoMapaMundo->fases[0] = (NodeMapa) {
        {212, 380},
        true,
        false,
        1
    };

    novoMapaMundo->fases[1] = (NodeMapa) {
        {632, 397},
        false,
        false,
        2
    };
    
    novoMapaMundo->fases[2] = (NodeMapa) {
        {1033, 387},
        false,
        false,
        3
    };

    novoMapaMundo->faseAtual = 0;

    novoMapaMundo->jogador = (Rectangle) {210, 380, 48, 48};

    return novoMapaMundo;

}

void desenharMapaMundo(MapaMundo *mapaMundo) {

    DrawTexturePro( 
        rm.texturaMapaMundo, 
        (Rectangle) {
            0, 0,
            rm.texturaMapaMundo.width, rm.texturaMapaMundo.height
        },
        (Rectangle) {
            0, 0,
            GetScreenWidth(), GetScreenHeight()
        },
        (Vector2) {0},
        0.0f,
        WHITE
    );

    DrawTexture(rm.texturaJogadorMapa, mapaMundo->jogador.x, mapaMundo->jogador.y, WHITE);


}

void destruirMapaMundo(MapaMundo *mapaMundo) {

    if(mapaMundo != NULL) {
        free(mapaMundo->fases);
        free(mapaMundo);
    }
}

void atualizarMapaMundo(GameWorld *gw, float delta) {


    MapaMundo *mapaMundo = gw->mapaMundo;

    if (gw->mapa != NULL && gw->mapa->faseCompleta) {
        int idxFase = gw->faseAtual - 1; 
        mapaMundo->fases[idxFase].finalizado = true;
    }

    for(int i = 0; i < mapaMundo->quantidadeFases - 1; i++) {
        if(mapaMundo->fases[i].finalizado) {
            mapaMundo->fases[i+1].liberado = true;
        }
    }
    
    //Andar entre fases
    if(IsKeyPressed(KEY_D)) {

        int proximaFase = mapaMundo->faseAtual + 1;

        if(proximaFase < mapaMundo->quantidadeFases && mapaMundo->fases[proximaFase].liberado) {
            mapaMundo->faseAtual = proximaFase;
            mapaMundo->jogador.x = mapaMundo->fases[proximaFase].pos.x;
            mapaMundo->jogador.y = mapaMundo->fases[proximaFase].pos.y;
        }
    
    } else if(IsKeyPressed(KEY_A)) {
    
        int faseAnterior = mapaMundo->faseAtual - 1;

        if(faseAnterior >= 0) {
            mapaMundo->faseAtual = faseAnterior;
            mapaMundo->jogador.x = mapaMundo->fases[faseAnterior].pos.x;
            mapaMundo->jogador.y = mapaMundo->fases[faseAnterior].pos.y;
        }

    }

    //Entrar numa fase
    if(IsKeyPressed(KEY_ENTER)) {
        gw->faseAtual = mapaMundo->fases[mapaMundo->faseAtual].fase;

        mapaMundo->fases[mapaMundo->faseAtual].finalizado = false;

        char caminhoMapa[100];
        sprintf(caminhoMapa, "resources/mapas/fase%d.txt", gw->faseAtual);
        gw->mapa = carregarMapa(caminhoMapa);

        gw->mapa->jogador->vidas = gw->vidasSalvas;
        gw->mapa->jogador->moedas = gw->moedasSalvas;

        gw->estado = ESTADO_JOGO_FADE_OUT;
    }
    

}