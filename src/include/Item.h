#pragma once

#include "Tipos.h"

Item *criarItem(float x, float y, float largura, float altura, Color cor);
void atualizarItem(Item *item, GameWorld *gw, float delta);
void destruirItem(Item *item);
void desenharItem(Item *item);