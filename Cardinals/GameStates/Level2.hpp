#pragma once
#ifndef _Level2_Header_
#define _Level2_Header_

#include <iostream>

#include <AEEngine.h>

#include "TextureManager.hpp"
#include "GameObject.hpp"
#include "MeshGenerator.hpp"
#include "InputHandler.hpp"
#include "UIManager.hpp"
#include "GStateManager.hpp"

void Level2_Load();
void Level2_Initialize();
void Level2_Update();
void Level2_Draw();
void Level2_Free();
void Level2_Unload();

#endif // !_Level1_Header_
