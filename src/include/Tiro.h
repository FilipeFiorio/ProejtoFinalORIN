#pragma once

#include "Tipos.h"

Tiro *criarTiro(TipoTiro tipo);
void atualizarTiro(Tiro *tiro, GameWorld *gw, float delta);
void destruirTiro(Tiro *tiro);
void desenharTiro(Tiro *tiro);