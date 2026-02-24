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
#include <iostream>

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
    constexpr ImageFile DigipenLogo{ "../../Assets/Textures/DigiPen_WHITE.png", 1525.f, 445.f };
    constexpr ImageFile Player{ "../../Assets/Textures/playerIdle.PNG", 500.f,500.f };

    constexpr ImageFile StartBtn{ "../../Assets/Textures/Start_Button.png", 412.f, 120.f };
	constexpr ImageFile PrevBtn{ "../../Assets/Textures/Previous_Button.png", 120.f, 120.f };
    constexpr ImageFile ExitBtn{ "../../Assets/Textures/Exit_Button.png", 412.f, 120.f };
    constexpr ImageFile CreditsBtn{ "../../Assets/Textures/Credits_Button.png", 412.f, 120.f };

    constexpr ImageFile YesBtn{ "../../Assets/Textures/Yes_Button.png", 412.f, 120.f };
    constexpr ImageFile NoBtn{ "../../Assets/Textures/No_Button.png", 412.f, 120.f };
    constexpr ImageFile PopUp{ "../../Assets/Textures/PopUp.png", 412.f, 120.f };


	constexpr ImageFile SettingsBtn{ "../../Assets/Textures/Settings_Button.png", 120.f, 120.f };
	constexpr ImageFile Bar{ "../../Assets/Textures/Generic_Bar.png", 500.f, 40.f };
	constexpr ImageFile KnobMaster{ "../../Assets/Textures/Master_Vol.png", 80.f, 80.f };
	constexpr ImageFile KnobMusic{ "../../Assets/Textures/Music_Vol.png", 80.f, 80.f };
	constexpr ImageFile KnobEFX{ "../../Assets/Textures/EFX_Vol.png", 80.f, 80.f };
    constexpr ImageFile CardBg{ "../../Assets/Textures/RoundedSq.png", 800.f, 600.f };
    constexpr ImageFile ResumeBtn{ "../../Assets/Textures/Resume_Button.png", 800.f, 600.f };
	constexpr ImageFile HomeBtn{ "../../Assets/Textures/Home_Button.png", 800.f, 600.f };
	constexpr ImageFile RestartBtn{ "../../Assets/Textures/Restart_Button.png", 800.f, 600.f };


    constexpr ImageFile Background{ "../../Assets/Textures/MenuBG.png", 1600.f, 900.f };
    constexpr ImageFile Placeholder{ "../../Assets/Textures/MissingTexture.png", 1600.f, 900.f };
    constexpr ImageFile Button{ "../../Assets/Textures/Button.png", 200.f, 200.f };
    constexpr ImageFile Lollipop{ "../../Assets/Textures/Lollipop.png", 100.f, 100.f };
    constexpr ImageFile CandyBall{ "../../Assets/Textures/Candy_Ball.png", 50.f, 50.f };

    constexpr ImageFile BG_BACKMOST{ "../../Assets/Textures/BG_BACKMOST.png", 1600.f, 900.f };
    constexpr ImageFile BG_MOUNTAIN{ "../../Assets/Textures/BG_BACKMOUNTAIN.png", 1600.f, 900.f };
    constexpr ImageFile BG_CLOUDS{ "../../Assets/Textures/BG_CLOUDS.png", 1600.f, 900.f };
    constexpr ImageFile BG_FRONTMOUNTAIN{ "../../Assets/Textures/BG_FRONTMOUNTAIN.png", 1600.f, 900.f };

    constexpr ImageFile Platform_VL{ "../../Assets/Textures/platform1.png" };
    constexpr ImageFile Platform_VM{ "../../Assets/Textures/platform2.png" };
    constexpr ImageFile Platform_VS{ "../../Assets/Textures/platform3.png" };
    constexpr ImageFile Platform_HS{ "../../Assets/Textures/platform4.png" };
    constexpr ImageFile Platform_HL{ "../../Assets/Textures/platform5.png" };
}

namespace Fonts
{
    constexpr FontFile Main{ "../../Assets/Fonts/DynaPuff-Regular.ttf" };
    constexpr FontFile Alt{ "../../Assets/Fonts/sourgummy-font.ttf" };
    constexpr FontFile Readable{ "../../Assets/Fonts/alansans-font.ttf" };
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

class Text {
private:
    s8 _font;
    f32 _fontSize;
    f32 _scale;
    Color _color;
    std::vector<std::string> _textArr;

public:
    Text();

    void Load_Font(const FontFile& fontFilePath, const f32 initHeight);
    void Input_Text(const char* text);
    void Input_Float(float t);
    void Draw_Text() const;
    void Draw_Text(f32 x, f32 y);
    void Draw_Text_Corner(f32 x, f32 y);
    void Draw_Text(AEVec2 pos, f32 scale, f32 fading);
    void Change_Text_Color(const Color color);
    void Free_Font() const;
};

#endif // !_Texture_Man_Header_