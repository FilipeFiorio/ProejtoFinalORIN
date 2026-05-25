#include <stdlib.h>

#include "raylib/raylib.h"

#include "Tipos.h"
#include "InimigoVoador.h"

static void resolverColisaoInimigoMapaX(InimigoVoador *i, Mapa *m);
static void resolverColisaoInimigoMapaY(InimigoVoador *i, Mapa *m);

InimigoVoador *criarInimigoVoador(float x, float y, float largura, float altura, Vector2 deslocamento, Vector2 vel, Color cor) {

    InimigoVoador *novoInimigoVoador = (InimigoVoador*) malloc(sizeof(InimigoVoador));

    novoInimigoVoador->ret.x = x;
    novoInimigoVoador->ret.y = y;
    novoInimigoVoador->ret.width = largura;
    novoInimigoVoador->ret.height = altura;
    novoInimigoVoador->deslocamento = deslocamento;
    novoInimigoVoador->cor = cor;
    novoInimigoVoador->vel = vel;
    novoInimigoVoador->posInicial = (Vector2) {x, y};
    novoInimigoVoador->estaVivo = true;

    return novoInimigoVoador;

}

void atualizarInimigoVoador(InimigoVoador *inimigo, GameWorld *gw, float delta) {

    if(inimigo->estaVivo) {

        Vector2 alvo;
        
        if(!inimigo->retornando) {
            alvo = (Vector2) {
                inimigo->posInicial.x + inimigo->deslocamento.x,
                inimigo->posInicial.y + inimigo->deslocamento.y
            };
        } else {
            alvo = inimigo->posInicial;
        }
        
        if(inimigo->deslocamento.x != 0) {
            if(!inimigo->retornando) {
                inimigo->ret.x += inimigo->vel.x * delta;
                
                if(inimigo->ret.x >= alvo.x) { 
                    inimigo->ret.x = alvo.x; 
                    inimigo->retornando = true; 
                }
            } else {
                inimigo->ret.x -= inimigo->vel.x * delta;

                if(inimigo->ret.x <= alvo.x) { 
                    inimigo->ret.x = alvo.x; 
                    inimigo->retornando = false; 
                }
            }
        }

        if(inimigo->deslocamento.y != 0) {
            if(!inimigo->retornando) {
                inimigo->ret.y += inimigo->vel.y * delta;

                if(inimigo->ret.y >= alvo.y) { 
                    inimigo->ret.y = alvo.y; 
                    inimigo->retornando = true; 
                }
            } else {
                inimigo->ret.y -= inimigo->vel.y * delta;
                
                if(inimigo->ret.y <= alvo.y) { 
                    inimigo->ret.y = alvo.y; 
                    inimigo->retornando = false; 
                }
            }
        }
        
        resolverColisaoInimigoMapaX(inimigo, gw->mapa);
        resolverColisaoInimigoMapaY(inimigo, gw->mapa);

    }
    
}

void destruirInimigoVoador(InimigoVoador *inimigo) {

    // Fazer dps

}

void desenharInimigoVoador(InimigoVoador *inimigo) {
    
    if(inimigo->estaVivo) {
        DrawRectangleRec(inimigo->ret, inimigo->cor);
    }

}

static void resolverColisaoInimigoMapaY( InimigoVoador *i, Mapa *m ) {

    ElementoMapa *el = m->obstaculos;

    while ( el != NULL ) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if ( CheckCollisionRecs( i->ret, o->ret ) ) {
                if ( i->ret.y + i->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                    i->ret.y = o->ret.y - i->ret.height;
                } else { //não precisa
                    i->ret.y = o->ret.y + o->ret.height;
                }
                i->vel.y = 0;
            }

        } else if(obs->tipo == OBSTACULO_MOVEL)  {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;
            
            if ( CheckCollisionRecs( i->ret, o->ret ) ) {
                if ( i->ret.y + i->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                    i->ret.y = o->ret.y - i->ret.height;
                } else { //não precisa
                    i->ret.y = o->ret.y + o->ret.height;
                }
                i->vel.y = 0;
            }
         }

        el = el->proximo;

    }

}

static void resolverColisaoInimigoMapaX(InimigoVoador *i, Mapa *m) {

    ElementoMapa *el = m->obstaculos;

    while(el != NULL) {

        Obstaculo *obs = (Obstaculo*) el->objeto;

        if(obs->tipo == OBSTACULO_NORMAL) {

            ObstaculoNormal *o = (ObstaculoNormal*) obs->objeto;

            if(CheckCollisionRecs(i->ret, o->ret)) {
                if(i->ret.x + i->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    i->ret.x = o->ret.x - i->ret.width;
                } else {
                    i->ret.x = o->ret.x + o->ret.width;
                }
                    
                i->vel.x = -i->vel.x;
                
            }

        } else if(obs->tipo == OBSTACULO_MOVEL) {

            ObstaculoMovel *o = (ObstaculoMovel*) obs->objeto;

            if(CheckCollisionRecs(i->ret, o->ret)) {
                if(i->ret.x + i->ret.width / 2 < o->ret.x + o->ret.width / 2) {
                    i->ret.x = o->ret.x - i->ret.width;
                } else {
                    i->ret.x = o->ret.x + o->ret.width;
                }
                    
                i->vel.x = -i->vel.x;
                
            }
            
        }

        el = el->proximo;

    }


}
