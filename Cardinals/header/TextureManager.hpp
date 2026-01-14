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

struct Image {
    const char* filename;
};
namespace Colors
{
    constexpr const Color White{ 1.f, 1.f, 1.f, 1.f };
    constexpr const Color Black{ 0.f, 0.f, 0.f, 1.f };
    constexpr const Color Red{ 1.f, 0.f, 0.f, 1.f };
    constexpr const Color Green{ 0.f, 1.f, 0.f, 1.f };
    constexpr const Color Blue{ 0.f, 0.f, 1.f, 1.f };
}

namespace Images
{
    constexpr const Image Planet{ "../../Assets/PlanetTexture.png" };
}

#endif // !_Texture_Man_Header_