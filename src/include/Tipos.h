#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

typedef enum EstadoJogo {
    ESTADO_JOGO_MENU,
    ESTADO_JOGO_WORLD_MAP,
    ESTADO_JOGO_GAMEPLAY,
    ESTADO_JOGO_PAUSE,
    ESTADO_JOGO_GAME_OVER

} EstadoJogo;

typedef struct Jogador {
    Rectangle ret;
    Vector2 vel;
    Color cor;
    int vidas;
    float velAndando;
    float velCorrendo;
    float velPulo;
    float velPuloCorrendo;
    float velMaxQueda;
    bool noChao;
} Jogador;

typedef struct Inimigo {
    Rectangle ret;
    Color cor;
    Vector2 vel;
    float velMaxQueda;
    bool estaVivo;
} Inimigo;

typedef struct Obstaculo {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} Obstaculo;

typedef struct ElementoMapa ElementoMapa;
struct ElementoMapa {
    Obstaculo obstaculo;
    ElementoMapa *proximo;
};

typedef struct Mapa {
    ElementoMapa *elementos;
    int quantidadeElementos;
    float tamanhoElemento;
    int linhas;
    int colunas;
} Mapa;

typedef struct GameWorld {
    Mapa *mapa;
    Jogador *jogador;
    Camera2D camera;
    Inimigo *inimigo;
    float gravidade;
}GameWorld;

