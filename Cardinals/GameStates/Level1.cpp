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
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false, 0.f, 0.f,squareMesh, 0.f, 150.f);
	mainPlayer.SetTexture(&playerTexture);
	mainPlayer.UpdateScale(AEVec2{ 100.f, 100.f });
	mainPlayer.UpdateRotation(0.f);
	mainPlayer.SetID(assigner.GetID());

	platform.SetMesh(squareMesh);
	platform.SetTexture(&placeholderTexture);
	platform.SetID(assigner.GetID());
	platform.UpdateScale(AEVec2{ 500.f,100.f });
	platform.UpdateRotation(0.f);
	platform.UpdatePosition(AEVec2{ 0.f, -200.f });

	wall.SetMesh(squareMesh);
	wall.SetTexture(&placeholderTexture);
	wall.SetID(assigner.GetID());
	wall.UpdateScale(AEVec2{ 100.f,500.f });
	wall.UpdateRotation(0.f);
	wall.UpdatePosition(AEVec2{ 500.f, -200.f });

	objects.push_back(&wall);
	objects.push_back(&platform);
	objects.push_back(&mainPlayer);
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
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
	AEVec2 prevPos = mainPlayer.GetPosition();
	mainPlayer.PlayerMovement(dt);

	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.left = true;
	mainPlayer.right = true;

	for (GameObject* obj : objects) {
		if (obj == &mainPlayer)
			continue;

		AEVec2 currentScale = obj->GetScale();
		AEVec2 currentPos = obj->GetPosition();

		if (!CollisionDetected(mainPlayer.GetCollider(), obj->GetCollider())) {
			continue;
		}

		mainPlayer.hasCollided = true;

		f32 overlapX =
			(std::min)(mainPlayer.GetCollider().p_maxPoint.x, obj->GetCollider().p_maxPoint.x) -
			(std::max)(mainPlayer.GetCollider().p_minPoint.x, obj->GetCollider().p_minPoint.x);

		f32 overlapY =
			(std::min)(mainPlayer.GetCollider().p_maxPoint.y, obj->GetCollider().p_maxPoint.y) -
			(std::max)(mainPlayer.GetCollider().p_minPoint.y, obj->GetCollider().p_minPoint.y);

		if (overlapX <= 0.f || overlapY <= 0.f)
			continue;

		// Resolve on smallest penetration axis
		if (overlapY <= overlapX + 0.5f)
		{
			AEVec2 tmpPos = mainPlayer.GetPosition();

			//if player is above collidable
			if (mainPlayer.GetPosition().y > obj->GetPosition().y)
			{
				tmpPos.y += overlapY;
				mainPlayer.UpdatePosition(tmpPos);

				mainPlayer.p_velocityVector.y = 0.f;
				mainPlayer.p_isGrounded = true;
				mainPlayer.p_isJumping = false;
			}
			//if player is under collidable
			else
			{
				tmpPos.y -= overlapY;
				mainPlayer.UpdatePosition(tmpPos);

				if (mainPlayer.p_velocityVector.y > 0.f)
					mainPlayer.p_velocityVector.y = 0.f;
			}
		}
		else
		{
			//temp side collision
			AEVec2 tmpPos = mainPlayer.GetPosition();

			if (mainPlayer.GetCollider().p_maxPoint.x > obj->GetCollider().p_minPoint.x &&
				mainPlayer.GetCollider().p_minPoint.x < obj->GetCollider().p_minPoint.x)
			{
				// Player hit object's LEFT side
				tmpPos.x -= overlapX;
				mainPlayer.right = false;
			}
			else
			{
				// Player hit object's RIGHT side
				std::cout << "right hit" << std::endl;
				tmpPos.x += overlapX;
				mainPlayer.left = false;
			}
			mainPlayer.UpdatePosition(tmpPos);
			mainPlayer.p_velocityVector.x = 0.f;
		}
	}
}

void Level1_Draw() {
	PRINT("Draw: %s", "Level 1\n");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);

	mainPlayer.Draw();

	platform.Draw();

	wall.Draw();
}

void Level1_Free() {
	PRINT("Free: %s", "Level 1\n");

	//digipenFont.Free_Font();
}

void Level1_Unload() {
	PRINT("Unload: %s", "Level 1\n");
	
	if (MassUnload()) PRINT("Unload Success!");
}
