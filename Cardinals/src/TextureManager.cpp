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

void Texture::Load_Image(const ImageFile& imageFilePath)
{
	_image = AEGfxTextureLoad(imageFilePath.filename);
	_width = imageFilePath.width;
	_height = imageFilePath.height;
	PRINT("Load Image: %s\n", imageFilePath.filename);
}

void Texture::Set_Image(f32 x, f32 y)
{	
	AEGfxTextureSet(_image, x, y);
}

f32 Texture::Get_Image_Width()
{
	return _width;
}

f32 Texture::Get_Image_Height()
{
	return _height;
}

void Texture::Free_Image()
{
	AEGfxTextureUnload(_image);
}

AEGfxTexture* Texture::Get_Image() const
{
	return _image;
}

Font::Font() :_font{0}, _scale{1.f}, _color { Colors::White } {

}
void Font::Load_Font(const FontFile& fontFilePath, const f32 initHeight)
{
	_font = AEGfxCreateFont(fontFilePath.filename, static_cast<int>(initHeight));
}

void Font::Input_Text(const char* text)
{
	_text = text;
}

void Font::Draw_Text() const
{
	f32 w{ 0 }, h{ 0 };
	AEGfxGetPrintSize(_font, _text, _scale, &w, &h);
	AEGfxPrint(_font, _text, (-w / 2), (-h / 2),
		_scale, _color.r, _color.g, _color.b, _color.a);
}

void Font::Draw_Text(f32 x, f32 y)
{
	f32 w{ 0 }, h{ 0 };
	AEGfxGetPrintSize(_font, _text, _scale, &w, &h);
	AEGfxPrint(_font, _text, (- w / 2) + x/800.f, (- h / 2) + y/450.f, 
		_scale, _color.r, _color.g, _color.b, _color.a);
}

void Font::Draw_Text(AEVec2 pos, f32 scale)
{
	f32 w{ 0 }, h{ 0 };
	_scale = scale;
	AEGfxGetPrintSize(_font, _text, scale, &w, &h);
	AEGfxPrint(_font, _text, (-w / 2) + pos.x / 800.f, (-h / 2) + pos.y / 450.f,
		_scale, _color.r, _color.g, _color.b, _color.a);
}

void Font::Change_Text_Color(const Color color)
{
	_color = color;
}

void Font::Free_Font() const
{
	AEGfxDestroyFont(_font);
}
