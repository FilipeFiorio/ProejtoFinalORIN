#pragma once

#include "Tipos.h"

ObstaculoMovel *criarObstaculoMovel(Rectangle ret, Rectangle fonte, Vector2 deslocamento, Vector2 vel, Color cor, Texture2D *textura);
void destruirObstaculoMovel(ObstaculoMovel *obstaculoMovel);
void desenharObstaculoMovel(ObstaculoMovel *obstaculoMovel);
void atualizarObstaculoMovel(ObstaculoMovel *obstaculoMovel, GameWorld *gw, float delta);