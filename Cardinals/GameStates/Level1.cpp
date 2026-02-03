#include "Level1.hpp"
#include "GameEditor.hpp"

extern std::vector<Texture*> textures;
extern std::vector<AEGfxVertexList**> meshes;
extern std::vector<Text*> texts;
extern bool MassUnload();

namespace {
	AEGfxVertexList* squareMesh;
	Text digipenFont;

	//PLAYER VARIABLES
	f32 playerHealth{ 150.f }, playerMovespeed{ 300.f };
	Player mainPlayer;
	Texture playerTexture{};

	//LEVEL VARIABLES
	Texture placeholderTexture;
	Texture backgrounds[4]{};
	GameObject backgroundObjs[4]{};
	GameObject platform;
	GameObject wall0;
	GameObject wall1;
	GameObject wall2;
	GameObject wall3;
	GameObjectIDAssigner assigner;
	std::vector<GameObject*> levelObjList;
	std::vector<GameObject> objData;
	//IMGUI
	//ControlPanel controlPanel;

	f32 levelTime{};

	//UIManager tempUI;
	InputHandler& input = InputHandler::GetInstance();
}

void Level1_Load() {
	PRINT("Load: %s", "Level 1\n");
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);

	//tempUI.InitTooltipFont(Fonts::Main, 30.f);

	playerTexture.Load_Image(Images::Player);
	placeholderTexture.Load_Image(Images::Placeholder);
	backgrounds[0].Load_Image(Images::BG_BACKMOST);
	backgrounds[1].Load_Image(Images::BG_CLOUDS);
	backgrounds[2].Load_Image(Images::BG_MOUNTAIN);
	backgrounds[3].Load_Image(Images::BG_FRONTMOUNTAIN);

	if (!textures.empty()) textures.clear();
	textures.push_back(&playerTexture);
	textures.push_back(&placeholderTexture);
	for (Texture& t : backgrounds) {
		textures.push_back(&t);
	}

	if (!meshes.empty()) meshes.clear();
	meshes.push_back(&squareMesh);

	if (!texts.empty()) texts.clear();
	texts.push_back(&digipenFont);
}

void Level1_Initialize()
{
	PRINT("Initialize: %s", "Level 1\n");

	//controlPanel.ControlPanel_Initialize();
	squareMesh = RectMeshCenter();
	assigner.currIdCount = 0;

	//Player Init
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false, 0.f, 0.f, squareMesh, AEGfxGetWinMinX() + 200.f, 75.f);
	mainPlayer.SetTexture(&playerTexture);
	mainPlayer.UpdateScale(AEVec2{ 100.f, 100.f });
	mainPlayer.UpdateRotation(0.f);
	mainPlayer.SetID(assigner.GetID());
	mainPlayer._tag = GameObjectTag::Player;

	platform.SetMesh(squareMesh);
	platform.SetTexture(&placeholderTexture);
	platform.SetID(assigner.GetID());
	platform.UpdateScale(AEVec2{ 500.f,900.f });
	platform.UpdateRotation(0.f);
	platform.UpdatePosition(AEVec2{ AEGfxGetWinMinX() + 200.f, AEGfxGetWinMinY()});

	wall0.SetMesh(squareMesh);
	wall0.SetTexture(&placeholderTexture);
	wall0.SetID(assigner.GetID());
	wall0.UpdateScale(AEVec2{ 100.f,500.f });
	wall0.UpdateRotation(0.f);
	wall0.UpdatePosition(AEVec2{ 500.f, -200.f });

	wall1.SetMesh(squareMesh);
	wall1.SetTexture(&placeholderTexture);
	wall1.SetID(assigner.GetID());
	wall1.UpdateScale(AEVec2{ 100.f,500.f });
	wall1.UpdateRotation(0.f);
	wall1.UpdatePosition(AEVec2{ 200.f, -200.f });

	wall2.SetMesh(squareMesh);
	wall2.SetTexture(&placeholderTexture);
	wall2.SetID(assigner.GetID());
	wall2.UpdateScale(AEVec2{ 500.f,100.f });
	wall2.UpdateRotation(0.f);
	wall2.UpdatePosition(AEVec2{ 0.f, -200.f });

	wall3.SetMesh(squareMesh);
	wall3.SetTexture(&placeholderTexture);
	wall3.SetID(assigner.GetID());
	wall3.UpdateScale(AEVec2{ 500.f,100.f });
	wall3.UpdateRotation(0.f);
	wall3.UpdatePosition(AEVec2{ 500.f, 200.f });

	for (int i{}; i < 4; ++i) {
		GameObject* obj = &backgroundObjs[i];
		obj->SetMesh(squareMesh);
		obj->SetTexture(&backgrounds[i]);
		obj->UpdateScale(AEVec2{ 1700.f, 960.f });
		obj->UpdateRotation(0.f);
		obj->UpdatePosition(AEVec2{ 0.f, 0.f });
	}

	levelObjList.clear();
	levelObjList.push_back(&mainPlayer);
	levelObjList.push_back(&platform);
	levelObjList.push_back(&wall0);
	levelObjList.push_back(&wall1);
	levelObjList.push_back(&wall2);
	levelObjList.push_back(&wall3);


	levelTime = 0.f;

	if (!IsUsingSaveFile()) return;
	//Read From JSON
	objData = ReadJsonToObjects("../../Saves/Level1_GameObjects.json");

	for (int i{}; i < objData.size(); ++i) {
		for (int j{}; j < levelObjList.size(); ++j) {
			if (objData[i].GetID() == levelObjList[j]->GetID()) {
				GameObject* curr = levelObjList[j];
				curr->UpdatePosition(objData[i].GetPosition());
				curr->UpdateRotation(objData[i].GetRotatation());
				curr->UpdateScale(objData[i].GetScale());
			}
		}
	}
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
	levelTime += AEFrameRateControllerGetFrameTime();

	if (input.isMouseBtnPressed(AEVK_LCTRL)) {
		if (input.isMouseBtnTriggered(AEVK_E)) ToggleEditorMode();
		if (IsEditorActive()) {

			DragObject(levelObjList);

			if (input.isMouseBtnTriggered(AEVK_R))
				RestartLevel(UpdateGameManagerState, Level1);

			if (input.isMouseBtnTriggered(AEVK_S))
				SaveGameObjectToFilepath("../../Saves/Level1_GameObjects.json"
					, levelObjList);

			if (input.isMouseBtnTriggered(AEVK_Q))
				ToggleSaveFileMode();
		}
	}



	for (GameObject& obj : backgroundObjs) {
		f32 offset = mainPlayer.GetPosition().x;
		f32 offsetFirst = offset / 200.f;
		f32 offsetSecond = offset / 75.f;
		f32 offsetThird = offset / 25.f;

		backgroundObjs[1].UpdatePosition(AEVec2{ -offsetFirst, backgroundObjs[1].GetPosition().y });
		backgroundObjs[2].UpdatePosition(AEVec2{ -offsetSecond, backgroundObjs[1].GetPosition().y });
		backgroundObjs[3].UpdatePosition(AEVec2{ -offsetThird, backgroundObjs[1].GetPosition().y });
		obj.Draw();
	}

	/*PLAYER STUFF*/

	if (input.isMouseBtnTriggered(AEVK_DOWN)) {
		mainPlayer.grav = Player::GravityDirection::Down;
	}
	if (input.isMouseBtnTriggered(AEVK_UP)) {
		mainPlayer.grav = Player::GravityDirection::Up;
	}
	if (input.isMouseBtnTriggered(AEVK_LEFT)) {
		mainPlayer.grav = Player::GravityDirection::Left;
	}
	if (input.isMouseBtnTriggered(AEVK_RIGHT)) {
		mainPlayer.grav = Player::GravityDirection::Right;
	}

	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	mainPlayer.PlayerMovement(dt, mainPlayer.grav);

	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.canMoveLeft = true;
	mainPlayer.canMoveRight = true;

	mainPlayer.PlayerCollision(levelObjList, mainPlayer.grav);
}

void Level1_Draw() {
	//PRINT("Draw: %s", "Level 1\n");

	platform.Draw();

	wall0.Draw();
	wall1.Draw();
	wall2.Draw();
	wall3.Draw();

	mainPlayer.Draw();

	if (IsEditorActive()) {
		digipenFont.Input_Text("Editor Active");
		digipenFont.Draw_Text(AEVec2{ -650.f, 430.f }, .5f, 1.f);
		if (IsUsingSaveFile()) {
			digipenFont.Input_Text("Using Save File");
			digipenFont.Draw_Text(AEVec2{ -640.f, 380.f }, .5f, 1.f);
		}
	}

	digipenFont.Input_Text("Time: ");
	digipenFont.Draw_Text(675.f, 400.f);
	digipenFont.Input_Float(levelTime);
	digipenFont.Draw_Text_Corner(725.f, 387.f);

	digipenFont.Input_Text("Level 1");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);
}

void Level1_Free() {
	PRINT("Free: %s", "Level 1\n");
}

void Level1_Unload() {
	PRINT("Unload: %s", "Level 1\n");

	if (MassUnload()) PRINT("Unload Success!");
}
