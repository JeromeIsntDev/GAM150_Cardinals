#include "SpritesheetHandler.hpp"
#include <unordered_map>
#include <vector>

void SpriteSheet::Load(const std::string& path, float w, float h)
{
    go_texture.Load_Image(path, w, h);

    sheetW = static_cast<int>(go_texture.Get_Image_Width());
    sheetH = static_cast<int>(go_texture.Get_Image_Height());

}

void SpriteSheet::Slice(const Texture& t)
{
    go_texture = t;
    slices.clear();

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            f32 xPos = (f32)(c * tileW) / sheetW;
            f32 yPos = (f32)(r * tileH) / sheetH;

            slices.push_back({ xPos, yPos, uvW, uvH });
        }
    }
}

void SpriteSheet::RegisterAllTiles()
{
    for (int i = 0; i < (int)slices.size(); i++)
    {
        idToSlice[i] = i;
    }
}

bool SpriteSheet::GetUV(int tileID, TileUV& outUV) const
{
    auto it = idToSlice.find(tileID);
    if (it == idToSlice.end())
    {
        PRINT("[DEBUG] => Tile not found!\n");
        return false;
    }

    outUV = slices[it->second];
    return true;
}