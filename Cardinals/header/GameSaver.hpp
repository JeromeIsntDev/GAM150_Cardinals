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

std::vector<GameObject> ReadJsonToObjects(const char* filePath);
void SaveObjectsToJson(std::vector<GameObject>, std::string);

#endif // !_Game_Saver_
