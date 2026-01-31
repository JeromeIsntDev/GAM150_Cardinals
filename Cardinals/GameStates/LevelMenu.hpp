#pragma once
#ifndef _Level_Menu_Header_
#define _Level_Menu_Header_

#include <iostream>
#include <memory>
#include <AEEngine.h>

#include "TextureManager.hpp"
#include "MeshGenerator.hpp"
#include "PlayerController.hpp"
#include "UIManager.hpp"

void LevelMenu_Load();
void LevelMenu_Initialize();
void LevelMenu_Update();
void LevelMenu_Draw();
void LevelMenu_Free();
void LevelMenu_Unload();

#endif // !_Main_Menu_Header_
