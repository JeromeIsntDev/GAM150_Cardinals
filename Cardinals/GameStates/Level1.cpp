#include "../GameStates/Level1.hpp"
#include <algorithm>
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
	GameObject platform;
	GameObject wall;
	GameObjectIDAssigner assigner;

	InputHandler& input = InputHandler::GetInstance();
	std::vector<GameObject*> objects;
	std::vector<GameObject> objData;
	//IMGUI
	//ControlPanel controlPanel;

	bool isReadFromFile = false;

	enum class GravityDirection {
		Up, Down, Left, Right,
		GravNum
	};

	GravityDirection grav;
}

void Level1_Load() {
	PRINT("Load: %s", "Level 1\n");
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);

	playerTexture.Load_Image(Images::Player);
	placeholderTexture.Load_Image(Images::Placeholder);

	if (!textures.empty()) textures.clear();
	textures.push_back(&playerTexture);
	textures.push_back(&placeholderTexture);

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
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false, 0.f, 0.f,squareMesh, AEGfxGetWinMinX() + 200.f, 150.f);
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

	wall.SetMesh(squareMesh);
	wall.SetTexture(&placeholderTexture);
	wall.SetID(assigner.GetID());
	wall.UpdateScale(AEVec2{ 100.f,500.f });
	wall.UpdateRotation(0.f);
	wall.UpdatePosition(AEVec2{ 500.f, -200.f });

	objects.clear();
	objects.push_back(&mainPlayer);
	objects.push_back(&platform);
	objects.push_back(&wall);


	if (!isReadFromFile) return;
	//Read From JSON
	objData = ReadJsonToObjects("../../Saves/Level1_GameObjects.json");

	for (int i{}; i < objData.size(); ++i) {
		for (int j{}; j < objects.size(); ++j) {
			if (objData[i].GetID() == objects[j]->GetID()) {
				GameObject* curr = objects[j];
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
	
	AEVec2 gravityVector{};
	switch (grav)
	{
	case GravityDirection::Up:
		AEVec2Set(&gravityVector, 0, 1);
		break;
	case GravityDirection::Down:
		AEVec2Set(&gravityVector, 0, -1);
		break;
	case GravityDirection::Left:
		AEVec2Set(&gravityVector, -1, 0);
		break;
	case GravityDirection::Right:
		AEVec2Set(&gravityVector, 1, 0);
		break;
	default:
		break;
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.95f, 0.65f, 0.77f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
	//controlPanel.ControlPanel_Update();

	

	if (input.isMouseBtnPressed(AEVK_LCTRL)) {
		if (input.isMouseBtnTriggered(AEVK_S)) {
			//Save to JSON
			objData.clear();
			objData.push_back(mainPlayer);
			objData.push_back(platform);
			objData.push_back(wall);
			SaveObjectsToJson(objData, "../../Saves/Level1_GameObjects.json");
		}
		if (input.isMouseBtnTriggered(AEVK_M)) {
			isReadFromFile = !isReadFromFile;
		}
		if (input.isMouseBtnTriggered(AEVK_R)) {
			UpdateGameManagerState(Level1);
		}
	}

	if (input.isMouseBtnPressed(AEVK_LBUTTON)) {
		AEVec2 mousePos{0.f,0.f};
		input.getMousePos(mousePos.x, mousePos.y);
		mousePos.x += AEGfxGetWinMaxX() - AEGfxGetWindowWidth();
		mousePos.y += AEGfxGetWinMaxY() - AEGfxGetWindowHeight();
		for (GameObject* obj : objects) {
			AEVec2 currentScale = obj->GetScale();
			AEVec2 currentPos = obj->GetPosition();

			float halfW = currentScale.x / 2.0f, halfH = currentScale.y / 2.0f;

			if (mousePos.x >= currentPos.x - halfW && mousePos.x <= currentPos.x + halfW &&
				-mousePos.y >= currentPos.y - halfH && -mousePos.y <= currentPos.y + halfH) {
				PRINT("Object touched: %f, %f", currentPos.x, currentPos.y);

				int mx, my;
				AEInputGetCursorPositionDelta(&mx, &my);
				obj->UpdatePosition(AEVec2{ currentPos.x + mx, currentPos.y - my });

				AEInputMouseWheelDelta(&mx);
				if (input.isMouseBtnPressed(AEVK_LSHIFT)) {
					mx*=10;
				}
				my = mx;
				obj->UpdateScale(AEVec2{ currentScale.x + mx, currentScale.y + my });
			}
		}
	}


	/*PLAYER STUFF*/
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
	AEVec2 prevPos = mainPlayer.GetPosition();
	mainPlayer.PlayerMovement(dt);

	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.canMoveLeft = true;
	mainPlayer.canMoveRight = true;

	mainPlayer.PlayerCollision(objects);
}

void Level1_Draw() {
	//PRINT("Draw: %s", "Level 1\n");

	platform.Draw();

	wall.Draw();

	mainPlayer.Draw();

	if (isReadFromFile) {
		digipenFont.Input_Text("Reading");
		digipenFont.Draw_Text(AEVec2{ 650.f, 430.f }, .5f, 1.f);
	}

	digipenFont.Input_Text("Level 1");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);
}

void Level1_Free() {
	PRINT("Free: %s", "Level 1\n");

	//digipenFont.Free_Font();
}

void Level1_Unload() {
	PRINT("Unload: %s", "Level 1\n");

	if (MassUnload()) PRINT("Unload Success!");
}
