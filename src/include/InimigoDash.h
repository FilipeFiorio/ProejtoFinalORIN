#pragma once

#include "Tipos.h"

InimigoDash *criarInimigoDash(float x, float y, float largura, float altura, float tamanhoDash, Color cor);
void atualizarInimigoDash(InimigoDash *inimigo, GameWorld *gw, float delta);
void destruirInimigoDash(InimigoDash *inimigo);
void desenharInimigoDash(InimigoDash *inimigo);
QuadroAnimacao *getQuadroAnimacaoAtualInimigoDash(InimigoDash *inimigo);