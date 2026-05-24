#pragma once

#include "Tipos.h"

ObstaculoNormal *criarObstaculoNormal(Rectangle ret, Rectangle fonte, Color cor, Texture2D *textura);
void destruirObstaculoNormal(ObstaculoNormal *obstaculoNormal);
void desenharObstaculoNormal(ObstaculoNormal *obstaculoNormal);