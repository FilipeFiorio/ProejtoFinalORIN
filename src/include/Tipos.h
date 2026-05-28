#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

typedef enum EstadoJogo {
    ESTADO_JOGO_INICIO,
    ESTADO_JOGO_WORLD_MAP,
    ESTADO_JOGO_GAMEPLAY,
    ESTADO_JOGO_PAUSE,
    ESTADO_JOGO_GAME_OVER,
    ESTADO_JOGO_FIM
} EstadoJogo;

typedef enum TipoElementoMapa {
    ELEMENTO_MAPA_OBSTACULO,
    ELEMENTO_MAPA_INIMIGO,
    ELEMENTO_MAPA_ITEM,
    ELEMENTO_MAPA_JOGADOR
} TipoElementoMapa;

typedef enum TipoObstaculo {
    OBSTACULO_NORMAL,
    OBSTACULO_MOVEL,
    OBSTACULO_CHEGADA 
}TipoObstaculo;

typedef enum TipoInimigo {
    INIMIGO_NORMAL,
    INIMIGO_VOADOR,
    INIMIGO_DASH
}TipoInimigo;

typedef enum TipoItem {
    ITEM_MOEDA,
    ITEM_MOEDA_ESPECIAL,
    ITEM_VIDA
} TipoItem;

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
    bool morto;
} Jogador;

typedef struct Inimigo {
    void *objeto;
    TipoInimigo tipo;
} Inimigo;

typedef struct InimigoNormal {
    Rectangle ret;
    Color cor;
    Vector2 vel;
    float velMaxQueda;
    bool estaVivo;
    bool noChao;
} InimigoNormal;

typedef struct InimigoDash {
    Rectangle ret;
    Color cor;
    Vector2 vel;
    float tamanhoDash;
    float velMaxQueda;
    float velXInicial;
    bool estaVivo;
    bool noChao;
} InimigoDash;

typedef struct InimigoVoador {
    Rectangle ret;
    Color cor;
    Vector2 vel;
    Vector2 posInicial;
    Vector2 deslocamento;
    bool estaVivo;
    bool retornando;
}InimigoVoador;

typedef struct Obstaculo {
    void *objeto;
    TipoObstaculo tipo;
} Obstaculo;

typedef struct ObstaculoNormal {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} ObstaculoNormal;

typedef struct ObstaculoMovel {
    Rectangle ret;
    Vector2 posInicial;
    Vector2 deslocamento;
    Vector2 vel;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
    bool retornando;
} ObstaculoMovel;

typedef struct ObstaculoChegada {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} ObstaculoChegada;

typedef struct Item {
    void *objeto;
    TipoItem tipo;
} Item;

typedef struct ItemMoeda {
    Rectangle ret;
    Color cor;
    int valor;
    bool ativo;
} ItemMoeda;

typedef struct ItemMoedaEspecial {
    Rectangle ret;
    Color cor;
    int valor;
    bool ativo;
}ItemMoedaEspecial;

typedef struct ItemVida{
    Rectangle ret;
    Color cor;
    bool ativo;
}ItemVida;

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

    Jogador *jogador;

    float tamanhoElemento;
    int linhas;
    int colunas;
} Mapa;

typedef struct GameWorld {
    Mapa *mapa;
    Camera2D camera;
    EstadoJogo estado;
    float gravidade;
    int timerJogo;
}GameWorld;

