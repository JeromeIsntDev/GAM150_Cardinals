/**
 * @file Level1.hpp
 * @brief Header for level functions
 * @details -
 * @author Orbital Candy
 * @date 01.04.2026
 */
#pragma once
#ifndef _Level1_Header_
#define _Level1_Header_

#define NOMINMAX

#include <AEEngine.h>
#include <iostream>

#include "MeshGenerator.hpp"
#include "TextureManager.hpp"
#include "AudioManager.hpp"
#include "PlayerController.hpp"
#include "InputHandler.hpp"
#include "GameSaver.hpp"
#include "MemoryManager.hpp"
#include "GStateManager.hpp"
#include "ParticleEffect.hpp"
#include "GameOverlayUI.hpp"
#include "CameraEffects.hpp"
/**
* @brief Loads the level
*/
void Level1_Load();
/**
* @brief Initialize the values for the level
*/
void Level1_Initialize();
/**
* @brief Update function for the level
*/
void Level1_Update();
/**
* @brief Draws the level objects
*/
void Level1_Draw();
/**
* @brief Frees the level object
*/
void Level1_Free();
/**
* @brief Unloads the level
*/
void Level1_Unload();

#endif // !_Level1_Header_
