#include "Level1.hpp"
#include "GameEditor.hpp"


namespace {
	AEGfxVertexList* squareMesh;
	AEGfxVertexList* tileMesh;
	Text digipenFont, pauseTitleFont;

	//PLAYER VARIABLES
	f32 playerHealth{ 150.f }, playerMovespeed{ 300.f };
	Player mainPlayer;
	Texture playerTexture{};
	ParticleEffect playerEffect;
	ParticleEffect gravityEffect;

	//LEVEL VARIABLES
	Texture placeholderTexture;
	Texture verticalPlatformTex;
	Texture horizontalPlatformTex;
	Texture backgrounds[4]{};
	GameObject backgroundObjs[4]{};
	std::vector<GameObject*> levelObjList;
	std::vector<GameObject> objData;

	// PAUSE MENU
	std::unique_ptr <Overlay> pauseMenu;
	std::unique_ptr<Overlay> gameOverMenu;
	Texture barTexture{}, knobMasterTexture{}, knobMusicTexture{}, knobEFXTexture{}, cardBgTexture{}, prevTexture{}, restartTexture{}, resumeTexture{}, homeTexture{}, nextTexture{}, candyTexture{}, greyCandyTexture;

	//IMGUI
	//ControlPanel controlPanel;

	f32 levelTime{};

	//UIManager tempUI;
	InputHandler& input = InputHandler::GetInstance();

	Grid grid;
}

void Level1_Load() {
	PRINT("Load: %s", "Level 1\n");

	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);

	tileMesh = RectMeshCenterSliced(1,3);
	//tempUI.InitTooltipFont(Fonts::Main, 30.f);
	//playerEffectTexture.Load_Image(Images::CandyBall);
	playerTexture.Load_Image(Images::Player);
	placeholderTexture.Load_Image(Images::Placeholder);
	verticalPlatformTex.Load_Image(Images::Platform_VL);
	horizontalPlatformTex.Load_Image(Images::Platform_HL);
	backgrounds[0].Load_Image(Images::BG_BACKMOST);
	backgrounds[1].Load_Image(Images::BG_CLOUDS);
	backgrounds[2].Load_Image(Images::BG_MOUNTAIN);
	backgrounds[3].Load_Image(Images::BG_FRONTMOUNTAIN); 

	// PAUSE MENU
	pauseTitleFont.Load_Font(Fonts::Alt, 72.f);
	prevTexture.Load_Image(Images::PrevBtn);
	barTexture.Load_Image(Images::Bar);
	knobMasterTexture.Load_Image(Images::KnobMaster);
	knobMusicTexture.Load_Image(Images::KnobMusic);
	knobEFXTexture.Load_Image(Images::KnobEFX);
	cardBgTexture.Load_Image(Images::CardBg);
	restartTexture.Load_Image(Images::RestartBtn);
	resumeTexture.Load_Image(Images::ResumeBtn);
	homeTexture.Load_Image(Images::HomeBtn);
	nextTexture.Load_Image(Images::NextBtn);
	candyTexture.Load_Image(Images::Candy);
	greyCandyTexture.Load_Image(Images::CandyGrey);

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	//textures.push_back(&playerEffectTexture);
	
	memoryManager.AddTextures({
			&playerTexture,
			& placeholderTexture,
			&verticalPlatformTex,
			&horizontalPlatformTex,
			&prevTexture,
			&barTexture,
			&knobMasterTexture,
			&knobMusicTexture,
			&knobEFXTexture,
			&cardBgTexture,
			&restartTexture,
			&resumeTexture,
			&homeTexture,
			&nextTexture,
			&candyTexture,
			&greyCandyTexture
		});

	for (Texture& t : backgrounds) {
		memoryManager.AddTexture(&t);
	}

	if (!memoryManager.meshes.empty()) memoryManager.meshes.clear();
	memoryManager.AddMesh(&squareMesh);
	memoryManager.AddMesh(&tileMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&digipenFont);
	memoryManager.AddFont(&pauseTitleFont);

	squareMesh = RectMeshCenter();

	objData = ReadJsonToObjects("../../Saves/Level1_GameObjects.json");

	levelObjList.clear();

	for (int i{}; i < objData.size(); ++i) {
		objData[i].SetMesh(squareMesh);
		objData[i].SetTexture(&placeholderTexture);
		objData[i].UpdatePosition(objData[i].GetPosition());

		PRINT("[DEBUG] Loaded => ");
		std::cout << objData[i] << '\n';

		levelObjList.push_back(&objData[i]);
	}
}

void Level1_Initialize()
{
	PRINT("Initialize: %s", "Level 1\n");

	//controlPanel.ControlPanel_Initialize();

	playerEffect.Init();
	gravityEffect.Init();

	//Player Init
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false, 
		0.f, 0.f, squareMesh, AEGfxGetWinMinX() + 200.f, 75.f);
	mainPlayer.SetMesh(squareMesh);
	mainPlayer.SetTexture(&playerTexture);
	mainPlayer.UpdateScale(mainPlayer.p_Scale);
	mainPlayer.UpdateRotation(0.f);
	mainPlayer._tag = GameObjectTag::Player;

	for (int i{}; i < 4; ++i) {
		GameObject* obj = &backgroundObjs[i];
		obj->SetMesh(squareMesh);
		obj->SetTexture(&backgrounds[i]);
		obj->UpdateScale(AEVec2{ 1700.f, 960.f });
		obj->UpdateRotation(0.f);
		obj->UpdatePosition(AEVec2{ 0.f, 0.f });
	}


	levelTime = 0.f;

	pauseMenu = std::make_unique<Overlay>(squareMesh, PAUSE, -1, []() {isGamePaused = 0; }, []() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::MainMenu); }, []() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::Level1); });

	pauseMenu->SetTextures(PauseMenuTextures{ &placeholderTexture, &cardBgTexture, &barTexture,
		&knobMasterTexture, &knobMusicTexture, &knobEFXTexture, &prevTexture, &resumeTexture, &restartTexture, &homeTexture, &pauseTitleFont });
	isGamePaused = 0;

	gameOverMenu = std::make_unique<Overlay>(squareMesh, LEVEL, 1, []() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level2); }, []() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); }, []() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level1); });

	gameOverMenu->SetTextures(GameOverTextures{ &placeholderTexture, &cardBgTexture, &candyTexture,
		&greyCandyTexture, &restartTexture, &homeTexture, &nextTexture, &pauseTitleFont, &digipenFont });
	isGameOver = 0;

	//Read From JSON
}

void Level1_Update() {
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	if (AEInputCheckTriggered(AEVK_P) && !isGameOver)
	{
		isGamePaused = !isGamePaused;
		if (pauseMenu)
			pauseMenu->SetActive(isGamePaused);
	}

	if (mainPlayer.IsGameOver() && !isGameOver)
	{
		isGameOver = true;
		isGamePaused = false;
		gameOverMenu->UpdateGameOverMenu(levelTime);

		if (pauseMenu)
			pauseMenu->SetActive(false);

		if (gameOverMenu)
			gameOverMenu->SetActive(true);
	}

	if (pauseMenu)
	{
		pauseMenu->Update(dt);

		if (pauseMenu->IsActive() && input.isMouseBtnTriggered(AEVK_LBUTTON))
			pauseMenu->OnClick();
	}

	if (gameOverMenu)
	{
		gameOverMenu->Update(dt);

		if (gameOverMenu->IsActive() && input.isMouseBtnTriggered(AEVK_LBUTTON))
			gameOverMenu->OnClick();
	}


	//declare mainPlayer variable
	//PRINT("Update: %s", "Main Menu\n");

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.95f, 0.65f, 0.77f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);

	//controlPanel.ControlPanel_Update();
	if (!isGamePaused && !isGameOver) {
		levelTime += static_cast<f32>(AEFrameRateControllerGetFrameTime());
	}
	//levelTime +=static_cast<f32>(AEFrameRateControllerGetFrameTime());

	if (!IsEditorActive() || IsEditorActive())
		if (input.isMouseBtnTriggered(AEVK_E))
			ToggleEditorMode();

	//std::cout << canEdit << std::endl;
	if (IsEditorActive()) {
		
		//DragObject(levelObjList);
		tileTag = (tileTag < 0) ? 0 : tileTag;

		if (input.isKeyTriggered(AEVK_LEFT) && tileTag > 0) {

			--tileTag;
		}
		else if (input.isKeyTriggered(AEVK_RIGHT)) {
			++tileTag;
		}

		if (input.isMouseBtnTriggered(AEVK_R))
			RestartLevel(UpdateGameManagerState, Level1);

		if (input.isMouseBtnTriggered(AEVK_S))
			SaveGameObjectToFilepath("../../Saves/Level1_GameObjects.json", levelObjList);

		if (input.isMouseBtnTriggered(AEVK_Q))
			ToggleSaveFileMode();

		if (InputHandler::GetInstance().isMouseBtnTriggered(AEVK_LBUTTON))
			SpawnNewTile(&grid, levelObjList, tileTag);

		/*if (InputHandler::GetInstance().isMouseBtnTriggered(AEVK_LBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().getMousePos(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;

			GridCell* cell = grid.GetCell(mousePos.y, mousePos.x);

			if (cell->obj != nullptr) {
				std::cout << "Cell Filled Already!" << std::endl;
				return;
			}

			GameObject* obj = new GameObject{};
			obj->SetMesh(squareMesh);
			obj->SetTexture(&placeholderTexture);
			obj->UpdateScale({ 50.f,50.f });
			obj->UpdatePosition({ cell->position.x - 800.f , 450.f - cell->position.y });
			cell->obj = obj;
			levelObjList.push_back(obj);

			std::cout << grid.cells[cell->row][cell->column] << std::endl;
		}*/
	}


	AEGfxSetColorToMultiply(.75f, .75f, .75f, 1.0f);
	/*BACKGROUND PARALLAXING*/
	for (GameObject& obj : backgroundObjs) {
		f32 offset = mainPlayer.GetPosition().x;
		f32 offsetFirst = offset / 200.f;
		f32 offsetSecond = offset / 75.f;
		f32 offsetThird = offset / 25.f;

		backgroundObjs[1].UpdatePosition(AEVec2{ -offsetFirst, backgroundObjs[1].GetPosition().y });
		backgroundObjs[2].UpdatePosition(AEVec2{ -offsetSecond, backgroundObjs[1].GetPosition().y });
		backgroundObjs[3].UpdatePosition(AEVec2{ -offsetThird, backgroundObjs[1].GetPosition().y });
	}
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	if (isGamePaused || isGameOver) return;
	/*PLAYER STUFF*/

	//f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	mainPlayer.PlayerMovement(dt, mainPlayer.grav);

	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.canMoveLeft = true;
	mainPlayer.canMoveRight = true;

	mainPlayer.PlayerCollision(levelObjList, mainPlayer.grav);

	playerEffect.Update(dt, mainPlayer.GetPosition());
	gravityEffect.Update(dt);

}

void Level1_Draw() {
	//PRINT("Draw: %s", "Level 1\n");
	
	AEGfxSetColorToMultiply(.75f, .75f, .75f, 1.0f);
	for (GameObject& obj : backgroundObjs) {
		obj.Draw();
	}
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	for (GameObject* obj : levelObjList) {
		obj->Draw();
	}
	levelObjList[0]->Draw();

	mainPlayer.Draw();
	playerEffect.DrawParticle();

	if (IsEditorActive()) {
		digipenFont.Input_Text("Editor Active");
		digipenFont.Draw_Text(AEVec2{ -650.f, 415.f }, .5f, 1.f);

		
		digipenFont.Input_Text("Tile Active: ");
		digipenFont.Draw_Text(AEVec2{ -100.f, -415.f }, .5f, 1.f);
		digipenFont.Input_Float(static_cast<int>(tileTag));
		digipenFont.Draw_Text(AEVec2{ 10.f, -415.f }, .5f, 1.f);


		if (IsUsingSaveFile()) {
			digipenFont.Input_Text("Using Save File");
			digipenFont.Draw_Text(AEVec2{ -640.f, 380.f }, .5f, 1.f);
		}
		DrawTileOutline();
		DrawDebugObjects(levelObjList);
		mainPlayer.DebugDraw();
	}

	digipenFont.Input_Text("Time: ");
	digipenFont.Draw_Text(675.f, 400.f);
	digipenFont.Input_Float(levelTime);
	digipenFont.Draw_Text_Corner(725.f, 387.f);

	digipenFont.Input_Text("Level 1");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);

	gravityEffect.DrawParticle();

	if (pauseMenu) pauseMenu->Draw();
	if (gameOverMenu) gameOverMenu->Draw();
}

void Level1_Free() {
	PRINT("Free: %s", "Level 1\n");
	playerEffect.Clear();
	gravityEffect.Clear();
}

void Level1_Unload() {
	PRINT("Unload: %s", "Level 1\n");
	pauseMenu.reset();
	gameOverMenu.reset();
	if (isGamePaused) isGamePaused = 0;
	if (isGameOver) isGameOver = 0;
	if (memoryManager.MassUnload()) PRINT("Unload Success!");
}
