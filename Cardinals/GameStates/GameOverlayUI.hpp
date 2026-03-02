/*!***********************************************************************
\file	GameOverlayUI.cpp
\author	Zoey Tan
\date	31/01/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	GameOverlayUI is responsible for displaying the overlay menu.

\par	Changes: None
**************************************************************************/

#pragma once
#ifndef _Game_Overlay_UI_Header_
#define _Game_Overlay_UI_Header_

#include <functional>
#include "../header/UIManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/GStateManager.hpp"

extern bool isGamePaused;
extern bool isGameOver;

enum GAMEMODE {
	PAUSE,
	SETTINGS,
	LEVEL
};

enum STARRATING {
	ONE_STAR,
	TWO_STARS,
	THREE_STARS,
	LOSE
};

struct PauseMenuTextures
{
	Texture* bg = nullptr;
	Texture* cardBg = nullptr;
	Texture* bar = nullptr;
	Texture* knobMaster = nullptr;
	Texture* knobMusic = nullptr;
	Texture* knobSFX = nullptr;
	Texture* prevBtn = nullptr;
	Texture* resumeBtn = nullptr;
	Texture* restartBtn = nullptr;
	Texture* homeBtn = nullptr;
	Text* titleFont = nullptr;
};

struct GameOverTextures
{
	Texture* bg = nullptr;
	Texture* cardBg = nullptr;
	Texture* candy = nullptr;
	Texture* greyCandy = nullptr;
	Texture* restartBtn = nullptr;
	Texture* homeBtn = nullptr;
	Texture* nextBtn = nullptr;
	Text* titleFont = nullptr;
	Text* bodyFont = nullptr;
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

class Overlay : public UIElement
{
private:
	GAMEMODE _mode;
	GameObject bg, cardBG, candy1, candy2, candy3;

	Texture* candy = nullptr;
	Texture* greyCandy = nullptr;

	AEGfxVertexList* commonMesh = nullptr;
	VolSlider* _masterSlider = nullptr;
	VolSlider* _musicSlider = nullptr;
	VolSlider* _sfxSlider = nullptr;
	Button* _prevBtn = nullptr;
	Button* _quitBtn = nullptr;
	Button* _restartBtn = nullptr;
	Button* _resumeBtn = nullptr;
	Button* _homeBtn = nullptr;
	Button* _nextBtn = nullptr;
	Text* _titleFont = nullptr;
	Text* _bodyFont = nullptr;
	const char* _titleText = "";
	char buffer[128] = "";
	int _gameLevel = -1;
	f32 _timeTaken = 0.f;
	STARRATING performance = THREE_STARS;
	std::function<void()> _onResume;
	std::function<void()> _onRestart;
	std::function<void()> _onQuit;

public:

	Overlay(AEGfxVertexList* mesh, GAMEMODE mode, int levelIndex = -1, std::function<void()> onResume = nullptr, std::function<void()> onQuit = nullptr, std::function<void()> onRestart = nullptr);
	 
	void Update(f32 dt) override;
	void Draw() const override;
	void OnClick() override;

	void SetActive(bool active) override;
	void SetTextures(const PauseMenuTextures& tex);
	void SetTextures(const GameOverTextures& tex);

	void SetupPauseMenu();
	void SetupSettingsMenu();
	void SetupLevelMenu(int level);
	void UpdateGameOverMenu(f32 time, int health);
};

#endif