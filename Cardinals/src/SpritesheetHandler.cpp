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

}

void SpriteSheet::Slice(Texture t)
{
    tex = t;
    slices.clear(); // reset

    // rows and column slicing bro
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            // 1 * uvW, etc. it just slices based on the index kinda
            // pushes into slices[]
            slices.push_back({ c * uvW, r * uvH, uvW, uvH });
        }
    }
}

void SpriteSheet::RegisterAllTiles()
{
    // maps every slice index to itself (0 = 0, 1=1 whatever
    for (int i = 0; i < (int)slices.size(); i++)
    {
        idToSlice[i] = i;
    }
}

//void SpriteSheet::RegisterTile(int tileID, int tileIndex)
//{
//    // js registers. diff stuff to diff tile or whatever u want
//    if (tileIndex < (int)slices.size())
//    {
//        idToSlice[tileID] = tileIndex;
//    }
//}

bool SpriteSheet::GetUV(int tileID, TileUV& outUV) const
{
    // searches map for ID key. if tileID = whatever, it looks for whatever
    auto it = idToSlice.find(tileID);
    if (it == idToSlice.end())
    {
        PRINT("[DEBUG] => Tile not found!\n");
        return false;
    }

    // grabs the tileUV struct and writes into outUV i love keyvalues vro
    // second literally is just the second val its the value stored at that key which is slices[] index
    outUV = slices[it->second];
    return true;
}