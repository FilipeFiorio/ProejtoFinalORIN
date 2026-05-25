#pragma once

#include "Tipos.h"

InimigoNormal *criarInimigoNormal(float x, float y, float largura, float altura, Color cor);
void atualizarInimigoNormal(InimigoNormal *inimigo, GameWorld *gw, float delta);
void destruirInimigoNormal(InimigoNormal *inimigo);
void desenharInimigoNormal(InimigoNormal *inimigo);