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
#include "Mapa.h"


#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h


static void atualizarCamera(GameWorld *gw);

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->mapa = carregarMapa("resources/mapas/fase01.txt");
    gw->jogador = criarJogador(GetScreenWidth() / 2, calcularAlturaMapa(gw->mapa) - 150, 50, 50, BLUE);
    gw->gravidade = 500;

    gw->camera = (Camera2D) {
        .offset = {0},
        .target = {0},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

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
    atualizarCamera(gw);

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();

    ClearBackground( PINK );

    BeginMode2D(gw->camera);

    desenharMapa(gw->mapa);
    desenharJogador(gw->jogador);

    EndMode2D();

    EndDrawing();

}

static void atualizarCamera(GameWorld *gw) {

    Jogador *jogador = gw->jogador;
    Camera2D *camera = &gw->camera;

    camera->offset.x = GetScreenWidth() / 2;
    camera->offset.y = GetScreenHeight() / 2;

    camera->target.x = jogador->ret.x + jogador->ret.width;
    camera->target.y = jogador->ret.y + jogador->ret.height;

    int minX = GetScreenWidth() / 2;
    int maxX = calcularLarguraMapa(gw->mapa) - GetScreenWidth() / 2;
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