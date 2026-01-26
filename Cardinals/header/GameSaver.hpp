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

void ReadJsonToObjects(const char* filePath);
void SaveObjectsToJson(std::vector<GameObject> gameObjects);

#endif // !_Game_Saver_
