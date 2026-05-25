#pragma once

#include "Tipos.h"

Inimigo *criarInimigo(TipoInimigo tipo);
void atualizarInimigo(Inimigo *inimigo, GameWorld *gw, float delta);
void destruirInimigo(Inimigo *inimigo);
void desenharInimigo(Inimigo *inimigo);