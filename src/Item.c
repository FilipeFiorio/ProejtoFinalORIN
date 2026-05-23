#include <stdlib.h>

#include "raylib/raylib.h"
#include "Tipos.h"
#include "Item.h"

Item *criarItem(float x, float y, float largura, float altura, Color cor) {

    Item *novoItem = (Item*) malloc(sizeof(Item));

    novoItem->ret.x = x;
    novoItem->ret.y = y;
    novoItem->ret.width = largura;
    novoItem->ret.height = altura;

    novoItem->cor = cor;

    novoItem->ativo = true;

    return novoItem;

}

void atualizarItem(Item *item, GameWorld *gw, float delta) {

    if(item->ativo) {
        // Para implementar animação
    }

}

void destruirItem(Item *item) {
    
    // Não precisa por agora

}

void desenharItem(Item *item) {

    if(item->ativo) {
        DrawRectangleRec(item->ret, item->cor);
    }

}