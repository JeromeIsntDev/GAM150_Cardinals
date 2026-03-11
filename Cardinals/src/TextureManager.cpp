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

///
/// TEXTURE
///

void Texture::Fill_Color(const Color color) 
{
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
}

void Texture::Load_Image(const ImageFile& imageFilePath)
{
	_image = AEGfxTextureLoad(imageFilePath.filename);
	_width = imageFilePath.width;
	_height = imageFilePath.height;
	PRINT("loading image: %s\n", imageFilePath.filename);
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
	if (!_image) return; // already unloaded or never loaded DIE DIE DIE DIE
	PRINT("unloading Image\n");
	AEGfxTextureUnload(_image);
	_image = nullptr;
}

AEGfxTexture* Texture::Get_Image() const
{
	// if it crashes it likely means your manifest filepath is wrronggg or you fucked up the
	// definition of the texture which shouldn't be the case since all you should be touching
	// is the manifest

	return _image;
}

// overloaded
void Texture::Load_Image(const std::string& path, float w, float h)
{
	if (_image)
	{
		AEGfxTextureUnload(_image);
		_image = nullptr;
	}

	_image = AEGfxTextureLoad(path.c_str());
	_width = w;
	_height = h;

	PRINT("at %s, result: %s | dimensions: %.0fx%.0f\n", path.c_str(), _image ? "loaded bro" : "failed bro", _width, _height);
}

///
/// TEXTUREMANAGER
///

void TextureManager::LoadManifest(const std::string& path)
{
	std::ifstream file(path);
	std::string line;

	if (!file.is_open())
	{
		PRINT("ERROR: where tf is ur manifest?? at %s\n", path.c_str());
		return;
	}

	while (std::getline(file, line))
	{
		// skip empty or lines starting with #
		if (line.empty() || line[0] == '#') continue;

		std::stringstream ss(line);
		std::string name, filePath, width, height;

		if (std::getline(ss, name, ',') &&
			std::getline(ss, filePath, ',') &&
			std::getline(ss, width, ',') &&
			std::getline(ss, height))
		{
			name.erase(0, name.find_first_not_of(" \t"));
			name.erase(name.find_last_not_of(" \t") + 1);
			filePath.erase(0, filePath.find_first_not_of(" \t"));
			filePath.erase(filePath.find_last_not_of(" \t\r\n") + 1);

			float w = std::stof(width), h = std::stof(height);

			textureMap[name].Load_Image(filePath, w, h);
		}
	}
}

Texture* TextureManager::Get(const std::string& name)
{
	auto it = textureMap.find(name);

	if (it == textureMap.end()) 
	{
		PRINT("[WARNING | Texture] => Texture not found: %s\n", name.c_str());
		return nullptr;
	}
	return &it->second;
}

void TextureManager::UnloadAll()
{
	for (auto it = textureMap.begin(); it != textureMap.end();)
	{
		if (persistentLoad.count(it->first))
		{
			PRINT("[DEBUG | Texture] => Persistent: %s\n", it->first.c_str());
			it++;
		}
		else
		{
			PRINT("[DEBUG | Texture] => Unloading: %s\n", it->first.c_str());
			it->second.Unload_Image();
			it = textureMap.erase(it);
		}
	}
}

void TextureManager::SetPersistent(const std::string& name)
{
	// omg waow so thats how it works
	persistentLoad.insert(name);
}

void TextureManager::UnloadPersistent() 
{
	for (auto& key : persistentLoad) 
	{
		PRINT("unloading persistent: %s\n", key.c_str());
		if (textureMap.count(key))
		{
			textureMap[key].Unload_Image();
		}
		else
		{
			PRINT("key not in mmmap: %s\n", key.c_str());
		}
	}

	persistentLoad.clear();
	textureMap.clear();
}

///
/// TEXT
///

Text::Text()
	:_font{ 0 }, _fontSize{}, _scale {1.f},
	_color{ Colors::White }, _textArr{ "" } {

}

void Text::Load_Font(const FontFile& fontFilePath, const f32 initHeight)
{
	_fontSize = initHeight;
	_font = AEGfxCreateFont(fontFilePath.filename, static_cast<int>(initHeight));
}

void Text::Input_Text(const char* text)
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

void Text::Input_Float(float t) {
	_textArr.clear();
	u32 levelTrunc = static_cast<int>(t);
	std::string str = std::to_string(levelTrunc);
	size_t start = 0;
	size_t pos;

	while ((pos = str.find('\n', start)) != std::string::npos) {
		_textArr.push_back(str.substr(start, pos - start));
		start = pos + 1;
	}

	_textArr.push_back(str.substr(start));
}

void Text::Draw_Text() const
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

void Text::Draw_Text(f32 x, f32 y)
{
	f32 yOffset = 0.0f;
	for (const std::string& line : _textArr)
	{
		f32 w{ 0 }, h{ 0 };

		AEGfxGetPrintSize(_font, line.c_str(), _scale, &w, &h);

		AEGfxPrint(_font, line.c_str(), (-w / 2) + x / 800.f, (-h / 2) + y / 450.f - yOffset,
			_scale,
			_color.r, _color.g, _color.b, _color.a
		);

		yOffset += _fontSize * 2 / 900.f;
	}
}

void Text::Draw_Text_Corner(f32 x, f32 y)
{
	f32 yOffset = 0.0f;
	for (const std::string& line : _textArr)
	{
		f32 w{ 0 }, h{ 0 };
		AEGfxGetPrintSize(_font, line.c_str(), _scale, &w, &h);
		AEGfxPrint(_font, line.c_str(), x / 800.f,  y / 450.f - yOffset,
			_scale,
			_color.r, _color.g, _color.b, _color.a
		);

		yOffset += _fontSize * 2 / 900.f;
	}
}

void Text::Draw_Text(AEVec2 pos, f32 scale, f32 fading)
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

void Text::Change_Text_Color(const Color color)
{
	_color = color;
}

void Text::Free_Font() const
{
	AEGfxDestroyFont(_font);
}