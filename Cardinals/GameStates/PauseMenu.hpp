/*!***********************************************************************
\file	PauseMenu.cpp
\author	Zoey Tan
\date	31/01/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	PauseMenu is responsible for displaying the pause menu.

\par	Changes: None
**************************************************************************/

#pragma once
#ifndef _Pause_Menu_Header_
#define _Pause_Menu_Header_

#include "../header/UIManager.hpp"
#include "../header/AudioManager.hpp"
#include <vector>
#include <memory>

class VolSlider : public UIElement
{
public:
	VolSlider(AEGfxVertexList* mesh, AudioType type, AEVec2 pos, f32 width);
	void Update(f32 dt) override; // from UIElement
	void Draw() const override; // from GameObject
	void SetTextures(Texture* barTex, Texture* knobTex); // unique to VolSlider

private:
	AudioType _audioType;
	f32 _width;
	bool _isDragging = false;
	UIElement _knob;
};

class PauseMenu
{
public:
	PauseMenu(bool isInGame);
	~PauseMenu();
	void Update(f32 dt);
	void Draw();
	void ShowMenu();
	void HideMenu();
	bool isVisible() const { return _isVisible; }
	void SetTextures(Texture* bg, Texture* bar, Texture* knob, Texture* prevBtn);
private:
	bool _isVisible = false;
	// to manage my pause menu UI
	UIManager _pauseMenuUI;
	GameObject _bg;
	// store sliders and buttons etc.
	std::vector<std::unique_ptr<UIElement>>_elements;
	AEGfxVertexList* _commonMesh = nullptr;
};

#endif