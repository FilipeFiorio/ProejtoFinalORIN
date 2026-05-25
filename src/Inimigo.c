#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Inimigo.h"
#include "InimigoNormal.h"
#include "InimigoDash.h"
#include "InimigoVoador.h"

Inimigo *criarInimigo(TipoInimigo tipo) {

    Inimigo *novoInimigo = (Inimigo*) malloc(sizeof(Inimigo));

    novoInimigo->objeto = NULL;
    novoInimigo->tipo = tipo;

    return novoInimigo;
   
}

void atualizarInimigo(Inimigo *inimigo, GameWorld *gw, float delta) {

    if(inimigo == NULL) {
        return;
    }

    if(inimigo->tipo == INIMIGO_NORMAL) {
        atualizarInimigoNormal((InimigoNormal*) inimigo->objeto, gw, delta);
    } else if(inimigo->tipo == INIMIGO_DASH) {
        atualizarInimigoDash((InimigoDash*) inimigo->objeto, gw, delta);
    } else if(inimigo->tipo == INIMIGO_VOADOR) {
        atualizarInimigoVoador((InimigoVoador*) inimigo->objeto, gw, delta);
    }

}

void destruirInimigo(Inimigo *inimigo) {

    // Fazer dps
    
}

void desenharInimigo(Inimigo *inimigo) {

    if(inimigo == NULL) {
        return;
    }

    if(inimigo->tipo == INIMIGO_NORMAL) {
        desenharInimigoNormal((InimigoNormal*) inimigo->objeto);
    } else if(inimigo->tipo == INIMIGO_DASH) {
        desenharInimigoDash((InimigoDash*) inimigo->objeto);
    } else if(inimigo->tipo == INIMIGO_VOADOR) {
        desenharInimigoVoador((InimigoVoador*) inimigo->objeto);
    }

}