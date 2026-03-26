#include "../GameStates/Level5.hpp"
#include "GameEditor.hpp"

void Level5_EditorUpdate();
void Level5_EditorDraw();
void Level5_Parallax();
void Level5_UIDraw();

extern bool isGamePaused;
extern bool isGameOver;

namespace {
	AEGfxVertexList* squareMesh;
	Text digipenFont, pauseTitleFont;


	//PLAYER VARIABLES
	f32 playerMovespeed{ 300.f };
	int  playerHealth{ 3 };
	Texture* playerTexture[2]{};
	ParticleEffect playerEffect;
	ParticleEffect gravityEffect;

	TextureManager& tex = TextureManager::GetInstance();
	AudioManager& bgm = AudioManager::Instance();

	Texture* placeholderTexture;
	Texture verticalPlatformTex;
	Texture horizontalPlatformTex;

	auto& input = InputHandler::GetInstance();

	//UIManager pauseUI;
	//std::unique_ptr<Button> conBtn, quitBtn;

	std::unique_ptr <Overlay> pauseMenu;
	std::unique_ptr<Overlay> gameOverMenu;
	Texture* barTexture{}, * knobMasterTexture{}, * knobMusicTexture{},
		* knobEFXTexture{}, * cardBgTexture{}, * prevTexture{},
		* restartTexture{}, * resumeTexture{}, * homeTexture{},
		* nextTexture{}, * retryTexture{}, * candyTexture{},
		* greyCandyTexture{};

	f32 levelTime{};

	GameObject gamelives;

	Texture* backgrounds[4]{};
	GameObject backgroundObjs[4]{};

	Grid grid;
	Player mainPlayer;
}

void Level5_Load() {

}

void Level5_Initialize() {

}
void Level5_Update() {

}
void Level5_Draw() {

}
void Level5_Free() {

}
void Level5_Unload() {

}


void Level5_EditorUpdate() {

}
void Level5_EditorDraw() {

}
void Level5_Parallax() {

}
void Level5_UIDraw() {

}