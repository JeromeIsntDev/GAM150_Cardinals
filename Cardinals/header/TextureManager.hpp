/*!***********************************************************************
\file	TextureManager.hpp
\author	Shirley Liau
\date	14/01/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	TextureManager is responsible for colors and images.

\par	Changes: None
**************************************************************************/

#pragma once
#ifndef _Texture_Man_Header_
#define _Texture_Man_Header_

#include <AEEngine.h>
#include <vector>
#include <string>

struct Color {
    f32 r, g, b, a;
};

struct FontFile {
    const char* filename;
};

struct ImageFile {
    const char* filename;
    f32 width, height;
};

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
    //constexpr ImageFile Background{ "../../Assets/Background.png", 1600.f, 900.f };
    constexpr ImageFile DigipenLogo{ "../../Assets/DigiPen_WHITE.png", 1525.f, 445.f };
    constexpr ImageFile Player{ "../../Assets/PlayerPlaceholder.png", 500.f,500.f };

    constexpr ImageFile StartBtn{ "../../Assets/Start_Button.png", 412.f, 120.f };
    constexpr ImageFile ExitBtn{ "../../Assets/Exit_Button.png", 412.f, 120.f };
    constexpr ImageFile CreditsBtn{ "../../Assets/Credits_Button.png", 412.f, 120.f };

    constexpr ImageFile Background{ "../../Assets/MenuBG.png", 1600.f, 900.f };
}

namespace Fonts
{
    constexpr FontFile Main{ "../../Assets/DynaPuff-Regular.ttf" };
    constexpr FontFile Alt{ "../../Assets/sourgummy-font.ttf" };
    constexpr FontFile Readable{ "../../Assets/alansans-font.ttf" };
}

class Texture
{
private:
    AEGfxTexture* _image;
    f32 _width;
    f32 _height;

public:
	void Fill_Color(const Color color);
	void Load_Image(const ImageFile& imageFilePath);
    void Set_Image(f32 x, f32 y);
    void Unload_Image();
    f32 Get_Image_Width() const;
    f32 Get_Image_Height() const;
    AEGfxTexture* Get_Image() const;
};

class Font {
private:
    s8 _font;
    f32 _fontSize;
    f32 _scale;
    Color _color;
    std::vector<std::string> _textArr;

public:
    Font();

    void Load_Font(const FontFile& fontFilePath, const f32 initHeight);
    void Input_Text(const char* text);
    void Draw_Text() const;
    void Draw_Text(f32 x, f32 y);
    void Draw_Text(AEVec2 pos, f32 scale, f32 fading);
    void Change_Text_Color(const Color color);
    void Free_Font() const;
};

#endif // !_Texture_Man_Header_