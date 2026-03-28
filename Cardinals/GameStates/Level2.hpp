#pragma once
#ifndef _Level2_Header_
#define _Level2_Header_

#define NOMINMAX

#include <iostream>

#include <AEEngine.h>
#include <iostream>

#include "MeshGenerator.hpp"
#include "TextureManager.hpp"
#include "AudioManager.hpp"
#include "PlayerController.hpp"
#include "ControlPanel.hpp"
#include "InputHandler.hpp"
#include "GameSaver.hpp"
#include "MemoryManager.hpp"
#include "GStateManager.hpp"
#include "ParticleEffect.hpp"
#include "GameOverlayUI.hpp"
#include "CameraEffects.hpp"

void Level2_Load();
void Level2_Initialize();
void Level2_Update();
void Level2_Draw();
void Level2_Free();
void Level2_Unload();

#endif // !_Level1_Header_
