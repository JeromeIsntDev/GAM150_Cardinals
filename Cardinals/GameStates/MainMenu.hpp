#pragma once
#ifndef _Main_Menu_Header_
#define _Main_Menu_Header_

#include <iostream>
#include <vector>
#include <AEEngine.h>

#include "GStateManager.hpp"
#include "TextureManager.hpp"
#include "MeshGenerator.hpp"
#include "PlayerController.hpp"
#include "UIManager.hpp"
#include "AudioManager.hpp"
#include "GameObject.hpp"
#include "GameSaver.hpp"
#include "GameOverlayUI.hpp"
#include "MemoryManager.hpp"

void MainMenu_Load();
void MainMenu_Initialize();
void MainMenu_Update();
void MainMenu_Draw();
void MainMenu_Free();
void MainMenu_Unload();

#endif // !_Main_Menu_Header_
