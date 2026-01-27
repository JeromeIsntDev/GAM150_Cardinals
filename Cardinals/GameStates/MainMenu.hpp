#pragma once
#ifndef _Main_Menu_Header_
#define _Main_Menu_Header_

#include <iostream>
#include <vector>
#include <AEEngine.h>

#include "../header/TextureManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/PlayerController.hpp"
#include "../header/UIManager.hpp"
#include "../header/AudioManager.hpp"
#include "../header/GameObject.hpp"
#include "../header/GameSaver.hpp"


void MainMenu_Load();
void MainMenu_Initialize();
void MainMenu_Update();
void MainMenu_Draw();
void MainMenu_Free();
void MainMenu_Unload();

#endif // !_Main_Menu_Header_
