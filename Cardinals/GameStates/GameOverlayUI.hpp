/**
 * @file GameOverlayUI.hpp
 * @brief Header file for the GameOverlayUI class
 * @details Manages the game's overlay user interface, including pause menu, settings menu, and level completion screen.
 * @author Zoey Tan
 * @date 31.01.2026
 */

#pragma once
#ifndef _Game_Overlay_UI_Header_
#define _Game_Overlay_UI_Header_

#include <functional>
#include "../header/UIManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/GStateManager.hpp"

extern bool isGamePaused;
extern bool isGameOver;
/**
 * @brief Enum representing different game modes for the overlay UI.	
 * 
 */
enum GAMEMODE {
	PAUSE,
	SETTINGS,
	LEVEL
};

/**
 * @brief Enum representing the star rating for level completion.
 * 
 */
enum STARRATING {
	ONE_STAR,
	TWO_STARS,
	THREE_STARS,
	LOSE
};


/**
 * @brief Structure to hold textures for the pause menu.
 * 
 */
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

/**
 * @brief Structure to hold textures for the game over menu.
 * 
 */
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

/**
 * @class VolSlider 
 * @brief A slider for controlling audio volume.
 */
class VolSlider : public UIElement
{
private:

	UIElement knob;
	AudioType audioType;
	f32 width;
	bool isDragging{ false };

public:
	/**
	 * @brief Construct a new Vol Slider object
	 * 
	 * @param mesh 
	 * @param type 
	 * @param pos 
	 * @param width 
	 */
	VolSlider(AEGfxVertexList* mesh, AudioType type, AEVec2 pos, f32 width);
	
	/**
	 * @brief Updates the slider state based on user interaction 
	 *		  and synchronizes the knob position with the current volume level.
	 * 
	 * @param dt 
	 */
	void Update(f32 dt) override; // from UIElement

	/**
	 * @brief Draws the slider on the screen.
	 * 
	 */
	void Draw() const override; // from GameObject

	/**
	 * @brief Handles click events for the slider, allowing the user to drag the knob and adjust the volume.
	 * 
	 */
	void OnClick() override;

	/**
	 * @brief Synchronizes the knob position with the current volume level.
	 * 
	 */
	void SyncKnobToCurrentVolume();

	/**
	 * @brief Set the Textures object
	 * 
	 * @param barTex 
	 * @param knobTex 
	 */
	void SetTextures(Texture* barTex, Texture* knobTex); // unique to VolSlider

	/**
	 * @brief Get the Audio Type object	
	 * 
	 * @return AudioType 
	 */
	AudioType GetAudioType() const { return audioType; }
};

/**
 * @class Overlay
 * @brief Manages different game overlay states (pause, settings, level completion).
 */
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
	AudioManager& bgm = AudioManager::Instance();

public:
	/**
	* @brief Construct a new Overlay object
	* 
	* @param mesh 
	* @param mode 
	* @param levelIndex 
	* @param onResume 
	* @param onQuit 
	* @param onRestart 
	*/
	Overlay(AEGfxVertexList* mesh, GAMEMODE mode, int levelIndex = -1, std::function<void()> onResume = nullptr, std::function<void()> onQuit = nullptr, std::function<void()> onRestart = nullptr);
	 
	/**
	 * @brief Updates the overlay state based on user interaction.
	 * 
	 * @param dt 
	 */
	void Update(f32 dt) override;

	/**
	 * @brief Draws the overlay on the screen.
	 * 
	 */
	void Draw() const override;

	/**
	 * @brief Handles click events for the overlay.
	 * 
	 */
	void OnClick() override;

	/**
	 * @brief Set the Active object	
	 * 
	 * @param active 
	 */
	void SetActive(bool active) override;

	/**
	 * @brief Set the Textures object
	 * 
	 * @param pauseTex 
	 */
	void SetTextures(const PauseMenuTextures& pauseTex);

	/**
	 * @brief Set the Textures object
	 * 
	 * @param gameOverTex 
	 */
	void SetTextures(const GameOverTextures& gameOverTex);

	/**
	 * @brief Sets up the pause menu.
	 * 
	 */
	void SetupPauseMenu();

	/**
	 * @brief Sets up the settings menu.
	 * 
	 */
	void SetupSettingsMenu();

	/**
	 * @brief Sets up the level menu.
	 * 
	 */
	void SetupLevelMenu();

	/**
	 * @brief Updates the game over menu with the latest time and health information.
	 * 
	 * @param time 
	 * @param health 
	 */
	void UpdateGameOverMenu(f32 time, int health);
};

#endif