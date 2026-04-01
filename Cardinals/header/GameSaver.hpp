/**
 * @file		GameSaver.hpp
 * @brief		Handles saving and loading game objects
 * @details		Saves mainly for loading and saving levels, does not save UI and interface values
 * @author		Jerome Lim
 * @date		19.01.2026
 */

#pragma once
#ifndef _Game_Saver_
#define _Game_Saver_

#include <fstream>
#include <iostream>
#include <vector>

#include <istreamwrapper.h>
#include <ostreamwrapper.h>
#include <writer.h>
#include <prettywriter.h>

#include "../header/GameObject.hpp"

/**
  * @brief		Returns a vector of GameObjects that have been read from a JSON FilePath; (Dev Note: Might be better to have used pointer)
*/
std::vector<GameObject> ReadJsonToObjects(const char* filePath);
/**
 * @brief		Saves a vector of GameObjects to a JSON FilePath 
 */
void SaveObjectsToJson(std::vector<GameObject*>, std::string);

#endif // !_Game_Saver_
