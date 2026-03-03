/*!***********************************************************************
\file	SpritesheetHandler.hpp
\author	Chua Jia Xuan Breanne
\date	25/02/2026
\par	Last Updated by: Breanne
\par	Project: Cardinals

\brief
Handles all the spritesheet reading and tilemap textures.
**************************************************************************/

#pragma once
#ifndef _UI_Manager_Header_
#define _UI_Manager_Header_

#include <AEEngine.h>
#include "TextureManager.hpp"

//struct Dimensions 
//{
//    int sheetWidth, sheetHeight, tileWidth, tileHeight;
//    int rows, cols;
//    float uvW, uvH;
//};

class SpriteSheet 
{
protected:
    int sheetW, sheetH;
    int tileW, tileH;
    int rows, cols;

    float uvW, uvH;

public:
    Texture tex;

    void Load(const std::string& path, float w, float h);

};

#endif