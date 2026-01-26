#pragma once
#ifndef _Game_Saver_
#define _Game_Saver_

#include <fstream>
#include <iostream>
#include <vector>

#include "../header/GameObject.hpp"

void JsonRead(const char* filePath);
void JsonPrint(std::vector<GameObject> gameObjects);

#endif // !_Game_Saver_
