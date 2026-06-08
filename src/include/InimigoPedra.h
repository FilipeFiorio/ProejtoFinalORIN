#pragma once

#include "Tipos.h"

InimigoEspinho *criarInimigoEspinho(float x, float y, float largura, float altura, Color cor);
void atualizarInimigoEspinho(InimigoEspinho *inimigo, GameWorld *gw, float delta);
void destruirInimigoEspinho(InimigoEspinho *inimigo);
void desenharInimigoEspinho(InimigoEspinho *inimigo);
QuadroAnimacao *getQuadroAnimacaoAtualInimigoEspinho(InimigoEspinho *inimigo);