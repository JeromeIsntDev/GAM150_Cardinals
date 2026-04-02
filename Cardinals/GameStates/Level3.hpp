/**
 * @file Level3.hpp
 * @brief Header for level functions
 * @details -
 * @author Orbital Candy
 * @date 01.04.2026
 */
#pragma once
#ifndef _Level3_Header_
#define _Level3_Header_

#include <iostream>

#include <AEEngine.h>

#include "TextureManager.hpp"
#include "MemoryManager.hpp"
#include "CameraEffects.hpp"

 /**
 * @brief Loads the level
 */
void Level3_Load();
/**
* @brief Initialize the values for the level
*/
void Level3_Initialize();
/**
* @brief Update function for the level
*/
void Level3_Update();
/**
* @brief Draws the level objects
*/
void Level3_Draw();
/**
* @brief Frees the level object
*/
void Level3_Free();
/**
* @brief Unloads the level
*/
void Level3_Unload();

#endif // !_Level1_Header_