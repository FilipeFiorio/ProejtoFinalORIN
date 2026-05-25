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
    destruirMapa(gw->mapa);
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {
    
    gw->timerJogo -= (int) (1000 * delta);

    if(gw->timerJogo <= 0) {
        gw->mapa->jogador->vidas--;
    }

    atualizarMapa(gw->mapa, gw, delta);
    verificarMorteJogador(gw);

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


    EndMode2D();

    char textoVidas[10];
    sprintf(textoVidas, "Vidas: %d", gw->mapa->jogador->vidas);
    DrawText(textoVidas,10, 10, 24, WHITE);

    char textoMoedas[10];
    sprintf(textoMoedas, "Moedas: %d", gw->mapa->jogador->moedas);
    DrawText(textoMoedas,150, 10, 24, WHITE);

    char textoTimer[10];
    sprintf(textoTimer, "Tempo: %d", gw->timerJogo / 1000);
    DrawText(textoTimer,300, 10, 24, WHITE);
    
    char posJogador[30];
    sprintf(posJogador, "posicao: x: %d, y: %d", (int) gw->mapa->jogador->ret.x, (int) gw->mapa->jogador->ret.y);
    DrawText(posJogador,10, 50, 20, BLACK);

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

    Jogador *jogador = gw->mapa->jogador;
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

    Jogador *jogador = gw->mapa->jogador;
    int alturaMapa = calcularAlturaMapa(gw->mapa);

    if(jogador->ret.y > alturaMapa) {
        jogador->vidas--; 
        reiniciarJogo(gw);
        return;
    }

}

//no futuro usar estados do jogo --> ESTADO_JOGO_GAME_OVER
static void verificarGameOver(GameWorld *gw) {

    if(gw->mapa->jogador->vidas <= 0) {
        //ir para a tela de gameOver
        //Opcao de sair do jogo, ou tentar de novo
    }

}

static void reiniciarJogo(GameWorld *gw) {

    destruirJogador(gw->mapa->jogador);
    destruirMapa(gw->mapa);

    inicializarGW(gw);
    
}


static void inicializarGW(GameWorld *gw) {

    gw->mapa = carregarMapa("resources/mapas/fase01.txt");
    gw->gravidade = 600;
    gw->timerJogo = 300000;

    gw->camera = (Camera2D) {
        .offset = {0},
        .target = {0},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

}