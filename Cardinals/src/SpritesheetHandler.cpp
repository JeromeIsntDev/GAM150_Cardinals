/*!***********************************************************************
\file	SpritesheetHandler.cpp
\author	Chua Jia Xuan Breanne
\date	25/02/2026
\par	Last Updated by: Breanne
\par	Project: Cardinals

\brief
Handles all the spritesheet reading and tilemap textures.
**************************************************************************/

#include "SpritesheetHandler.hpp"

void SpriteSheet::Load(const std::string& path, float w, float h)
{
    tex.Load_Image(path, w, h);

    sheetW = tex.Get_Image_Width();
    sheetH = tex.Get_Image_Height();

    cols = sheetW / tileW;
    rows = sheetH / tileH;

    uvW = (float)tileW / sheetW;
    uvH = (float)tileH / sheetH;
}