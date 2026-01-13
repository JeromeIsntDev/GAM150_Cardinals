#include "../header/GStateManager.hpp"

GameStateManager::GameStateManager(GameState* state)
	: pt_CurrentGameState(state)
{
	
}

void GameStateManager::SetCurrentGameState(GameState* state)
{
	pt_CurrentGameState->ExitState();
	pt_CurrentGameState = state;
	pt_CurrentGameState->EnterState();
}

GameState* GameStateManager::GetCurrentGameState()
{
	return pt_CurrentGameState;
}

void GameStateManager::EnterCurrState()
{
	pt_CurrentGameState->EnterState();
}

void GameStateManager::CurrStateUpdate()
{
	pt_CurrentGameState->StateUpdate();
}

void GameStateManager::ExitCurrState()
{
	pt_CurrentGameState->ExitState();
}
