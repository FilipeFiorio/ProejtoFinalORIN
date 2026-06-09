#pragma once

#include "Tipos.h"

TiroNormal *criarTiroNormal(InimigoPlanta *inimigo);
void atualizarTiroNormal(TiroNormal *tiro, GameWorld *gw, float delta);
void destruirTiroNormal(TiroNormal *tiro);
void desenharTiroNormal(TiroNormal *tiro);
QuadroAnimacao *getQuadroAnimacaoAtualTiroNormal(TiroNormal *tiro);