#include "../GameStates/Credits.hpp"


namespace {
	AEGfxVertexList* squareMesh;

	Text titleFont;
	Text nameFont;
	f32 startY;
	f32 endY = 1300.f;
	f32 winHeight = 900.f;
	f32 speed = 3.f;
	f32 fading;

	TextureManager& tex = TextureManager::GetInstance();
	InputHandler& input = InputHandler::GetInstance();

	GameObject logo;

	f32 quit_dt;
	bool quitCredits = false;
}

void Credits_Load() {
	titleFont.Load_Font(Fonts::Main, 25.f);
	nameFont.Load_Font(Fonts::Main, 50.f);

	//for every additional name \n\n one \n in name is \n\n in title 
	titleFont.Input_Text("By Orbital Candy\n\n\n\n\n TECHNICAL LEAD\n\n\n\nGAMEPLAY PROGRAMMER\n\n\n\n UI & ART LEAD\n\n\n\n SYSTEM PROGRAMMER\n\n\n\n\n\n\n\nFACULTY and ADVISORS\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPRESIDENT\n\n\n\nEXECUTIVES\n\n\n\n\n\n\n\n\n\nWWW.DIGIPEN.EDU\nAll content (C) 2026 Digipen Institute of Technology Singapore. All Rights Reserved");
	nameFont.Input_Text("Cardinals\n\n\n\nJerome Lim\n\nSadric Koh\n\nBreanne Chua\n\nShirley Liau\nZoey Tan\n\n\nGerald Wong\nSoroor Malekmohammadi Faradounbeh\nTommy Tan\n\n\n\nCreated at\n\n\n\n\n\n\n\nClaude Comair\n\nChu Jason Yeu Tat   Michael Gats\nTan Chek Ming   Prasanna Kumar Ghali\nMandy Wong   Johnny Deek");

	squareMesh = RectMeshCenter();

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddMesh(&squareMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&titleFont);
	memoryManager.AddFont(&nameFont);

	PRINT("Load: %s", "Credits\n");
}

void Credits_Initialize()
{
	startY = -430.f;
	logo.SetMesh(squareMesh);
	logo.SetTexture(tex.Get("DigipenLogo"));
	logo.UpdateScale(AEVec2{ 685.f, 200.f });
	quitCredits = false;
	fading = 1.f;
	quit_dt = 2.f;
	titleFont.Change_Text_Color(Colors::PastelYellow);
	PRINT("Initialize: %s", "Credits\n");
}

void Credits_Update() {
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.95f, 0.65f, 0.77f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(fading);

	startY += startY > endY ? 0.f : speed;
	logo.UpdatePosition(AEVec2{ 0.f, startY - 1150.f });

	if (!quitCredits && startY >= endY) {
		quit_dt -= dt;
		fading -= quit_dt <= 1.f ? dt : 0;
	}
	
	if (quitCredits || input.isKeyTriggered(AEVK_Q)){
		UpdateGameManagerState(GameStateEnum::MainMenu);
	}

	if (quit_dt <= 0.f) quitCredits = true;

	PRINT("drawat: %.2f\n", quit_dt);
}

void Credits_Draw() {
	titleFont.Draw_Text(AEVec2{ 0, startY }, 1.f, fading);
	nameFont.Draw_Text(AEVec2{ 0, startY + 37.5f }, 1.f, fading);

	if (startY >= endY - winHeight) {
		logo.Draw();
	}
}

void Credits_Free() {
	PRINT("Free: %s", "Credits\n");
	if(quitCredits) quitCredits = false;
}

void Credits_Unload() {
	if (memoryManager.MassUnload()) PRINT("Unload Success");
	PRINT("Unload: %s", "Credits\n");
}
