#pragma once

#include "raylib/raylib.h"
#include "Tipos.h"

Jogador *criarJogador(float x, float y, float largura, float altura, Color cor);
void entradaJogador(Jogador *j);
void destruirJogador(Jogador *j);
void atualizarJogador(Jogador *j, GameWorld *gw, float delta);
void desenharJogador(Jogador *j);