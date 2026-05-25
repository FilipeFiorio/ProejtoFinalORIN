#pragma once

#include "Tipos.h"

InimigoVoador *criarInimigoVoador(float x, float y, float largura, float altura, Vector2 deslocamento, Vector2 vel, Color cor);
void atualizarInimigoVoador(InimigoVoador *inimigo, GameWorld *gw, float delta);
void destruirInimigoVoador(InimigoVoador *inimigo);
void desenharInimigoVoador(InimigoVoador *inimigo);