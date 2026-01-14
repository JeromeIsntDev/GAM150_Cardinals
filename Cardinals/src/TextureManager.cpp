/*!***********************************************************************
\file	TextureManager.cpp
\author	Shirley Liau
\date	14/01/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	TextureManager is responsible for colors and images.

\par	Changes: None
**************************************************************************/

#include "../header/TextureManager.hpp"

void Texture::Fill_Color(const Color color) 
{
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
}

AEGfxTexture* Texture::Load_Image(const FilePath& image)
{
	AEGfxTexture* img = AEGfxTextureLoad(image.filename);
	return img;
}

void Texture::Draw_Image(AEGfxTexture* image, f32 x, f32 y)
{
	AEGfxTextureSet(image, x, y);
}

void Texture::Free_Image(AEGfxTexture* image)
{
	AEGfxTextureUnload(image);
}

s8 Texture::Load_Font(const FilePath& font)
{
	return AEGfxCreateFont(font.filename, 72.f);
}

void Texture::Draw_Text(s8 font, const char* text, f32 x, f32 y, f32 scale, const Color color)
{
	f32 w, h;
	AEGfxGetPrintSize(font, text, scale, &w, &h);
	AEGfxPrint(font, text, (- w / 2) + x, (- h / 2) + y, scale, color.r, color.g, color.b, color.a);
}

void Texture::Free_Font(s8 font)
{
	AEGfxDestroyFont(font);
}
