#pragma once
#ifndef _Game_State_Header_
#define _Game_State_Header_

class GameState {
public:
	virtual void LoadState() = 0;
	virtual void InitializeState() = 0;
	virtual void UpdateState() = 0;
	virtual void StateDraw() = 0;
	virtual void FreeState() = 0;
	virtual void UnloadState() = 0;
};

#endif // !_Game_State_Header_
