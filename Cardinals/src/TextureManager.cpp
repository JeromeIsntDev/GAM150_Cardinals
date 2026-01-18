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

void Texture::Load_Image(const FilePath& imageFilePath)
{
	image = AEGfxTextureLoad(imageFilePath.filename);
}

void Texture::Set_Image(f32 x, f32 y)
{
	AEGfxTextureSet(image, x, y);
}

void Texture::Free_Image()
{
	AEGfxTextureUnload(image);
}

AEGfxTexture* Texture::Get_Image() const
{
	return image;
}

void Font::Load_Font(const FilePath& fontFilePath, const f32 initHeight)
{
	font = AEGfxCreateFont(fontFilePath.filename, static_cast<int>(initHeight));
}

void Font::Draw_Text(const char* text, f32 x, f32 y, f32 scale, const Color color) const
{
	f32 w{ 0 }, h{ 0 };
	AEGfxGetPrintSize(font, text, scale, &w, &h);
	AEGfxPrint(font, text, (- w / 2) + x/800.f, (- h / 2) + y/450.f, 
		scale, color.r, color.g, color.b, color.a);
}

void Font::Free_Font() const
{
	AEGfxDestroyFont(font);
}
