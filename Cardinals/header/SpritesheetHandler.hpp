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
#ifndef _SpriteSheet_Manager_Header_
#define _SpriteSheet_Manager_Header_

#include <AEEngine.h>
#include "TextureManager.hpp"
#include <unordered_map>
#include <vector>

struct TileUV 
{ 
    f32 u, v, uWidth, vHeight;
};

class SpriteSheet 
{
public:
    int sheetW{}, sheetH{}, tileW{}, tileH{};
    int rows{}, cols{};
    float uvW{}, uvH{};
    Texture go_texture{};

    std::vector<TileUV> slices; // all the sliced up uv rects
    std::unordered_map<int, int> idToSlice; // tileID > slices[] index

public:

    void Load(const std::string& path, float w, float h);
    void Slice(Texture go_texture);

    void RegisterAllTiles(); // flat index > ID
    // uhh in case we gotta map diff tags to diff tiles
    //void RegisterSpecificTile(int tileID, int tileIndex);

    bool GetUV(int tileID, TileUV& outUV) const; // no way in hell u wanna get it changed i think
    Texture& GetTexture() { return go_texture; }
};

#endif