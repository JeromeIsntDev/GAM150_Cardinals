#pragma once
#ifndef _Game_State_Header_
#define _Game_State_Header_

class GameState {
protected:
	const char* _name;
public:
	virtual void LoadState() = 0;
	virtual void InitializeState() = 0;
	virtual void UpdateState() = 0;
	virtual void StateDraw() = 0;
	virtual void FreeState() = 0;
	virtual void UnloadState() = 0;
};

#endif // !_Game_State_Header_

#include <iostream>
#include <AEEngine.h>

class TestState : public GameState {
private:
	const char* p_msg;
public:
	TestState(const char* msg) {
		PRINT(msg);
		p_msg = msg;
	}

	void LoadState() {
		PRINT("Load: %s", p_msg);
	}

	void InitializeState() {
		PRINT("Initialize: %s", p_msg);
	}

	void UpdateState() {
		PRINT("Update: %s", p_msg);
	}

	void StateDraw() {
		PRINT("Draw: %s", p_msg);
	}

	void FreeState() {
		PRINT("Free: %s", p_msg);
	}

	void UnloadState() {
		PRINT("Unload: %s", p_msg);
	}
};
