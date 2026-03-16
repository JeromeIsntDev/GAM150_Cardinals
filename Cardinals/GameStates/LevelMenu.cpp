#include "LevelMenu.hpp"


namespace {
	AEGfxVertexList* buttonMesh;

	Text textToUse;
	Texture buttonTexture;
	Texture exitTexture;
	std::unique_ptr<Button> lvl1Btn, lvl2Btn, lvl3Btn, lvl4Btn, lvl5Btn;
	std::unique_ptr<Button> backBtn;
	
	UIManager uiManager;
}


void LevelMenu_Load() {
	PRINT("Load: %s", "Level Menu\n");
	textToUse.Load_Font(Fonts::Main, 100.f);
	textToUse.Input_Text("Level Menu");

	buttonTexture.Load_Image(Images::Button);
	exitTexture.Load_Image(Images::ExitBtn);

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddTexture(&buttonTexture);
	memoryManager.AddTexture(&exitTexture);

	if (!memoryManager.meshes.empty()) memoryManager.meshes.clear();
	memoryManager.AddMesh(&buttonMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();

	// only uncomment if you want tooltips here bruh
	//uiManager.InitTooltipFont(Fonts::Main, 30.f);
}

void LevelMenu_Initialize()
{
	PRINT("Initialize: %s", "Level Menu\n");

	buttonMesh = RectMeshCenter();
	lvl1Btn = std::make_unique<Button>(buttonMesh, AEVec2{ -300.f, 50.f }, 0.f, AEVec2{ buttonTexture.Get_Image_Width(), buttonTexture.Get_Image_Height() });
	lvl1Btn->SetTexture(&buttonTexture);
	lvl1Btn->SetActive(true);
	lvl1Btn->SetInteractable(true);
	lvl1Btn->SetOnClick([]() {
		PRINT("Level 1 Pressed\n");
		UpdateGameManagerState(GameStateEnum::Level1);
		});


	lvl2Btn = std::make_unique<Button>(buttonMesh,
		AEVec2{ 0.f, 50.f },
		0.f,
		AEVec2{ 200.f, 200.f });
	lvl2Btn->SetTexture(&buttonTexture);
	lvl2Btn->SetActive(true);
	lvl2Btn->SetInteractable(true);
	lvl2Btn->SetOnClick([]() {
		PRINT("Level 2 Pressed\n");
		UpdateGameManagerState(GameStateEnum::Level2);
		});


	lvl3Btn = std::make_unique<Button>(buttonMesh,
		AEVec2{ 300.f, 50.f },
		0.f,
		AEVec2{ 200.f, 200.f });
	lvl3Btn->SetTexture(&buttonTexture);
	lvl3Btn->SetActive(true);
	lvl3Btn->SetInteractable(true);
	lvl3Btn->SetOnClick([]() {
		PRINT("Level 3 Pressed\n");
		UpdateGameManagerState(GameStateEnum::Level3);
		});


	lvl4Btn = std::make_unique<Button>(buttonMesh,
		AEVec2{ -150.f, -200.f },
		0.f,
		AEVec2{ 200.f, 200.f });
	lvl4Btn->SetTexture(&buttonTexture);
	lvl4Btn->SetActive(true);
	lvl4Btn->SetInteractable(true);


	lvl5Btn = std::make_unique<Button>(buttonMesh,
		AEVec2{ 150.f, -200.f },
		0.f,
		AEVec2{ 200.f, 200.f });
	lvl5Btn->SetTexture(&buttonTexture);
	lvl5Btn->SetActive(true);
	lvl5Btn->SetInteractable(true);

	backBtn = std::make_unique<Button>(buttonMesh,
		AEVec2{ -625.f, 375.f },
		0.f,
		AEVec2{ 280.f, 80.f });
	backBtn->SetTexture(&exitTexture);
	backBtn->SetActive(true);
	backBtn->SetInteractable(true);
	backBtn->SetOnClick([]() {
		PRINT("Back Pressed");
		UpdateGameManagerState(GameStateEnum::MainMenu);
		});

	uiManager.Add(lvl1Btn.get());
	uiManager.Add(lvl2Btn.get());
	uiManager.Add(lvl3Btn.get());
	uiManager.Add(lvl4Btn.get());
	uiManager.Add(lvl5Btn.get());
	uiManager.Add(backBtn.get());
}

void LevelMenu_Update() {
	//PRINT("Update: %s", "Main Menu\n");

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(Colors::PastelPurple.r, Colors::PastelPurple.g, Colors::PastelPurple.b);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetTransparency(1.f);

	uiManager.Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));
}

void LevelMenu_Draw() {
	//PRINT("Draw: %s", "Level Menu\n");

	textToUse.Input_Text("Select Your Level");
	textToUse.Draw_Text(0.f, 300.f);

	uiManager.Draw();

	textToUse.Input_Text("1");
	textToUse.Draw_Text(-300.f, 50.f);
	textToUse.Input_Text("2");
	textToUse.Draw_Text(0.f, 50.f);
	textToUse.Input_Text("3");
	textToUse.Draw_Text(300.f, 50.f);
	textToUse.Input_Text("4");
	textToUse.Draw_Text(-150.f, -200.f);
	textToUse.Input_Text("5");
	textToUse.Draw_Text(150.f, -200.f);
}

void LevelMenu_Free() {
	PRINT("Free: %s", "Level Menu\n");
	textToUse.Free_Font();
	uiManager.Exit();
}

void LevelMenu_Unload() {
	PRINT("Unload: %s", "Level Menu\n");

	if (memoryManager.MassUnload()) PRINT("Unload Success!");
}
