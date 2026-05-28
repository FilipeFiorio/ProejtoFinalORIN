#pragma once

#include "Tipos.h"

ObstaculoChegada *criarObstaculoChegada(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura);
void destruirObstaculoChegada(ObstaculoChegada *obstaculoChegada);
void desenharObstaculoChegada(ObstaculoChegada *obstaculoChegada);