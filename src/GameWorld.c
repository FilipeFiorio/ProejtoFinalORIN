/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "ResourceManager.h"
#include "Tipos.h"
#include "Jogador.h"
#include "Inimigo.h"
#include "Mapa.h"


#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h


static void atualizarCamera(GameWorld *gw);
static void desenharFundo(GameWorld *gw);
static void verificarMorteJogador(GameWorld *gw);
static void verificarGameOver(GameWorld *gw);
static void verificarColisaoJogadorInimigo(GameWorld *gw);
static void verificarColisaoJogadorItem(GameWorld *gw);
static void reiniciarJogo(GameWorld *gw);
static void inicializarGW(GameWorld *gw);


/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    inicializarGW(gw);

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    destruirJogador(gw->jogador);
    destruirMapa(gw->mapa);
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    entradaJogador(gw->jogador);
    atualizarJogador(gw->jogador, gw, delta);
    atualizarMapa(gw->mapa, gw, delta);
    verificarMorteJogador(gw);
    verificarColisaoJogadorInimigo(gw);
    verificarColisaoJogadorItem(gw);

    atualizarCamera(gw);

    verificarGameOver(gw);

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();

    ClearBackground( (Color) {167, 222, 255, 255} );

    BeginMode2D(gw->camera);

    desenharFundo(gw);

    desenharMapa(gw->mapa);

    desenharJogador(gw->jogador);

    EndMode2D();

    char textoVidas[10];
    sprintf(textoVidas, "vidas: %d", gw->jogador->vidas);
    DrawText(textoVidas,10, 10, 20, WHITE);

    char textoMoedas[10];
    sprintf(textoMoedas, "moedas: %d", gw->jogador->moedas);
    DrawText(textoMoedas,100, 10, 20, WHITE);
    
    char posJogador[30];
    sprintf(posJogador, "posicao: x: %d, y: %d", (int) gw->jogador->ret.x, (int) gw->jogador->ret.y);
    DrawText(posJogador,10, 30, 20, BLACK);

    EndDrawing();

}

static void desenharFundo( GameWorld *gw ) {

    int larguraFundo = rm.texturaFundo.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int alturaMapa = calcularAlturaMapa( gw->mapa );
    int repeticoes = larguraMapa / larguraFundo;

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * 200 );

    for ( int i = 0; i <= repeticoes + 1; i++ ) {
        DrawTexture( rm.texturaFundo, larguraFundo * i - deslocamentoParallax, alturaMapa - rm.texturaFundo.height, WHITE );
    }

}

static void atualizarCamera(GameWorld *gw) {

    Jogador *jogador = gw->jogador;
    Camera2D *camera = &gw->camera;

    camera->offset.x = GetScreenWidth() / 2;
    camera->offset.y = GetScreenHeight() / 2;

    camera->target.x = jogador->ret.x + jogador->ret.width;
    camera->target.y = jogador->ret.y + jogador->ret.height;

    int minX = GetScreenWidth() / 2 + 50;
    int maxX = calcularLarguraMapa(gw->mapa) - GetScreenWidth() / 2 - 50;
    int maxY = calcularAlturaMapa(gw->mapa) - GetScreenHeight() / 2;

    if(camera->target.x < minX) {
        camera->target.x = minX;
    } else if (camera->target.x > maxX) {
        camera->target.x = maxX;
    }

    if(camera->target.y > maxY) {
        camera->target.y = maxY;
    }

}

static void verificarMorteJogador(GameWorld *gw) {

    Jogador *jogador = gw->jogador;
    int alturaMapa = calcularAlturaMapa(gw->mapa);

    if(jogador->ret.y > alturaMapa) {
        jogador->vidas--; 
        reiniciarJogo(gw);
        return;
    }

}

//no futuro usar estados do jogo --> ESTADO_JOGO_GAME_OVER
static void verificarGameOver(GameWorld *gw) {

    if(gw->jogador->vidas <= 0) {
        //ir para a tela de gameOver
        //Opcao de sair do jogo, ou tentar de novo
    }

}

static void reiniciarJogo(GameWorld *gw) {

    destruirJogador(gw->jogador);
    destruirMapa(gw->mapa);

    inicializarGW(gw);
    
}


static void inicializarGW(GameWorld *gw) {

    gw->mapa = carregarMapa("resources/mapas/fase01.txt");
    gw->jogador = criarJogador(GetScreenWidth() / 2 - 100, calcularAlturaMapa(gw->mapa) - 150, 50, 50, BLUE);
    gw->gravidade = 600;

    gw->camera = (Camera2D) {
        .offset = {0},
        .target = {0},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

}


static void verificarColisaoJogadorInimigo(GameWorld *gw) {

    Jogador *j = gw->jogador;
    ElementoMapa *el = gw->mapa->inimigos;

    // Atualmente verifica apenas se há colisão pela esquerda, direita e por cima
    // por cima apenas ve se a vel.x é maior que zero
    // se for > 0 o jogador está em cima do inimigo

    while (el != NULL) {

        Inimigo *i = (Inimigo*) el->objeto;

        if(CheckCollisionRecs(j->ret, i->ret) && i->estaVivo) {
            if(j->vel.y > 0) {
                i->estaVivo = false;  
                j->vel.y = j->velPulo * 0.75;   
            } else {
                if(j->ret.x + j->ret.width / 2 > i->ret.x + i->ret.width / 2) {
                    j->ret.x = i->ret.x + i->ret.width;
                    j->vidas--;
                } else if(j->ret.x + j->ret.width / 2 < i->ret.x + i->ret.width / 2 ) {
                    j->ret.x = i->ret.x - i->ret.width;
                    j->vidas--;
                }
            }
            return;
        }

        el = el->proximo;
    }

}

static void verificarColisaoJogadorItem(GameWorld *gw) {

    Jogador *j = gw->jogador;
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


