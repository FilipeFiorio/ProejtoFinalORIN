#pragma once

#include "Tipos.h"

InimigoFantasma *criarInimigoFantasma(float x, float y, float largura, float altura, Color cor);
void atualizarInimigoFantasma(InimigoFantasma *inimigo, GameWorld *gw, float delta);
void destruirInimigoFantasma(InimigoFantasma *inimigo);
void desenharInimigoFantasma(InimigoFantasma *inimigo);
QuadroAnimacao *getQuadroAnimacaoAtualInimigoFantasma(InimigoFantasma *inimigo);