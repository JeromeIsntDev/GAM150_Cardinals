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

AEGfxTexture* Texture::Load_Image(const Image& image)
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
