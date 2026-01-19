/*!***********************************************************************
\file	Main.cpp
\author	Nil
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	Entry Point for project

\par	Changes: Added temp mainPlayer values - Sadric 14/1/26
**************************************************************************/
#include <crtdbg.h> // To check for memory leaks
#include <AEEngine.h>
#include <iostream>
#include "../header/MeshGenerator.hpp"
#include "../header/TextureManager.hpp"
#include "../header/AudioManager.hpp"
#include "../header/GStateManager.hpp"
#include "../header/PlayerController.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);


	Audio audioTest1;
	audioTest1.CreateAudioGroup();
	audioTest1.Load_Audio(Audios::BGM);
	audioTest1.Play_Audio();

	// Changing the window title
	AESysSetWindowTitle("CARDINAL");

	AEGfxVertexList* squareMesh = RectMeshCenter();
	Texture digipenTexture{};
	digipenTexture.Load_Image(Images::DigipenLogo);
	f32 logo_transparency = 0.f;

	//declare mainPlayer variable
	f32 playerHealth{ 150.f }, playerMovespeed{ 100.f }, playerJumpForce{ 100.f };
	bool playerIsJump{ false };

	Player mainPlayer(playerHealth, playerMovespeed, playerJumpForce, playerIsJump, squareMesh);
	mainPlayer.SetTexture(digipenTexture);
	mainPlayer.UpdateScale(AEVec2{ digipenTexture.Get_Image_Width(), digipenTexture.Get_Image_Height() });
	//TO BE DELETED. TEST PLAYER GET AND SET
	//mainPlayer.UpdatePosition(_initP_pos);

	GameStateManager manager;
	manager.SetCurrentGameState(GameStateManager::GetGameState(GameStates::MainMenu));

	//Player player(100.f, 30.f, 100.f, true, squareMesh);
	//player.SetTexture(digipenTexture);

	//std::cout << player;

	while (gGameRunning)
	{
		AESysFrameStart();

		if (AEInputCheckReleased(AEVK_0)) {
			manager.SetCurrentGameState(GameStateManager::GetGameState(GameStates::MainMenu));
		}
		if (AEInputCheckReleased(AEVK_1)) {
			manager.SetCurrentGameState(GameStateManager::GetGameState(GameStates::Level1));
		}
		if (AEInputCheckReleased(AEVK_2)) {
			manager.SetCurrentGameState(GameStateManager::GetGameState(GameStates::Level2));
		}
		if (AEInputCheckReleased(AEVK_3)) {
			manager.SetCurrentGameState(GameStateManager::GetGameState(GameStates::Level3));
		}
		manager.UpdateCurrentState();
		manager.DrawCurrentState();

		f32 dt = AEFrameRateControllerGetFrameTime();

		mainPlayer.PlayerMovement(dt);
		
		/*if (AEFrameRateControllerGetFrameCount() < 100) {
			logo_transparency += 0.02f;
		}
		else if (AEFrameRateControllerGetFrameCount() > 100 && (AEFrameRateControllerGetFrameCount() < 200)) {
			logo_transparency -= 0.02f;
		}
		else if (AEFrameRateControllerGetFrameCount() > 200){
			logo_transparency = 0.f;
		}*/
		

		// Set the texture to pTex
		//digipenTexture.Set_Image(0.f, 0.f);

		// Tell Alpha Engine to use the matrix in 'transform' to apply onto all
		// the vertices of the mesh that we are about to choose to draw in the next line.
		//AEGfxSetTransform(transform.m);

		// Tell Alpha Engine to draw the mesh with the above settings.
		//AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);

		//digipenTexture.Draw_Text(pFont, pText, 0.f, 0.5f, 1.f, Colors::PastelPink);
		// Informing the system about the loop's end

		mainPlayer.Draw();

		AESysFrameEnd();
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	AEGfxMeshFree(squareMesh);
	digipenTexture.Free_Image();
	// free the system
	AESysExit();

}
