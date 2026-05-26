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

}

void unloadResourcesResourceManager( void ) {

    UnloadTexture( rm.texturaTerreno );
    UnloadTexture( rm.texturaFundo );

}