#include "../header/GStateManager.hpp"

GameStateManager::GameStateManager(GameState* state)
	: pt_CurrentGameState(state)
{
	
}

void GameStateManager::SetCurrentGameState(GameState* state)
{
	pt_CurrentGameState->UnloadState();
	pt_CurrentGameState = state;
	pt_CurrentGameState->LoadState();
}

GameState* GameStateManager::GetCurrentGameState()
{
	return pt_CurrentGameState;
}

void GameStateManager::EnterCurrState()
{
	pt_CurrentGameState->LoadState();
}

void GameStateManager::CurrStateUpdate()
{
	pt_CurrentGameState->UpdateState();
}

void GameStateManager::ExitCurrState()
{
	pt_CurrentGameState->UnloadState();
}
