#pragma once
#ifndef _Game_State_Header_
#define _Game_State_Header_

class GameState {

public:
	virtual void EnterState() = 0;
	virtual void StateUpdate() = 0;
	virtual void ExitState() = 0;
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

	void EnterState() {
		std::cout << "Entered: " << p_msg << std::endl;
	}

	void StateUpdate() {
		PRINT("Update: ");
		PRINT(p_msg);
		PRINT("\n");
	}

	void ExitState() {
		std::cout << "Exit: " << p_msg << std::endl;
	}
};
