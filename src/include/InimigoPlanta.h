#pragma once

#include "Tipos.h"

InimigoPlanta *criarInimigoPlanta(float x, float y, float largura, float altura, Color cor);
void atualizarInimigoPlanta(InimigoPlanta *inimigo, GameWorld *gw, float delta);
void destruirInimigoPlanta(InimigoPlanta *inimigo);
void desenharInimigoPlanta(InimigoPlanta *inimigo);
QuadroAnimacao *getQuadroAnimacaoAtualInimigoPlanta(InimigoPlanta *inimigo);