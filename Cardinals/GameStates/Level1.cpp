/*!***********************************************************************
\file	MainMenu.hpp
\author	Jerome Lim
\date	15/01/2026
\par	Project: Cardinals

\brief	Source File for Level 1 Game State
**************************************************************************/
#include "Level1.hpp"
#include "GameEditor.hpp"


//----------------------FORWARD DECLERATIONS---------------------------//
void Level1_EditorUpdate();
void Level1_EditorDraw();
void Level1_Parallax();
void Tutorial();
//---------------------!FORWARD DECLERATIONS---------------------------//


namespace {
	AEGfxVertexList* squareMesh;
	//AEGfxVertexList* tileMesh;
	Text digipenFont, pauseTitleFont;

	//PLAYER VARIABLES
	f32 playerMovespeed{ 300.f };
	int  playerHealth{ 3 };
	Texture playerTexture[2]{};
	ParticleEffect playerEffect;
	ParticleEffect gravityEffect;

	//LEVEL VARIABLES
	Texture placeholderTexture;
	Texture verticalPlatformTex;
	Texture horizontalPlatformTex;
	Texture backgrounds[4]{};
	GameObject backgroundObjs[4]{};
	GameObject gamelives[3]{};

	// PAUSE MENU
	std::unique_ptr <Overlay> pauseMenu;
	std::unique_ptr<Overlay> gameOverMenu;
	Texture barTexture{}, knobMasterTexture{}, knobMusicTexture{}, 
			knobEFXTexture{}, cardBgTexture{}, prevTexture{}, 
			restartTexture{}, resumeTexture{}, homeTexture{}, 
			nextTexture{}, retryTexture{}, candyTexture{}, 
			greyCandyTexture{};

	//IMGUI
	//ControlPanel controlPanel;

	f32 levelTime{};

	InputHandler& input = InputHandler::GetInstance();
	TextureManager& go_texture = TextureManager::GetInstance();

	Grid grid;
	Player mainPlayer;

	//TUTORIAL VARIABLES
	int checkPoints;
}

void Level1_Load() {
	PRINT("[DEBUG | LEVEL] => Load: %s", "Level 1\n");

	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);

	squareMesh = RectMeshCenter();

	//tempUI.InitTooltipFont(Fonts::Main, 30.f);
	//playerEffectTexture.Load_Image(Images::CandyBall);
	playerTexture[0].Load_Image(Images::Player);
	playerTexture[1].Load_Image(Images::PlayerJump);
	placeholderTexture.Load_Image(Images::Placeholder);
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
	retryTexture.Load_Image(Images::RetryBtn);
	candyTexture.Load_Image(Images::Candy);
	greyCandyTexture.Load_Image(Images::CandyGrey);

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	//textures.push_back(&playerEffectTexture);
	
	memoryManager.AddTextures({
			&placeholderTexture,
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
			&retryTexture,
			&candyTexture,
			&greyCandyTexture
		});

	for (Texture& T : playerTexture) {
		memoryManager.AddTexture(&T);
	}

	for (Texture& t : backgrounds) {
		memoryManager.AddTexture(&t);
	}

	if (!memoryManager.meshes.empty()) memoryManager.meshes.clear();
	memoryManager.AddMesh(&squareMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&digipenFont);
	memoryManager.AddFont(&pauseTitleFont);

	std::vector<GameObject> objData = ReadJsonToObjects("../../Saves/Level1_GameObjects.json");

	// bro
	grid.Load(go_texture.Get("TileSheet"), 900, 1200, 300, 300);
	grid.Initialize();

	for (int i{}; i < objData.size(); ++i) {
		objData[i].SetTexture(&grid.tileTexture);

		AEVec2& pos = objData[i].GetPosition();
		objData[i].UpdatePosition(pos);
		u32 tag = static_cast<u32>(objData[i]._tag);
		grid.WorldToGrid(&pos.x, &pos.y);
		grid.SpawnNewTile(tag, objData[i].GetSlice(), pos.x, pos.y, objData[i].GetRotation());
	}
}

void Level1_Initialize()
{
	PRINT("[DEBUG | LEVEL] => Initialize: %s", "Level 1\n");

	checkPoints = 0;

	playerEffect.Init();
	gravityEffect.Init();

	//Player Init
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false, 
		0.f, 0.f, squareMesh, AEGfxGetWinMinX() + 225.f, -25.f, false);
	mainPlayer.SetMesh(squareMesh);
	mainPlayer.SetTexture(&playerTexture[0]);
	mainPlayer.UpdateScale(AEVec2{50.f,50.f});
	mainPlayer.UpdateRotation(0.f);
	mainPlayer.UpdatePosition(mainPlayer.GetPosition());
	mainPlayer._tag = GameObjectTag::Player;
	mainPlayer.SetHealth(3);

	for (int i{}; i < 4; ++i) {
		GameObject* obj = &backgroundObjs[i];
		obj->SetMesh(squareMesh);
		obj->SetTexture(&backgrounds[i]);
		obj->UpdateScale(AEVec2{ 1700.f, 960.f });
		obj->UpdateRotation(0.f);
		obj->UpdatePosition(AEVec2{ 0.f, 0.f });
	}

	for (int i{}; i < 3; ++i) {
		GameObject* live = &gamelives[i];
		live->SetTexture(&candyTexture);
		live->SetMesh(squareMesh);
		live->UpdateScale(AEVec2{ 70.f, 70.f });
		live->UpdatePosition(AEVec2{ -70.f + i * 70.f, 380.f });
	}
	levelTime = 0.f;

	pauseMenu = std::make_unique<Overlay>(squareMesh, PAUSE, -1, 
		[]() {isGamePaused = 0; }, 
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::MainMenu); }, 
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::Level1); });

	pauseMenu->SetTextures(
		PauseMenuTextures{ 
			&placeholderTexture, &cardBgTexture, &barTexture,
			&knobMasterTexture, &knobMusicTexture, &knobEFXTexture, 
			&prevTexture, &resumeTexture, &restartTexture, 
			&homeTexture, &pauseTitleFont 
		});
	isGamePaused = 0;

	gameOverMenu = std::make_unique<Overlay>(
		squareMesh, 
		LEVEL, 1, 
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level2); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level1); });

	gameOverMenu->SetTextures(
		GameOverTextures{ 
			&placeholderTexture, &cardBgTexture, &candyTexture,
			&greyCandyTexture, &retryTexture, &homeTexture, 
			&nextTexture, &pauseTitleFont, &digipenFont 
		});
	isGameOver = 0;
}

void Level1_Update() {
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	if (pauseMenu) //Checks if pauseMenu Exists
	{
		if (AEInputCheckTriggered(AEVK_P) && !isGameOver)
		{
			isGamePaused = !isGamePaused;
			pauseMenu->SetActive(isGamePaused);
		}

		pauseMenu->Update(dt);

		if (pauseMenu->IsActive() && input.isKeyTriggered(AEVK_LBUTTON))
			pauseMenu->OnClick();
	}
	else {
		PRINT("[WARNING] => Pause Menu Unique Pointer Not Initialized!");
	}


	if (gameOverMenu)
	{
		if (AEInputCheckTriggered(AEVK_O) && !isGameOver)
		{
			isGameOver = true;
			isGamePaused = false;
			gameOverMenu->UpdateGameOverMenu(levelTime, mainPlayer.GetPlayerHealth());

			pauseMenu->SetActive(false);
			gameOverMenu->SetActive(true);
		}

		gameOverMenu->Update(dt);

		if (gameOverMenu->IsActive() && input.isKeyTriggered(AEVK_LBUTTON))
			gameOverMenu->OnClick();
	}
	else {
		PRINT("[WARNING] => Game Over Menu Unique Pointer Not Initialized!");
	}


	if (mainPlayer.getGameOver()) {
		isGameOver = true;
		gameOverMenu->UpdateGameOverMenu(levelTime, mainPlayer.GetPlayerHealth());
		if (gameOverMenu)
			gameOverMenu->SetActive(true);
		mainPlayer.SetGameOver(false);
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.95f, 0.65f, 0.77f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);

	if (!isGamePaused && !isGameOver) {
		levelTime += static_cast<f32>(AEFrameRateControllerGetFrameTime());
	}

	if (!IsEditorActive() || IsEditorActive())
		if (input.isKeyTriggered(AEVK_E))
			ToggleEditorMode();

	Level1_EditorUpdate();

	Level1_Parallax();

	
	for (GameObject* obj : grid.objs) {
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
		gravityEffect.Update(dt, obj->GetPosition(), obj->_tag);
	}

	for (int i{}; i < 3; ++i) {
		GameObject* live = &gamelives[i];
		if (i < mainPlayer.GetPlayerHealth()) {
			live->SetTexture(&candyTexture);
		}
		else {
			live->SetTexture(&greyCandyTexture);
		}
	}

	if (isGamePaused || isGameOver) return;
	/*PLAYER STUFF*/
	mainPlayer.PlayerMovement(dt, mainPlayer.grav);
	if (mainPlayer.GetPlayerJump()) {
		mainPlayer.SetTexture(&playerTexture[1]);
	}
	else {
		mainPlayer.SetTexture(&playerTexture[0]);
	}
	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.canMoveLeft = true;
	mainPlayer.canMoveRight = true;
	mainPlayer.CheckBinaryCollision(&grid,mainPlayer.grav);
	mainPlayer.PlayerCollision(grid.objs);

	playerEffect.Update(dt, mainPlayer.GetPosition());

}

void Level1_Draw() {
	
	AEGfxSetColorToMultiply(.75f, .75f, .75f, 1.0f);
	for (GameObject& obj : backgroundObjs) {
		obj.Draw();
	}
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	for (GameObject* obj : grid.objs) {
		//obj->Draw();
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
		gravityEffect.DrawParticle();
	}
	grid.Draw();
	mainPlayer.Draw();
	playerEffect.DrawParticle();

	digipenFont.Input_Text("Time: ");
	digipenFont.Draw_Text(675.f, 400.f);
	digipenFont.Input_Float(levelTime);
	digipenFont.Draw_Text_Corner(725.f, 387.f);

	digipenFont.Input_Text("Level 1");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);
	
	for (GameObject& live : gamelives) {
		live.Draw();
	}

	if (pauseMenu) pauseMenu->Draw();
	if (gameOverMenu) gameOverMenu->Draw();

	if(!pauseMenu->IsActive() && !gameOverMenu->IsActive())Tutorial();

	Level1_EditorDraw();
}

void Level1_Free() {
	PRINT("[DEBUG | LEVEL] => Free: %s", "Level 1\n");

	playerEffect.Clear();
	gravityEffect.Clear();
}

void Level1_Unload() {
	PRINT("[DEBUG | LEVEL] => Unload: %s", "Level 1\n");

	pauseMenu.reset();
	gameOverMenu.reset();
	if (isGamePaused) isGamePaused = 0;
	if (isGameOver) isGameOver = 0;
	if (mainPlayer.getGameOver()) mainPlayer.SetGameOver(false);

	//tex.UnloadAll();
	if (memoryManager.MassUnload()) PRINT("[MEM DEBUG] => Level 1 Unload Success!\n");
	grid.Clear();
}


void Level1_Parallax() {
	//Reduce Colour -> To Bright
	AEGfxSetColorToMultiply(.75f, .75f, .75f, 1.0f);
	f32 offset = mainPlayer.GetPosition().x;
	f32 offsetFirst = offset / 200.f;
	f32 offsetSecond = offset / 75.f;
	f32 offsetThird = offset / 25.f;

	backgroundObjs[1].UpdatePosition(AEVec2{ -offsetFirst, backgroundObjs[1].GetPosition().y });
	backgroundObjs[2].UpdatePosition(AEVec2{ -offsetSecond, backgroundObjs[1].GetPosition().y });
	backgroundObjs[3].UpdatePosition(AEVec2{ -offsetThird, backgroundObjs[1].GetPosition().y });
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
}

void Level1_EditorUpdate() {
	if (IsEditorActive() && !isGamePaused && !isGameOver) {

		//DragObject(levelObjList);
		ChangeTileTag();
		ChangeTileSlice();

		if (input.isKeyTriggered(AEVK_R))
			RestartLevel(UpdateGameManagerState, Level1);

		if (input.isKeyPressed(AEVK_LCTRL) && input.isKeyTriggered(AEVK_S))
			SaveGameObjectToFilepath("../../Saves/Level1_GameObjects.json", grid.objs);

		if (input.isKeyTriggered(AEVK_Q))
			ToggleSaveFileMode();

		if (InputHandler::GetInstance().isKeyTriggered(AEVK_LBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.SpawnNewTile(EditorTileTag(), EditorTileSlice(), mousePos.x, mousePos.y, 0.f);
		}

		if (InputHandler::GetInstance().isKeyTriggered(AEVK_RBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.DeleteTile(mousePos.x, mousePos.y);
		}
	}
}

void Level1_EditorDraw() {
	if (pauseMenu->IsActive() || gameOverMenu->IsActive()) return;
	if (IsEditorActive()) {
		digipenFont.Input_Text("Editor Active");
		digipenFont.Draw_Text(AEVec2{ -650.f, 415.f }, .5f, 1.f);


		digipenFont.Input_Text("Tile Tag: ");
		digipenFont.Draw_Text(AEVec2{ -100.f, -365.f }, .5f, 1.f);
		switch (EditorTileTag()) {
		case 0:
			digipenFont.Input_Text("Static");
			break;
		case 1:
			digipenFont.Input_Text("Dynamic");
			break;
		case 2:
			digipenFont.Input_Text("Platform");
			break;
		case 3:
			digipenFont.Input_Text("Player");
			break;
		case 4:
			digipenFont.Input_Text("Win");
			break;
		case 5:
			digipenFont.Input_Text("GravUp");
			break;
		case 6:
			digipenFont.Input_Text("GravDown");
			break;
		case 7:
			digipenFont.Input_Text("GravLeft");
			break;
		case 8:
			digipenFont.Input_Text("GravRight");
			break;
		case 9:
			digipenFont.Input_Text("Obstacle");
			break;
		}
		digipenFont.Draw_Text(AEVec2{ 75.f, -365.f }, .5f, 1.f);

		digipenFont.Input_Text("Tile Slice: ");
		digipenFont.Draw_Text(AEVec2{ -100.f, -415.f }, .5f, 1.f);
		switch (EditorTileSlice()) {
		case 0:
			digipenFont.Input_Text("Top-left");
			break;
		case 1:
			digipenFont.Input_Text("Top-Middle");
			break;
		case 2:
			digipenFont.Input_Text("Top-Right");
			break;
		case 3:
			digipenFont.Input_Text("Middle-Left");
			break;
		case 4:
			digipenFont.Input_Text("Middle-Middle");
			break;
		case 5:
			digipenFont.Input_Text("Middle-Right");
			break;
		case 6:
			digipenFont.Input_Text("Bottom-Left");
			break;
		case 7:
			digipenFont.Input_Text("Bottom-Middle");
			break;
		case 8:
			digipenFont.Input_Text("Bottom-Right");
			break;
		case 9:
			digipenFont.Input_Text("9");
			break;
		case 10:
			digipenFont.Input_Text("10");
			break;
		case 11:
			digipenFont.Input_Text("11");
			break;
		}
		digipenFont.Draw_Text_Corner(15.f, -430.f);


		if (IsUsingSaveFile()) {
			digipenFont.Input_Text("Using Save File");
			digipenFont.Draw_Text(AEVec2{ -640.f, 380.f }, .5f, 1.f);
		}
		DrawTileOutline();
		mainPlayer.DebugDraw();
		DrawDebugObjects(grid.objs);
	}
}

void Tutorial() {

	digipenFont.Change_Text_Color(Colors::White);
	if (mainPlayer.GetPosition().x > -200.f) {
		switch (checkPoints) {
		case 0:
			if (mainPlayer.GetPosition().x > -200.f) ++checkPoints;
			break;
		case 1:
			digipenFont.Input_Text("Avoid the CANDY CORN!!!");
			digipenFont.Draw_Text(0.f, 50.f);
			if (mainPlayer.GetPosition().x > 35.f) ++checkPoints;
			break;
		case 2:
			digipenFont.Input_Text("Touch the GRAVITY CHANGERS!");
			digipenFont.Draw_Text(0.f, 50.f);
			if (mainPlayer.GetPosition().y > 100.f) {
				++checkPoints;
			}
			else if (mainPlayer.GetPosition().x > 375.f) {
				checkPoints = 10;
			}
			break;
		case 3:
			digipenFont.Input_Text("You can move along the ceiling!");
			digipenFont.Draw_Text(150.f, 75.f);
			if (mainPlayer.GetPosition().x > 450.f) ++checkPoints;
			break;
		case 4:
			digipenFont.Input_Text("Touch the COTTON PORTAL and go to");
			digipenFont.Draw_Text(350.f, 45.f);
			digipenFont.Input_Text("the next level!");
			digipenFont.Draw_Text(350.f, 15.f);
			break;
		case 10:
			digipenFont.Input_Text("Or don't touch them...");
			digipenFont.Draw_Text(0.f, 50.f);
			if (mainPlayer.GetPosition().y > 100.f) {
				checkPoints = 3;
			}
			else if (mainPlayer.GetPosition().x > 450.f) {
				checkPoints = 4;
			}
			break;
		default:
			checkPoints = 0;
			break;
		}
	}
	else {
		switch (mainPlayer.GetPlayerHealth()) {
		case 3:
			
			digipenFont.Input_Text("Welcome to Cardinals!");
			digipenFont.Draw_Text(-400.f, 175.f);
			digipenFont.Input_Text("Use 'A' and 'D' to move left and right.");
			digipenFont.Draw_Text(-400.f, 145.f);
			digipenFont.Input_Text("Use 'L-Shift' to sprint.");
			digipenFont.Draw_Text(-400.f, 115.f);
			digipenFont.Input_Text("Use 'Space' to jump.");
			digipenFont.Draw_Text(-400.f, 85.f);
			break;
		case 2:
			digipenFont.Input_Text("Oops! Try Again!");
			digipenFont.Draw_Text(-400.f, 100.f);
			break;
		case 1:
			digipenFont.Input_Text("Umm... You can do it! ...");
			digipenFont.Draw_Text(-400.f, 100.f);
			break;
		};
	}
	digipenFont.Change_Text_Color(Colors::White);
}
