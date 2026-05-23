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

typedef enum TipoElementoMapa {
    ELEMENTO_MAPA_OBSTACULO,
    ELEMENTO_MAPA_INIMIGO,
    ELEMENTO_MAPA_ITEM
} TipoElementoMapa;

typedef struct Jogador {
    Rectangle ret;
    Vector2 vel;
    Color cor;
    int vidas;
    int moedas;
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
    bool noChao;
} Inimigo;

typedef struct Obstaculo {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} Obstaculo;

typedef struct Item {
    Rectangle ret;
    Color cor;
    bool ativo;
} Item;

typedef struct ElementoMapa ElementoMapa;
struct ElementoMapa {
    void *objeto;
    TipoElementoMapa tipo;
    ElementoMapa *proximo;
};

typedef struct Mapa {
    ElementoMapa *obstaculos;
    int quantidadeObstaculos;

    ElementoMapa *inimigos;
    int quantidadeInimigos;

    ElementoMapa *itens;
    int quantidadeItens;

    float tamanhoElemento;
    int linhas;
    int colunas;
} Mapa;

typedef struct GameWorld {
    Mapa *mapa;
    Jogador *jogador;
    Camera2D camera;
    float gravidade;
}GameWorld;

