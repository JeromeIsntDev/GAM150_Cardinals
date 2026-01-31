#include "../GameStates/Level1.hpp"

extern std::vector<Texture> textures;

namespace {
	AEGfxVertexList* squareMesh;
	Text digipenFont;

	//PLAYER VARIABLES
	f32 playerHealth{ 150.f }, playerMovespeed{ 300.f };
	bool playerIsJump{ false };
	Player mainPlayer;
	Texture playerTexture{};

	//LEVEL VARIABLES
	Texture placeholderTexture;
	GameObject platform;
	GameObjectIDAssigner assigner;


	//IMGUI
	//ControlPanel controlPanel;
}

void Level1_Load() {
	PRINT("Load: %s", "Level 1\n");
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);

	playerTexture.Load_Image(Images::Player);
	placeholderTexture.Load_Image(Images::Placeholder);

	if (!textures.empty()) textures.clear();
	textures.push_back(playerTexture);
	textures.push_back(placeholderTexture);
}

void Level1_Initialize()
{
	PRINT("Initialize: %s", "Level 1\n");
	//controlPanel.ControlPanel_Initialize();
	squareMesh = RectMeshCenter();
	assigner.currIdCount = 0;

	//Player Init
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false, 0.f, 0.f,squareMesh, 0.f, 150.f);
	mainPlayer.SetTexture(&playerTexture);
	mainPlayer.UpdateScale(AEVec2{ 100.f, 100.f });
	mainPlayer.UpdateRotation(0.f);
	mainPlayer.SetID(assigner.GetID());

	platform.SetMesh(squareMesh);
	platform.SetTexture(&placeholderTexture);
	platform.SetID(assigner.GetID());
	platform.UpdateScale(AEVec2{ 100.f,100.f });
	platform.UpdateRotation(0.f);
	platform.UpdatePosition(AEVec2{ 0.f, -200.f });
}

void Level1_Update() {
	//declare mainPlayer variable
	//PRINT("Update: %s", "Main Menu\n");
	
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.95f, 0.65f, 0.77f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
	//controlPanel.ControlPanel_Update();
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	mainPlayer.PlayerMovement(dt);
}

void Level1_Draw() {
	PRINT("Draw: %s", "Level 1\n");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);

	mainPlayer.Draw();

	platform.Draw();
}

void Level1_Free() {
	PRINT("Free: %s", "Level 1\n");

	digipenFont.Free_Font();
	AEGfxMeshFree(squareMesh);
}

void Level1_Unload() {
	PRINT("Unload: %s", "Level 1\n");
	
	for (Texture& t : textures) {
		t.Unload_Image();
	}
}
