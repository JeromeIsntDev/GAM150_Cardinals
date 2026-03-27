#pragma once
#ifndef _LevelS_Header_
#define _LevelS_Header_

#include <iostream>

#include <AEEngine.h>

#include "TextureManager.hpp"
#include "MemoryManager.hpp"

void LevelS_Load();
void LevelS_Initialize();
void LevelS_Update();
void LevelS_Draw();
void LevelS_Free();
void LevelS_Unload();

#endif // !_LevelS_Header_