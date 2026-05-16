#pragma once

#include "Tipos.h"

Mapa *carregarMapa(const char *caminhoArquivo);
void destruirMapa(Mapa *m);
void desenharMapa(Mapa *);
int calcularLarguraMapa(Mapa *m);
int calcularAlturaMapa(Mapa *m);