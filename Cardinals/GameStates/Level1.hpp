#pragma once
#ifndef _Level1_Header_
#define _Level1_Header_

#define NOMINMAX

#include <AEEngine.h>
#include <iostream>

#include "MeshGenerator.hpp"
#include "TextureManager.hpp"
#include "AudioManager.hpp"
#include "PlayerController.hpp"
#include "ControlPanel.hpp"
#include "InputHandler.hpp"
#include "GameSaver.hpp"
#include "GStateManager.hpp"

void Level1_Load();
void Level1_Initialize();
void Level1_Update();
void Level1_Draw();
void Level1_Free();
void Level1_Unload();

#endif // !_Level1_Header_
