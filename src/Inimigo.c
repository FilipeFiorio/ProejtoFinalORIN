#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Inimigo.h"
#include "InimigoNormal.h"
#include "InimigoDash.h"
#include "InimigoVoador.h"
#include "InimigoEspinho.h"
#include "InimigoFantasma.h"
#include "InimigoPedra.h"
#include "InimigoPlanta.h"
#include "InimigoPlantaGelo.h"

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
    } else if(inimigo->tipo == INIMIGO_ESPINHO) {
        atualizarInimigoEspinho((InimigoEspinho*) inimigo->objeto, gw, delta);
    } else if(inimigo->tipo == INIMIGO_FANTASMA) {
        atualizarInimigoFantasma((InimigoFantasma*) inimigo->objeto, gw, delta);
    }

}

void destruirInimigo(Inimigo *inimigo) {

    if(inimigo == NULL) {
        return;
    }

    if(inimigo->tipo == INIMIGO_NORMAL) {
        destruirInimigoNormal((InimigoNormal*) inimigo->objeto);
    } else if(inimigo->tipo == INIMIGO_DASH) {
        destruirInimigoDash((InimigoDash*) inimigo->objeto);
    } else if(inimigo->tipo == INIMIGO_VOADOR) {
        destruirInimigoVoador((InimigoVoador*) inimigo->objeto);
    } else if(inimigo->tipo == INIMIGO_ESPINHO) {
        destruirInimigoEspinho((InimigoEspinho*) inimigo->objeto);
    } else if(inimigo->tipo == INIMIGO_FANTASMA) {
        destruirInimigoFantasma((InimigoFantasma*) inimigo->objeto);
    }
    
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
    } else if(inimigo->tipo == INIMIGO_ESPINHO) {
        desenharInimigoEspinho((InimigoEspinho*) inimigo->objeto);
    } else if(inimigo->tipo == INIMIGO_FANTASMA) {
        desenharInimigoFantasma((InimigoFantasma*) inimigo->objeto);
    }

}