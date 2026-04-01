/**
 * @file		SpritesheetHandler.hpp
 * @brief		The Spritesheet class that handles the loading and slicing of tiles for the map
 * @details		Handles all the spritesheet reading and tilemap textures
 * @author		Chua Jia Xuan Breanne
 * @date		04.03.2026
 */

#pragma once
#ifndef _SpriteSheet_Manager_Header_
#define _SpriteSheet_Manager_Header_

#include <AEEngine.h>
#include "TextureManager.hpp"

/**
 * @brief TileUV struct to store dimensions
 */
struct TileUV 
{ 
    f32 u, v, uWidth, vHeight;
};

/**
 * @brief SpriteSheet class to manage texture slicing and UV mapping
 * This class handles loading textures, calculating UV coordinates for
 * individual tiles, and mapping tile IDs to specific UV slices.
 */
class SpriteSheet 
{
public:

    /**
     * @brief Dimensions and properties of the sheet and tiles
     */
    int sheetW{}, sheetH{}, tileW{}, tileH{};
    int rows{}, cols{};
    float uvW{}, uvH{};
    Texture go_texture{};

    /**
     * @brief Containers for UV data and ID mapping
     */
    std::vector<TileUV> slices;
    std::unordered_map<int, int> idToSlice;

public:

    /**
     * @brief Loads a texture and initializes sheet dimensions
     * * @param path File path to the texture
     * @param w Expected tile width
     * @param h Expected tile height
     * @return void
     */
    void Load(const std::string& path, float w, float h);

    /**
     * @brief Slices the texture into UV rectangles based on rows and columns
     * * @param t The texture to associate with this sheet
     * @return void
     */
    void Slice(const Texture& go_texture);

    /**
     * @brief Maps every available slice index to a default tile ID
     * @return void
     */
    void RegisterAllTiles();

    /**
     * @brief Finds and retrieves UV data for a specific tile ID
     * @param tileID The ID to search for in the map
     * @param outUV Reference to store the found UV coordinates
     * @return bool True if the ID was found, false otherwise
     */
    bool GetUV(int tileID, TileUV& outUV) const;

    /**
     * @brief Returns a reference to the underlying texture
     * @return Texture& Reference to the sheet's texture object
     */
    Texture& GetTexture() { return go_texture; }
};

#endif