/**
 * @file		TextureManager.hpp
 * @brief		Header for managing textures, colors, and fonts.
 * @details		Handles all the texture reading, loading, colors, fonts and etc.
 * @author		Shirley Liau, Chua Jia Xuan Breanne
 * @date		20.01.2026
 */

#pragma once
#ifndef _Texture_Man_Header_
#define _Texture_Man_Header_

#include <AEEngine.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>

 /**
  * @brief Color struct for RGBA normalized values
  */
struct Color { f32 r, g, b, a; };

/**
 * @brief FontFile struct to store font pathing
 */
struct FontFile { const char* filename; };

/**
 * @brief ImageFile struct to store file path and expected dimensions
 */
struct ImageFile {
    const char* filename;
    f32 width, height;
};

///
// NAMESPACES (Colors, Images, Fonts) ...
///

namespace Colors
{
    constexpr Color White{ 1.f, 1.f, 1.f, 1.f };
    constexpr Color Black{ 0.f, 0.f, 0.f, 1.f };
    constexpr Color PastelPink{ 242.f / 255.f, 198.f / 255.f, 222.f / 255.f, 1.f };
    constexpr Color PastelOrange{ 247.f / 255.f, 217.f / 255.f, 196.f / 255.f, 1.f };
    constexpr Color PastelPurple{ 219.f / 255.f, 205.f / 255.f, 240.f / 255.f, 1.f };
    constexpr Color PastelBlue{ 198.f / 255.f, 222.f / 255.f, 241.f / 255.f, 1.f };
    constexpr Color PastelGreen{ 201.f / 255.f, 228.f / 255.f, 222.f / 255.f, 1.f };
    constexpr Color PastelYellow{ 250.f / 255.f, 237.f / 255.f, 203.f / 255.f, 1.f };
}

namespace Images
{
    constexpr ImageFile DigipenLogo{ "../Assets/Textures/DigiPen_WHITE.png", 1525.f, 445.f };
    constexpr ImageFile Player{ "../Assets/Textures/playerIdle.png", 500.f,500.f };
    constexpr ImageFile PlayerJump{ "../Assets/Textures/playerJump.png", 500.f,500.f };

	constexpr ImageFile PrevBtn{ "../Assets/Textures/Previous_Button.png", 120.f, 120.f };
    constexpr ImageFile ExitBtn{ "../Assets/Textures/Exit_Button.png", 412.f, 120.f };

	constexpr ImageFile Bar{ "../Assets/Textures/Generic_Bar.png", 500.f, 40.f };
	constexpr ImageFile KnobMaster{ "../Assets/Textures/Master_Vol.png", 80.f, 80.f };
	constexpr ImageFile KnobMusic{ "../Assets/Textures/Music_Vol.png", 80.f, 80.f };
	constexpr ImageFile KnobEFX{ "../Assets/Textures/EFX_Vol.png", 80.f, 80.f };
    constexpr ImageFile CardBg{ "../Assets/Textures/RoundedSq.png", 800.f, 600.f };
    constexpr ImageFile ResumeBtn{ "../Assets/Textures/Resume_Button.png", 412.f, 120.f };
	constexpr ImageFile HomeBtn{ "../Assets/Textures/Home_Button.png", 412.f, 120.f };
    constexpr ImageFile RestartBtn{ "../Assets/Textures/Restart_Button.png", 412.f, 120.f };
    constexpr ImageFile NextBtn{ "../Assets/Textures/Next_Button.png", 412.f, 120.f };
    constexpr ImageFile RetryBtn{ "../Assets/Textures/Retry_Button.png", 412.f, 120.f };

    constexpr ImageFile Background{ "../Assets/Textures/MenuBG.png", 1600.f, 900.f };
    constexpr ImageFile Placeholder{ "../Assets/Textures/MissingTexture.png", 1600.f, 900.f };
    constexpr ImageFile Button{ "../Assets/Textures/Button.png", 200.f, 200.f };
    constexpr ImageFile Lollipop{ "../Assets/Textures/Lollipop.png", 100.f, 100.f };
    constexpr ImageFile Candy{ "../Assets/Textures/Candy.png", 50.f, 50.f };
    constexpr ImageFile CandyGrey{ "../Assets/Textures/Candy_Grey.png", 50.f, 50.f };
    constexpr ImageFile CandyAni{ "../Assets/Textures/Candy_Animation.png", 50.f, 50.f };
    constexpr ImageFile CandyBall{ "../Assets/Textures/Candy_Ball.png", 50.f, 50.f };
    constexpr ImageFile CandyCorn{ "../Assets/Textures/Candy_Corn.png", 160.f, 160.f };
    constexpr ImageFile GravImg{ "../Assets/Textures/GravityBlock.png", 160.f, 160.f };
    constexpr ImageFile WinPortal{ "../Assets/Textures/WinPortal.png", 160.f, 160.f };
    constexpr ImageFile BG_BACKMOST{ "../Assets/Textures/BG_BACKMOST.png", 1600.f, 900.f };
    constexpr ImageFile BG_MOUNTAIN{ "../Assets/Textures/BG_BACKMOUNTAIN.png", 1600.f, 900.f };
    constexpr ImageFile BG_CLOUDS{ "../Assets/Textures/BG_CLOUDS.png", 1600.f, 900.f };
    constexpr ImageFile BG_FRONTMOUNTAIN{ "../Assets/Textures/BG_FRONTMOUNTAIN.png", 1600.f, 900.f };
    constexpr ImageFile Collectible{ "../Assets/Textures/Collectible.png", 160.f, 160.f };
}   

namespace Fonts
{
    constexpr FontFile Main{ "../Assets/Fonts/DynaPuff-Regular.ttf" };
    constexpr FontFile Alt{ "../Assets/Fonts/sourgummy-font.ttf" };
    constexpr FontFile Readable{ "../Assets/Fonts/alansans-font.ttf" };
}

///
/// TEXTURE
///

/**
 * @brief Texture class wrapper for AEGfxTexture
 */
class Texture
{
private:

    AEGfxTexture* _image;
    f32 _width;
    f32 _height;
public:

    /**
     * @brief Sets the global drawing color
     * @param color The Color struct to apply
     */
	void Fill_Color(const Color color);

    /**
     * @brief Loads an image from an ImageFile struct
     * @param imageFilePath Struct containing path and size
     */
	void Load_Image(const ImageFile& imageFilePath);

    /**
     * @brief Overloaded loader for manual path and dimension input from the manifest
     * @param path String path to file
     * @param w Width of image
     * @param h Height of image
     */
    void Load_Image(const std::string& path, float w, float h);

    /**
     * @brief Sets the texture to the active rendering slot
     * @param x The width
     * @param y The height
     * @return void
     */
    void Set_Image(f32 x, f32 y);

    /**
     * @brief Frees the texture resource
     * @return void
     */
    void Unload_Image();

    /**
     * @brief Retrieves the width of the loaded texture
     * @return f32 The width in pixels
     */
    f32 Get_Image_Width() const;
    
    /**
     * @brief Retrieves the height of the loaded texture
     * @return f32 The height in pixels
     */
    f32 Get_Image_Height() const;

    /**
     * @brief Returns the underlying Alpha Engine texture pointer
     * @return AEGfxTexture* Pointer to the texture object
     */
    AEGfxTexture* Get_Image() const;
};

///
/// TEXTUREMANAGER
///

/**
 * @brief Singleton manager for handling texture lifecycles
 */
class TextureManager
{
private:
    std::map<std::string, Texture> textureMap;
    std::unordered_set<std::string> persistentLoad;
    TextureManager() = default;

public:

    /**
     * @brief Access the Singleton instance
     * @return TextureManager&
     */
    static TextureManager& GetInstance() 
    {
        static TextureManager instance;
        return instance;
    }

    /**
     * @brief Parses a .txt manifest to bulk load textures
     * @param path Path to the manifest file
     */
    virtual void LoadManifest(const std::string& path);

    /**
     * @brief Retrieves a pointer to a loaded texture by name
     * @param name The key used in the manifest
     * @return Texture* Pointer to texture, or nullptr if not found
     */
    Texture* Get(const std::string& name);

    /**
     * @brief Unloads all non-persistent textures from the manager
     * @return void
     */
    void UnloadAll();

    /**
     * @brief Unloads all textures marked as persistent and clears the sets
     * @return void
     */
    void UnloadPersistent();

    /**
     * @brief Marks a specific texture ID as persistent so it survives UnloadAll()
     * @param name The unique string identifier of the texture
     * @return void
     */
    virtual void SetPersistent(const std::string& name);
};

///
/// TEXTMANAGER
///

/**
 * @brief Class for handling font loading and text rendering
 */
class Text {
private:
    s8 _font;
    f32 _fontSize;
    f32 _scale;
    Color _color;
    std::vector<std::string> _textArr;

public:
    Text();

    /**
     * @brief Loads a font file and initializes its base height
     * @param fontFilePath Struct containing the .ttf path
     * @param initHeight The base resolution/size of the font
     * @return void
    */
    void Load_Font(const FontFile& fontFilePath, const f32 initHeight);

    /**
     * @brief Parses a string into the internal text array, handling newlines
     * @param text The string to be stored for rendering
     * @return void
     */
    void Input_Text(const char* text);

    /**
     * @brief Converts a float to an integer string and stores it for rendering
     * @param t The floating point value to display
     * @return void
     */
    void Input_Float(float t);

    /**
     * @brief Renders the stored text array centered at the screen origin (0,0)
     * @return void
     */
    void Draw_Text() const;

    /**
     * @brief Renders the stored text array centered at a specific pixel offset
     * @param x Horizontal pixel offset
     * @param y Vertical pixel offset
     * @return void
     */
    void Draw_Text(f32 x, f32 y);

    /**
     * @brief Renders the text using x and y as the top-left corner coordinates
     * @param x Horizontal screen position
     * @param y Vertical screen position
     * @return void
     */
    void Draw_Text_Corner(f32 x, f32 y);

    /**
     * @brief Renders text at a position with custom scaling and transparency
     * @param pos Vector2 containing the screen position
     * @param scale The size multiplier for the font
     * @param fading The alpha transparency value (0.0 to 1.0)
     * @return void
     */
    void Draw_Text(AEVec2 pos, f32 scale, f32 fading);

    /**
     * @brief Updates the RGBA color used for subsequent text rendering
     * @param color The Color struct containing normalized RGBA values
     * @return void
     */
    void Change_Text_Color(const Color color);

    /**
     * @brief Destroys the font resource and frees
     * @return void
     */
    void Free_Font() const;
};

#endif // !_Texture_Man_Header_