#pragma once

#include "Tipos.h"

void desenharObstaculo(Obstaculo *obstaculo);
void desenharObstaculos(Obstaculo *obstaculos, int quantidade);
Obstaculo *criarObstaculo(Rectangle ret, Rectangle fonte, Texture2D *textura, Color cor);