/*!***********************************************************************
\file   UIManager.hpp
\author Breanne Chua
\date   16/01/2026
\par    Last Updated by: Breanne Chua
\par    Project: Cardinals

\brief    UIManager is responsible for the UI interfaces.

\par    Changes: None
**************************************************************************/

#pragma once
#ifndef _UI_Manager_Header_
#define _UI_Manager_Header_

#include <AEEngine.h>
#include <vector>
#include "../header/GameObject.hpp"

// this guy controls every ui element there is
class UIElement : public GameObject 
{
protected:
	bool isActive = false;
	bool isInteractable = false;

public:

	// inheriting gameobject constructors
	using GameObject::GameObject;

	virtual ~UIElement() = default; // destructor

	// grabbing shit from gameobj waoowww
	UIElement(AEGfxVertexList* mesh, AEVec2 pos, float rot, AEVec2 scale) : GameObject(mesh, pos, rot, scale) {}

	// working on input and interactivity later
	void SetActive(bool active) { isActive = active; }
	void SetInteractable(bool interactable) { isInteractable = interactable; }

	bool IsActive() const { return isActive; }
	bool IsInteractable() const { return isInteractable; }

	virtual void Update(float dt) override {}
	virtual void OnClick() {}
};

// this guy just handles all the diff elements
// kinda stores everything and thats it
class UIManager {

private:
	// dynamic array to store different elements
	std::vector<UIElement*> uiElements;
	//std::vector<std::unique_ptr<UIElement>> uiElements;

public:

	virtual ~UIManager() = default; // destructor!! IMPORTANT!!!

	void Add(UIElement* element); // just adds it so it Exists
	void Update(float dt); // loops thru all elements and calls their updaate
	void Draw() const; // same shit but just yoinks the draw from gameobj and draws bruh
	void HandleClick(); // useless rn
};

///
///
/// 

#include <functional> // im ngl idk much abt this i desperately just needed to debug

class Button : public UIElement
{
public:

	using UIElement::UIElement;
	Button() = default;

	void SetOnClick(std::function<void()> func) { onClick = func; }

	void OnClick() override
	{
		if (!isActive || !isInteractable)
			return;

		// this does NNNOT work yet gimme a while more to tickle input handling
		if (onClick)
			std::cout << "yeowi";
	}

private:
	std::function<void()> onClick;
};

#endif // !_UI_Manager_Header_