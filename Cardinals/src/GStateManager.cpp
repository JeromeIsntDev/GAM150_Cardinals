#include "../header/GStateManager.hpp"

GameStateManager::GameStateManager(GameState* state)
	: pt_CurrentGameState(state), 
		pt_PreviousGameState(state), pt_NextGameState(state)
{
	
}

void GameStateManager::SetCurrentGameState(GameState* state)
{
	pt_CurrentGameState->FreeState();
	pt_CurrentGameState->UnloadState();
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
