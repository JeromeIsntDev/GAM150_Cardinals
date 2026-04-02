/**
 * @file Level5.hpp
 * @brief Header for level functions
 * @details -
 * @author Orbital Candy
 * @date 01.04.2026
 */

#pragma once
#ifndef _Level5_Header_
#define _Level5_Header_

#include <iostream>

#include <AEEngine.h>

#include "TextureManager.hpp"
#include "MemoryManager.hpp"
#include "CameraEffects.hpp"

 /**
 * @brief Loads the level
 */
void Level5_Load();
/**
* @brief Initialize the values for the level
*/
void Level5_Initialize();
/**
* @brief Update function for the level
*/
void Level5_Update();
/**
* @brief Draws the level objects
*/
void Level5_Draw();
/**
* @brief Frees the level object
*/
void Level5_Free();
/**
* @brief Unloads the level
*/
void Level5_Unload();

#endif // !_Level5_Header_