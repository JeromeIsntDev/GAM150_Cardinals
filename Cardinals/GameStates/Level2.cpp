#include "Level2.hpp"

extern std::vector<Texture*> textures;
extern std::vector<AEGfxVertexList**> meshes;
extern std::vector<Text*> texts;
extern bool MassUnload();
extern int isGamePaused;

namespace {
	Text digipenFont;

	Texture placeholder;
	Texture playerTexture;

	GameObject placeholderObj;
	GameObject playerObj;

	AEGfxVertexList* rectMesh;

	UIManager pauseUI;
	std::unique_ptr<Button> conBtn, quitBtn;
}

void Level2_Load() {
	PRINT("Load: %s", "Level 2\n");

	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 2");
	digipenFont.Change_Text_Color(Colors::Black);

	placeholder.Load_Image(Images::Placeholder);
	playerTexture.Load_Image(Images::Player);
}

void Level2_Initialize()
{
	PRINT("Initialize: %s", "Level 2\n");

	rectMesh = RectMeshCenter();

	placeholderObj.SetMesh(rectMesh);
	placeholderObj.SetTexture(&placeholder);
	placeholderObj.UpdateScale(AEVec2{ 1600.f, 900.f });
	placeholderObj.UpdateRotation(0.f);

	playerObj.SetMesh(rectMesh);
	playerObj.SetTexture(&playerTexture);
	playerObj.UpdateScale(AEVec2{ 100.f, 100.f });
	playerObj.UpdateRotation(0.f);


	conBtn = std::make_unique<Button>(rectMesh,
		AEVec2{ 0.f, 50.f }, 
		0.f, 
		AEVec2{ 200.f, 100.f });
	conBtn->SetTexture(&placeholder); conBtn->SetActive(true); conBtn->SetInteractable(true);
	conBtn->SetOnClick([]() {
		PRINT("Continue Pressed\n");
		isGamePaused = 0;
		});

	quitBtn = std::make_unique<Button>(rectMesh,
		AEVec2{ 0.f, -100.f },
		0.f,
		AEVec2{ 200.f, 100.f });
	quitBtn->SetTexture(&placeholder); quitBtn->SetActive(true); quitBtn->SetInteractable(true);
	quitBtn->SetOnClick([]() {
		PRINT("Exit Pressed\n");
		UpdateGameManagerState(GameStateEnum::MainMenu);
		});

	pauseUI.Add(conBtn.get());
	pauseUI.Add(quitBtn.get());

	if (!textures.empty()) textures.clear();
	textures.push_back(&playerTexture);
	textures.push_back(&placeholder);

	if (!meshes.empty()) meshes.clear();
	meshes.push_back(&rectMesh);

	if (!texts.empty()) texts.clear();
	texts.push_back(&digipenFont);
}

void Level2_Update() {
	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.9f, 0.85f, 0.65f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);

	auto& input = InputHandler::GetInstance();

	if (input.isMouseBtnTriggered(AEVK_P)) {
		isGamePaused = ~isGamePaused;
	}

	
	if (!isGamePaused) return;
	pauseUI.Update(0.f);
}

void Level2_Draw() {
	PRINT("Draw: %s", "Level 2\n");
	digipenFont.Input_Text("Level 2");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);
	playerObj.Draw();

	if (!isGamePaused) return;
	AEGfxSetTransparency(.8f);

	placeholderObj.Draw();

	digipenFont.Input_Text("Game Paused");
	digipenFont.Draw_Text(AEVec2{ 0.f, 250.f }, .5f, 1.f);
	digipenFont.Change_Text_Color(Colors::White);

	AEGfxSetTransparency(1.f);
	pauseUI.Draw();
	
}

void Level2_Free() {
	PRINT("Free: %s", "Level 2\n");
}

void Level2_Unload() {
	PRINT("Unload: %s", "Level 2\n");

	pauseUI.Exit();
	if (isGamePaused) isGamePaused = 0;
	if (MassUnload()) PRINT("Unload Success");
}
