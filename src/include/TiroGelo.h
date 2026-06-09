#pragma once

#include "Tipos.h"

TiroGelo *criarTiroGelo(InimigoPlantaGelo *inimigo);
void atualizarTiroGelo(TiroGelo *tiro, GameWorld *gw, float delta);
void destruirTiroGelo(TiroGelo *tiro);
void desenharTiroGelo(TiroGelo *tiro);
QuadroAnimacao *getQuadroAnimacaoAtualTiroGelo(TiroGelo *tiro);