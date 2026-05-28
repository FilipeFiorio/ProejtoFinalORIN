#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Obstaculo.h"
#include "ObstaculoNormal.h"
#include "ObstaculoMovel.h"
#include "ObstaculoChegada.h"

void desenharObstaculo(Obstaculo *obstaculo) {

    if(obstaculo == NULL) {
        return;
    }
    
    if(obstaculo->tipo == OBSTACULO_NORMAL) {
        desenharObstaculoNormal((ObstaculoNormal*) obstaculo->objeto);
    } else if(obstaculo->tipo == OBSTACULO_MOVEL) {
        desenharObstaculoMovel((ObstaculoMovel*) obstaculo->objeto);
    } else if(obstaculo->tipo == OBSTACULO_CHEGADA) {
        desenharObstaculoChegada((ObstaculoChegada*) obstaculo->objeto);
    }
}

Obstaculo *criarObstaculo(TipoObstaculo tipo) {

    Obstaculo *novoObstaculo = (Obstaculo*) malloc(sizeof(Obstaculo));

    novoObstaculo->objeto = NULL;
    novoObstaculo->tipo = tipo;

    return novoObstaculo;
}

void destruirObstaculo(Obstaculo *obstaculo) {

    if(obstaculo == NULL) {
        return;
    }

    if(obstaculo->tipo == OBSTACULO_NORMAL) {
        destruirObstaculoNormal((ObstaculoNormal*) obstaculo->objeto);
    } else if(obstaculo->tipo == OBSTACULO_MOVEL) {
        destruirObstaculoMovel((ObstaculoMovel*) obstaculo->objeto);
    } else if(obstaculo->tipo == OBSTACULO_CHEGADA) {
        destruirObstaculoChegada((ObstaculoChegada*) obstaculo->objeto);
    }
    
}