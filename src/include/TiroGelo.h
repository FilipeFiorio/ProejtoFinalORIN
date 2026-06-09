#pragma once

#include "Tipos.h"

TiroGelo *criarTiroGelo(InimigoPlanta *inimigo);
void atualizarTiroGelo(TiroGelo *tiro, GameWorld *gw, float delta);
void destruirTiroGelo(TiroGelo *tiro);
void desenharTiroGelo(TiroGelo *tiro);