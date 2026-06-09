#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tiro.h"
#include "TiroNormal.h"
#include "TiroGelo.h"

Tiro *criarTiro(TipoTiro tipo) {

    Tiro *novoTiro = (Tiro*) malloc(sizeof(Tiro));

    novoTiro->objeto = NULL;
    novoTiro->tipo = tipo;

    return novoTiro;

}

void atualizarTiro(Tiro *tiro, GameWorld *gw, float delta) {

    if(tiro->tipo == TIRO_NORMAL) {
        atualizarTiroNormal((TiroNormal*) tiro->objeto, gw, delta);
    } else if(tiro->tipo == TIRO_GELO) {
        atualizarTiroGelo((TiroGelo*) tiro->objeto, gw, delta);
    }

}

void destruirTiro(Tiro *tiro) {

    if(tiro->tipo == TIRO_NORMAL) {
        destruirTiroNormal((TiroNormal*) tiro->objeto);
    } else if(tiro->tipo == TIRO_GELO) {
        destruirTiroGelo((TiroGelo*) tiro->objeto);
    }

}

void desenharTiro(Tiro *tiro) {

    if(tiro->tipo == TIRO_NORMAL) {
        desenharTiroNormal((TiroNormal*) tiro->objeto);
    } else if(tiro->tipo == TIRO_GELO) {
        desenharTiroGelo((TiroGelo*) tiro->objeto);
    }

}