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
#include "MapaMundo.h"
#include "Utils.h"


#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h


static void atualizarCamera(GameWorld *gw);
static void desenharFundo(GameWorld *gw);
static void verificarMorteJogador(GameWorld *gw);
static void verificarGameOver(GameWorld *gw);
static void reiniciarJogo(GameWorld *gw);
static void inicializarGW(GameWorld *gw);
static void desenharHud(GameWorld *gw);
static void passarFase(GameWorld *gw);

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

    if (gw->mapa != NULL) {
        destruirMapa(gw->mapa);
    }

    if (gw->mapaMundo != NULL) {
        destruirMapaMundo(gw->mapaMundo);
    }

    free(gw);

}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    switch (gw->estado) {

        case ESTADO_JOGO_GAMEPLAY:

            if(IsKeyPressed(KEY_P)) {
                gw->estado = ESTADO_JOGO_PAUSE;
            }

            gw->timerJogo -= (int) (1000 * delta);
        
            if(gw->timerJogo <= 0) {
                gw->mapa->jogador->morto = true;
            }
        
            atualizarMapa(gw->mapa, gw, delta);

            if (gw->mapa->faseCompleta) {
                gw->mapaMundo->fases[gw->faseAtual - 1].finalizado = true; // faseAtual é 1-based
            }

            verificarMorteJogador(gw);
            atualizarCamera(gw);
            verificarGameOver(gw);

            if(gw->mapaMundo->fases[gw->faseAtual - 1].finalizado) {
                passarFase(gw);
            }

            break;

        case ESTADO_JOGO_MAPA_MUNDO:

            atualizarMapaMundo(gw, delta);

            break;
        
        case ESTADO_JOGO_GAME_OVER:

            if(IsKeyPressed(KEY_ENTER)) {
                inicializarGW(gw);
                gw->estado = ESTADO_JOGO_GAMEPLAY;
            }
            
            break;

        case ESTADO_JOGO_INICIO:

            if(IsKeyPressed(KEY_ENTER)) {
                gw->estado = ESTADO_JOGO_MAPA_MUNDO;
            }

            break;

        case ESTADO_JOGO_PAUSE:

            if(IsKeyPressed(KEY_P)) {
                gw->estado = ESTADO_JOGO_GAMEPLAY;
            }

            break;

        case ESTADO_JOGO_FIM:

            if(IsKeyPressed(KEY_ENTER)){
                gw->estado = ESTADO_JOGO_INICIO;
            }

            break;
        
        case ESTADO_JOGO_FADE_IN:
            
            gw->alphaTransicao -= 5;

            if (gw->alphaTransicao <= 0) {
                gw->alphaTransicao = 0;
                gw->estado = ESTADO_JOGO_GAMEPLAY;
            }

            break;
        
        case ESTADO_JOGO_FADE_OUT:

            gw->alphaTransicao += 5;

            if (gw->alphaTransicao >= 255) {
                gw->alphaTransicao = 255;
                gw->estado = ESTADO_JOGO_FADE_IN;
            }

            break;

        default:
            break;
    }
    

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {
    
    BeginDrawing();

    DrawFPS(GetScreenWidth() - 100, 50);

    switch (gw->estado) {

        case ESTADO_JOGO_GAMEPLAY:

            ClearBackground( (Color) {175, 231, 255, 255} );
        
            BeginMode2D(gw->camera);

            desenharFundo(gw);
            desenharMapa(gw->mapa);

            EndMode2D();

            desenharHud(gw);

            break;

        case ESTADO_JOGO_MAPA_MUNDO:

            desenharMapaMundo(gw->mapaMundo);

            break;
        
        case ESTADO_JOGO_GAME_OVER:

            ClearBackground(BLACK);
            drawTextAlinhado("GAME OVER", 200, 50, WHITE, CENTRO);
            drawTextAlinhado("Aperte [ENTER] para continuar", 400, 25, WHITE, CENTRO);

            break;

        case ESTADO_JOGO_INICIO:

            ClearBackground(WHITE);

            DrawTexture(rm.texturaInicio, 0, 0, WHITE);

            drawTextAlinhado("Mr. Guzão", 200, 72, WHITE, CENTRO);
            drawTextAlinhado("Aperte [ENTER] para iniciar", 500, 25, WHITE, CENTRO);

            break;

        case ESTADO_JOGO_PAUSE:    

            ClearBackground( (Color) {175, 231, 255, 255} );

            drawTextAlinhado("Jogo Pausado", 200, 25, WHITE, CENTRO);
            desenharHud(gw);

            BeginMode2D(gw->camera);

            desenharFundo(gw);
            desenharMapa(gw->mapa);
            
            EndMode2D();

            break;

        case ESTADO_JOGO_FIM:

            ClearBackground(BLACK);

            drawTextAlinhado("Você Venceu!!!", 200, 50, WHITE, CENTRO);
            drawTextAlinhado("Aperte ENTER para voltar para o início", 400, 25, WHITE, CENTRO);

            break;

        case ESTADO_JOGO_FADE_IN:
        case ESTADO_JOGO_FADE_OUT:

            DrawRectangle(
                0,
                0,
                GetScreenWidth(),
                GetScreenHeight(),
                (Color){0,0,0,gw->alphaTransicao}
            );

            drawTextAlinhado("Carregando...", 650, 25, WHITE, DIREITA);

            break;

        default:


            break;



    }

    EndDrawing();

}

static void desenharFundo( GameWorld *gw ) {

    Texture2D fundo = {0};

    switch (gw->faseAtual) {
        case 1:
            fundo = rm.texturaFundo;
            break;
        case 2:
            fundo = rm.texturaFundoNeve;
            break;
        case 3:
            fundo = rm.texturaFundoDeserto;
            break;
        default:
            break;
    }

    int larguraFundo = fundo.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int alturaMapa = calcularAlturaMapa( gw->mapa );
    int repeticoes = larguraMapa / larguraFundo;

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * 200 );

    for ( int i = 0; i <= repeticoes + 1; i++ ) {
        DrawTexture( fundo, larguraFundo * i - deslocamentoParallax, alturaMapa - fundo.height, WHITE );
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

    Jogador *j = gw->mapa->jogador;

    bool foraDoMapa = (j->ret.y > calcularAlturaMapa(gw->mapa));

    if(foraDoMapa || j->morto) {
        j->vidas--;
        if(j->vidas >= 0) {
            reiniciarJogo(gw);
            j->morto = false;
        }
    } 

}

static void verificarGameOver(GameWorld *gw) {

    if(gw->mapa->jogador->vidas < 0) {
        destruirMapa(gw->mapa);
        gw->mapa = NULL;
        gw->estado = ESTADO_JOGO_GAME_OVER;
    }

}

static void reiniciarJogo(GameWorld *gw) {

    Jogador *j = gw->mapa->jogador;

    int vidaAtual = j->vidas;
    int moedaAtual = j->moedas;
    int faseAtual = gw->faseAtual;

    destruirMapa(gw->mapa);

    char caminhoMapa[100];
    sprintf(caminhoMapa, "resources/mapas/fase%d.txt", faseAtual);
    gw->mapa = carregarMapa(caminhoMapa);

    gw->gravidade = 600;
    gw->timerJogo = 200000;
    gw->faseAtual = faseAtual;
    gw->estado = ESTADO_JOGO_GAMEPLAY;
    
    gw->mapa->jogador->vidas = vidaAtual;
    gw->mapa->jogador->moedas = moedaAtual;
    
}

static void inicializarGW(GameWorld *gw) {

    if (gw->mapaMundo != NULL) {
        destruirMapaMundo(gw->mapaMundo);
    }

    
    gw->faseAtual = 1;
    gw->mapaMundo = criarMapaMundo(3);
    gw->gravidade = 600;
    gw->timerJogo = 200000;
    gw->alphaTransicao = 0;
    gw->mapa = NULL;
    gw->estado = ESTADO_JOGO_INICIO;
    gw->vidasSalvas = 5;
    gw->moedasSalvas = 0;

    gw->camera = (Camera2D) {
        .offset = {0},
        .target = {0},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

}

static void desenharHud(GameWorld *gw) {

    char textoHudJogador[50];
    char textoHudTempo[50];
    sprintf(textoHudJogador, "Vidas: %d   Moedas: %d", gw->mapa->jogador->vidas, gw->mapa->jogador->moedas);
    sprintf(textoHudTempo, "Tempo: %ds", gw->timerJogo / 1000);
    drawTextAlinhado(textoHudJogador, 10, 20, WHITE, ESQUERDA);
    drawTextAlinhado(textoHudTempo, 10, 20, WHITE, CENTRO);

}

static void passarFase(GameWorld *gw) {

    gw->vidasSalvas = gw->mapa->jogador->vidas;
    gw->moedasSalvas = gw->mapa->jogador->moedas;

    destruirMapa(gw->mapa);
    gw->mapa = NULL;

    gw->faseAtual++;

    if(gw->faseAtual > 3) {
        gw->estado = ESTADO_JOGO_FIM;
        return;
    }

    gw->timerJogo = 200000;
    gw->alphaTransicao = 0;
    gw->estado = ESTADO_JOGO_MAPA_MUNDO;
}
