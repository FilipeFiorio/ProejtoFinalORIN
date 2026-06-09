#pragma once

#include "Tipos.h"

InimigoPlantaGelo *criarInimigoPlantaGelo(float x, float y, float largura, float altura, Color cor);
void atualizarInimigoPlantaGelo(InimigoPlantaGelo *inimigo, GameWorld *gw, float delta);
void destruirInimigoPlantaGelo(InimigoPlantaGelo *inimigo);
void desenharInimigoPlantaGelo(InimigoPlantaGelo *inimigo);
QuadroAnimacao *getQuadroAnimacaoAtualInimigoPlantaGelo(InimigoPlantaGelo *inimigo);