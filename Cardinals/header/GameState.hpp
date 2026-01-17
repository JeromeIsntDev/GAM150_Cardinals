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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
=======

>>>>>>> Stashed changes
class TestState : public GameState {
private:
	const char* p_msg;
public:
	TestState(const char* msg) {
		PRINT(msg);
		p_msg = msg;
	}

	void EnterState() {
<<<<<<< Updated upstream
		std::cout << "Entered: " << p_msg << std::endl;
=======
>>>>>>> Stashed changes
		PRINT("Entered: %s\n", p_msg);
	}

	void StateUpdate() {
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		PRINT("Update: ");
		PRINT(p_msg);
		PRINT("\n");
=======
		PRINT("Update: %s\n", p_msg);
>>>>>>> Stashed changes
	}

	void ExitState() {
		std::cout << "Exit: " << p_msg << std::endl;
=======
		PRINT("Update: %s\n", p_msg);
	}

	void ExitState() {
>>>>>>> Stashed changes
		PRINT("Exit: %s\n", p_msg);
	}
};
