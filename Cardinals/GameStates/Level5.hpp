#pragma once
#ifndef _Level5_Header_
#define _Level5_Header_

#include <iostream>

#include <AEEngine.h>

#include "TextureManager.hpp"
#include "MemoryManager.hpp"
#include "CameraEffects.hpp"

void Level5_Load();
void Level5_Initialize();
void Level5_Update();
void Level5_Draw();
void Level5_Free();
void Level5_Unload();

#endif // !_Level5_Header_