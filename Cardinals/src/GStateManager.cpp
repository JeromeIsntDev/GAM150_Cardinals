#include "../header/GStateManager.hpp"

GameStateManager::GameStateManager()
	: pt_CurrentGameState(nullptr)
{
	
}

void GameStateManager::SetCurrentGameState(GameState* state)
{
	if (pt_CurrentGameState != nullptr) {
		pt_CurrentGameState->FreeState();
		pt_CurrentGameState->UnloadState();
	}
	delete pt_CurrentGameState;
	pt_CurrentGameState = state;
	pt_CurrentGameState->LoadState();
	pt_CurrentGameState->InitializeState();
}

GameState* GameStateManager::GetCurrentGameState()
{
	return pt_CurrentGameState;
}

void GameStateManager::LoadCurrentState()
{
	pt_CurrentGameState->LoadState();
}

void GameStateManager::InitCurrentState()
{
	pt_CurrentGameState->InitializeState();
}

void GameStateManager::UpdateCurrentState()
{
	if (pt_CurrentGameState == nullptr) PRINT("NO STATE!!!");
	pt_CurrentGameState->UpdateState();
}

void GameStateManager::DrawCurrentState()
{
	pt_CurrentGameState->StateDraw();
}

void GameStateManager::FreeCurrentState()
{
	pt_CurrentGameState->FreeState();
}

void GameStateManager::UnloadCurrentState()
{
	pt_CurrentGameState->UnloadState();
}
