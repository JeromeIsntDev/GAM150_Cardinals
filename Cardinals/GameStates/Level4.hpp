/**
 * @file Level4.hpp
 * @brief Header for level functions
 * @details -
 * @author Orbital Candy
 * @date 01.04.2026
 */
#pragma once
#ifndef _Level4_Header_
#define _Level4_Header_

#include <iostream>

#include <AEEngine.h>

#include "TextureManager.hpp"
#include "MemoryManager.hpp"
#include "CameraEffects.hpp"


 /**
 * @brief Loads the level
 */
void Level4_Load();
/**
* @brief Initialize the values for the level
*/
void Level4_Initialize();
/**
* @brief Update function for the level
*/
void Level4_Update();
/**
* @brief Draws the level objects
*/
void Level4_Draw();
/**
* @brief Frees the level object
*/
void Level4_Free();
/**
* @brief Unloads the level
*/
void Level4_Unload();

#endif // !_Level1_Header_
