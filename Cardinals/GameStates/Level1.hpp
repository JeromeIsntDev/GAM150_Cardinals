#pragma once
#ifndef _Level1_Header_
#define _Level1_Header_

#include <iostream>

#include <AEEngine.h>

#include "../header/MeshGenerator.hpp"
#include "../header/TextureManager.hpp"
#include "../header/AudioManager.hpp"
#include "../header/PlayerController.hpp"

void Level1_Load();
void Level1_Initialize();
void Level1_Update();
void Level1_Draw();
void Level1_Free();
void Level1_Unload();

#endif // !_Level1_Header_
