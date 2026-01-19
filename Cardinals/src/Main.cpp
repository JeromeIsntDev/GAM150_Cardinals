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
#include "../header/GStateManager.hpp"
#include "../header/PlayerController.hpp"
#include "../header/TextureManager.hpp"
#include "../header/AudioManager.hpp"


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

	AudioFile musicFile{ "../../Assets/bouken.mp3" };

	AEAudioGroup _group = AEAudioCreateGroup();

	Audio audioTest1;
	audioTest1.Load_Music(musicFile);
	audioTest1.Play_Audio(1.f, 1.f, 0.f);

	/*AEAudio Audio::Load_Sound(AudioFile audio_file)
	{
		AEAudio audio = AEAudioLoadSound(audio_file.filename);
		return audio;
	}*/

	// Changing the window title
	AESysSetWindowTitle("CARDINAL");

	AEGfxVertexList* squareMesh = RectMeshCenter();
	Texture digipenTexture{};
	digipenTexture.Load_Image(Images::DigipenLogo);
	Font digipenFont{};
	digipenFont.Load_Font(Fonts::Main, 72.f);
	f32 logo_transparency = 0.f;

	//declare mainPlayer variable
	f32 playerHealth{ 150.f }, playerMovespeed{ 100.f }, playerJumpForce{ 100.f };
	bool playerCanJump{ true };

	Player mainPlayer(playerHealth, playerMovespeed, playerJumpForce, playerCanJump, squareMesh);
	mainPlayer.SetTexture(digipenTexture);

	const char* pText = "Cardinal";

	TestState testState1("Test State 1\n");
	TestState testState2("Test State 2\n");
	//TO BE DELETED. TEST PLAYER GET AND SET
	//mainPlayer.UpdatePosition(_initP_pos);
	//std::cout << "Player X: " << mainPlayer.GetPosition().x << " Y: " << mainPlayer.GetPosition().y << std::endl;

	GameStateManager manager(&testState1);

	Player player(100.f, 30.f, 100.f, true, squareMesh);
	player.SetTexture(digipenTexture);
	std::cout << player;

	AEVec2 movementVector{ 0.f,0.f };
	AEVec2 _initP_pos{ 0.f,0.f };
	f32 moveSpeed{ 300.f };
	mainPlayer.UpdatePosition(_initP_pos);
	while (gGameRunning)
	{
		AESysFrameStart();



		if (AEInputCheckTriggered(AEVK_Q)) {
			manager.SetCurrentGameState(&testState2);
		}
		manager.UpdateCurrentState();

		f32 dt = AEFrameRateControllerGetFrameTime();

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetTransparency(1.f);

		if (AEInputCheckCurr(AEVK_D)) {
			movementVector.x = 1.f * moveSpeed * dt;
			_initP_pos.x += movementVector.x;
		}
		if (AEInputCheckCurr(AEVK_A)) {
			movementVector.x = -1.f * moveSpeed * dt;
			_initP_pos.x += movementVector.x;
		}
		mainPlayer.UpdatePosition(_initP_pos);

		
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


		//digipenFont.Draw_Text(pText, 0.f, 0.f, 1.f, Colors::PastelPurple);

		AESysFrameEnd();
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	AEGfxMeshFree(squareMesh);
	digipenTexture.Free_Image();
	digipenFont.Free_Font();
	// free the system
	AESysExit();

}
