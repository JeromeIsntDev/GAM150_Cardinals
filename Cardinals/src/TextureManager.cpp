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

f32 Texture::Get_Image_Width() const
{
	return _width;
}

f32 Texture::Get_Image_Height() const
{
	return _height;
}

void Texture::Unload_Image()
{
	PRINT("Unload Image\n");
	AEGfxTextureUnload(_image);
	_image = nullptr;
}

AEGfxTexture* Texture::Get_Image() const
{
	return _image;
}


//Font
Font::Font() 
	:_font{ 0 }, _fontSize{}, _scale {1.f},
	_color{ Colors::White }, _textArr{ "" } {

}

void Font::Load_Font(const FontFile& fontFilePath, const f32 initHeight)
{
	_fontSize = initHeight;
	_font = AEGfxCreateFont(fontFilePath.filename, static_cast<int>(initHeight));
}

void Font::Input_Text(const char* text)
{
	_textArr.clear();
	std::string str(text);
	size_t start = 0;
	size_t pos;

	while ((pos = str.find('\n', start)) != std::string::npos) {
		_textArr.push_back(str.substr(start, pos - start));
		start = pos + 1;
	}

	_textArr.push_back(str.substr(start));
}

void Font::Draw_Text() const
{
	f32 yOffset = 0.0f;

	for (const std::string& line : _textArr)
	{
		f32 w{ 0 }, h{ 0 };
		AEGfxGetPrintSize(_font, line.c_str(), _scale, &w, &h);

		AEGfxPrint(
			_font,
			line.c_str(),
			(-w / 2),
			(-h / 2) - yOffset,
			_scale,
			_color.r, _color.g, _color.b, _color.a
		);

		yOffset += _fontSize * 2 / 900.f;
	}
}

void Font::Draw_Text(f32 x, f32 y)
{
	f32 yOffset = 0.0f;
	for (const std::string& line : _textArr)
	{
		f32 w{ 0 }, h{ 0 };

		AEGfxGetPrintSize(_font, line.c_str(), _scale, &w, &h);

		AEGfxPrint(
			_font,
			line.c_str(),
			(-w / 2) + x / 800.f,
			(-h / 2) + y / 450.f - yOffset,
			_scale,
			_color.r, _color.g, _color.b, _color.a
		);

		yOffset += _fontSize * 2 / 900.f;
	}
}

void Font::Draw_Text(AEVec2 pos, f32 scale, f32 fading)
{
	_scale = scale;
	f32 yOffset = 0.0f;
	for (const std::string& line : _textArr)
	{
		f32 w{ 0 }, h{ 0 };

		AEGfxGetPrintSize(_font, line.c_str(), scale, &w, &h);
		AEGfxPrint(
			_font,
			line.c_str(),
			(-w / 2) + pos.x / 800.f,
			(-h / 2) + pos.y / 450.f - yOffset,
			_scale,
			_color.r, _color.g, _color.b, fading
		);
		yOffset += _fontSize * 2 / 900.f;
	}
}

void Font::Change_Text_Color(const Color color)
{
	_color = color;
}

void Font::Free_Font() const
{
	AEGfxDestroyFont(_font);
}
