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

#include <AEEngine.h>

struct Color {
    f32 r, g, b, a;
};

struct FilePath {
    const char* filename;
};
namespace Colors
{
    constexpr const Color White{ 1.f, 1.f, 1.f, 1.f };
    constexpr const Color Black{ 0.f, 0.f, 0.f, 1.f };
    constexpr const Color PastelPink{ 242.f / 255.f, 198.f / 255.f, 222.f / 255.f, 1.f };
    constexpr const Color PastelOrange{ 247.f / 255.f, 217.f / 255.f, 196.f / 255.f, 1.f };
    constexpr const Color PastelPurple{ 219.f / 255.f, 205.f / 255.f, 240.f / 255.f, 1.f };
    constexpr const Color PastelBlue{ 198.f / 255.f, 222.f / 255.f, 241.f / 255.f, 1.f };
    constexpr const Color PastelGreen{ 201.f / 255.f, 228.f / 255.f, 222.f / 255.f, 1.f };
    constexpr const Color PastelYellow{ 250.f / 255.f, 237.f / 255.f, 203.f / 255.f, 1.f };
}

namespace Images
{
    constexpr const FilePath Planet{ "../../Assets/PlanetTexture.png" };
}

namespace Fonts
{
    constexpr const FilePath Main{ "../../Assets/DynaPuff-Regular.ttf" };
}
class Texture
{
public:

	void Fill_Color(const Color color);
	AEGfxTexture* Load_Image(const FilePath& image);
    void Draw_Image(AEGfxTexture* image, f32 x, f32 y);
    void Free_Image(AEGfxTexture* image);
    s8 Load_Font(const FilePath& font);
    void Draw_Text(s8 font, const char* text, f32 x, f32 y, f32 scale, const Color color);
    void Free_Font(s8 font);
};

#endif // !_Texture_Man_Header_