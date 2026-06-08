/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "ResourceManager.h"
#include "raylib/raylib.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {

    rm.texturaTerreno = LoadTexture( "resources/images/tiles/terreno.png" );
    rm.texturaFundo = LoadTexture("resources/images/fundos/bg.png");
    rm.texturaInicio = LoadTexture("resources/images/fundos/inicio.png");
    rm.texturaInimigoNormal = LoadTexture("resources/images/sprites/inimigoNormal.png");
    rm.texturaInimigoDash = LoadTexture("resources/images/sprites/inimigoDash.png");
    rm.texturaInimigoVoador = LoadTexture("resources/images/sprites/inimigoVoador.png");
    rm.texturaInimigoEspinho = LoadTexture("resources/images/sprites/InimigoEspinho.png");
    rm.texturaInimigoFantasma = LoadTexture("resources/images/sprites/InimigoFantasma.png");
    rm.texturaInimigoPedra = LoadTexture("resources/images/sprites/InimigoPedra.png");
    rm.texturaInimigoPlanta = LoadTexture("resources/images/sprites/InimigoPlanta.png");
    rm.texturaInimigoPlantaGelo = LoadTexture("resources/images/sprites/InimigoPlantaGelo.png");
    rm.texturaItens = LoadTexture("resources/images/sprites/itens.png");
    rm.texturaInimigoMorrendo = LoadTexture("resources/images/sprites/morteInimigo.png");
    rm.texturaJogador = LoadTexture("resources/images/sprites/jogador.png");

    rm.fonte = LoadFontEx("resources/fontes/PressStart2P-Regular.ttf", 32, 0, 250);

}

void unloadResourcesResourceManager( void ) {

    UnloadTexture( rm.texturaTerreno );
    UnloadTexture( rm.texturaFundo );
    UnloadTexture(rm.texturaInicio);
    UnloadTexture(rm.texturaInimigoNormal);
    UnloadTexture(rm.texturaInimigoDash);
    UnloadTexture(rm.texturaInimigoVoador);
    UnloadTexture(rm.texturaItens);
    UnloadTexture(rm.texturaInimigoMorrendo);
    UnloadTexture(rm.texturaJogador);
    UnloadFont(rm.fonte);

}