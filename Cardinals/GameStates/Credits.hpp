#pragma once
#ifndef _Credits_Header_
#define _Credits_Header_

#include <iostream>

#include <AEEngine.h>

#include "../header/TextureManager.hpp"
#include "../header/GameObject.hpp"
#include "../header/MemoryManager.hpp"
#include "../header/GStateManager.hpp"

void Credits_Load();
void Credits_Initialize();
void Credits_Update();
void Credits_Draw();
void Credits_Free();
void Credits_Unload();

#endif // !_Credits_Header_
