/**
 * @file Level2.hpp
 * @brief Header for level functions
 * @details -
 * @author Orbital Candy
 * @date 01.04.2026
 */
#pragma once
#ifndef _Level2_Header_
#define _Level2_Header_

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
void Level2_Load();
/**
* @brief Initialize the values for the level
*/
void Level2_Initialize();
/**
* @brief Update function for the level
*/
void Level2_Update();
/**
* @brief Draws the level objects
*/
void Level2_Draw();
/**
* @brief Frees the level object
*/
void Level2_Free();
/**
* @brief Unloads the level
*/
void Level2_Unload();

#endif // !_Level1_Header_
