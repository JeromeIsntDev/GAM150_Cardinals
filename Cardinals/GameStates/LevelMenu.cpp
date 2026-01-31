#include "../GameStates/LevelMenu.hpp"

extern std::vector<Texture> textures;

namespace {
	AEGfxVertexList* buttonMesh;

	Text textToUse;
	Texture buttonTexture;
	std::unique_ptr<Button> lvl1Btn, lvl2Btn, lvl3Btn, lvl4Btn, lvl5Btn;
	std::unique_ptr<Button> backToMainBtn;
	UIManager uiManager;
}


void LevelMenu_Load() {
	PRINT("Load: %s", "Level Menu\n");
	textToUse.Load_Font(Fonts::Main, 160.f);
	textToUse.Input_Text("Level Menu");

	buttonTexture.Load_Image(Images::Button);

	if (!textures.empty()) textures.clear();
	textures.push_back(buttonTexture);
}

void LevelMenu_Initialize()
{
	PRINT("Initialize: %s", "Level Menu\n");

	buttonMesh = RectMeshCenter();

	lvl1Btn = std::make_unique<Button>(buttonMesh, 
		AEVec2{ 0.f, 0.f }, 
		0.f, 
		AEVec2{ buttonTexture.Get_Image_Width(), buttonTexture.Get_Image_Height() });
	lvl1Btn->SetTexture(&buttonTexture); 
	lvl1Btn->SetActive(true);
	lvl1Btn->SetInteractable(true);

	uiManager.Add(lvl1Btn.get());
}

void LevelMenu_Update() {
	//PRINT("Update: %s", "Main Menu\n");

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(Colors::PastelPurple.r, Colors::PastelPurple.g, Colors::PastelPurple.b);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetTransparency(1.f);

	uiManager.Update(0.f);
}

void LevelMenu_Draw() {
	//PRINT("Draw: %s", "Level Menu\n");

	textToUse.Draw_Text(0, 200.f);
	uiManager.Draw();
}

void LevelMenu_Free() {
	PRINT("Free: %s", "Level Menu\n");
	textToUse.Free_Font();
	uiManager.Exit();
}

void LevelMenu_Unload() {
	PRINT("Unload: %s", "Level Menu\n");

	for (Texture& t : textures) {
		t.Unload_Image();
	}
}
