/**
 * @file ResourceManager.h
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager struct and function declarations.
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"

typedef struct ResourceManager {
    Texture2D texturaTerreno;
    Texture2D texturaFundo;
    Texture2D texturaInicio;
    Texture2D texturaInimigoNormal;
    Texture2D texturaInimigoDash;
    Texture2D texturaInimigoVoador;
    Texture2D texturaInimigoEspinho;
    Texture2D texturaInimigoPedra;
    Texture2D texturaInimigoPlanta; 
    Texture2D texturaInimigoPlantaGelo; 
    Texture2D texturaInimigoFantasma;
    Texture2D texturaItens;
    Texture2D texturaInimigoMorrendo;
    Texture2D texturaJogador;

    Font fonte;
} ResourceManager;

/**
 * @brief Global ResourceManager instance.
 */
extern ResourceManager rm;

/**
 * @brief Load global game resources, linking them in the global instance of
 * ResourceManager called rm.
 */
void loadResourcesResourceManager( void );

/**
 * @brief Unload global game resources.
 */
void unloadResourcesResourceManager( void );