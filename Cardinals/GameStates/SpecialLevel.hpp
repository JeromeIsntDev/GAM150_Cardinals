/**
 * @file SpecialLevel.hpp
 * @brief Header for level functions
 * @details -
 * @author Orbital Candy
 * @date 01.04.2026
 */

#pragma once
#ifndef _LevelS_Header_
#define _LevelS_Header_

#include <iostream>

#include <AEEngine.h>

#include "TextureManager.hpp"
#include "MemoryManager.hpp"

 /**
 * @brief Loads the level
 */
void LevelS_Load();
/**
* @brief Initialize the values for the level
*/
void LevelS_Initialize();
/**
* @brief Update function for the level
*/
void LevelS_Update();
/**
* @brief Draws the level objects
*/
void LevelS_Draw();
/**
* @brief Frees the level object
*/
void LevelS_Free();
/**
* @brief Unloads the level
*/
void LevelS_Unload();

#endif // !_LevelS_Header_