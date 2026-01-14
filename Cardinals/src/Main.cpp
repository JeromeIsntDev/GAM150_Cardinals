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


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	//declare mainPlayer variable

	Player mainPlayer(150.f, 100.f, 100.f, true);

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("Cardinal");

	// Pointer to Mesh. All our sprites use the same mesh, so we can just have one.
	AEGfxVertexList* squareMesh = CircleMesh(32);

	std::cout << squareMesh << std::endl;

	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 400.f, 400.f);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, 0);

	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 0, 0);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	TestState testState1("Test State 1");
	TestState testState2("Test State 2");
	GameStateManager manager(&testState1);

	//stateManager.SetGameState(&testState);
	//manager.SetCurrentGameState(&testState);
	manager.EnterCurrState();
	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		if (AEInputCheckTriggered(AEVK_Q)) {
			manager.SetCurrentGameState(&testState2);
		}
		manager.CurrStateUpdate();

		// Your own rendering logic goes here
		// Set the background to black.
		AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

		// Tell the engine to get ready to draw something with texture.
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// Set the the color to multiply to white, so that the sprite can 
		// display the full range of colors (default is black).
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		// Set the color to add to nothing, so that we don't alter the sprite's color
		AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);

		// Set blend mode to AE_GFX_BM_BLEND
		// This will allow transparency.
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		// Set the texture to pTex

		// Tell Alpha Engine to use the matrix in 'transform' to apply onto all
		// the vertices of the mesh that we are about to choose to draw in the next line.
		AEGfxSetTransform(transform.m);

		// Tell Alpha Engine to draw the mesh with the above settings.
		AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	AEGfxMeshFree(squareMesh);
	// free the system
	AESysExit();

}
