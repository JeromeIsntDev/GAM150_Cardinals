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

#include <functional>
#include "../header/UIManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/GStateManager.hpp"

extern int isGamePaused;

struct PauseMenuTextures
{
	Texture* bg = nullptr;
	Texture* cardBg = nullptr;
	Texture* bar = nullptr;
	Texture* knobMaster = nullptr;
	Texture* knobMusic = nullptr;
	Texture* knobSFX = nullptr;
	Texture* prevBtn = nullptr;
};

class VolSlider : public UIElement
{
private:

	UIElement knob;
	AudioType audioType;
	f32 width;
	bool isDragging{ false };

public:
	
	VolSlider(AEGfxVertexList* mesh, AudioType type, AEVec2 pos, f32 width);
	
	void Update(f32 dt) override; // from UIElement
	void Draw() const override; // from GameObject
	void OnClick() override;

	void SetTextures(Texture* barTex, Texture* knobTex); // unique to VolSlider
	AudioType GetAudioType() const { return audioType; }
};

class PauseMenu : public UIElement
{
private:

	GameObject bg, cardBG;

	AEGfxVertexList* commonMesh = nullptr;
	VolSlider* _masterSlider = nullptr;
	VolSlider* _musicSlider = nullptr;
	VolSlider* _sfxSlider = nullptr;
	Button* _prevBtn = nullptr;
	Button* _quitBtn = nullptr;

public:

	PauseMenu(AEGfxVertexList* mesh, bool isInGame, std::function<void()> onResume = nullptr, std::function<void()> onQuit = nullptr);

	void Update(f32 dt) override;
	void Draw() const override;
	void OnClick() override;

	void SetActive(bool active) override;
	void SetTextures(const PauseMenuTextures& tex);
};

#endif