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

class Texture
{
public:

	void Fill_Color(const Color color) {
		AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
	}

	AEGfxTexture* Load_Image(const Image& image) {
		AEGfxTexture* img = AEGfxTextureLoad(image.filename);
		return img;
	}

	void Draw_Image(AEGfxTexture* image, f32 x, f32 y) {
		AEGfxTextureSet(image, x, y);
	}

	void Free_Image(AEGfxTexture* image) {
		AEGfxTextureUnload(image);
	}
};