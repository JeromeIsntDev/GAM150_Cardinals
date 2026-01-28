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
#include "../header/InputHandler.hpp"

// this guy IS every ui element
class UIElement : public GameObject 
{
protected:
	bool isActive = false;
	bool isInteractable = false;
	//Color tint = Colors::White; // full brightness
	std::vector<UIElement*> children;

public:

	// inheriting gameobject constructors
	using GameObject::GameObject;

	virtual ~UIElement() = default; // destructor

	// grabbing shit from gameobj waoowww
	UIElement(AEGfxVertexList* mesh, AEVec2 pos, float rot, AEVec2 scale) : GameObject(mesh, pos, rot, scale) {}

	// treat these like unity hehe
	// element->setactive(false) type shit bro
	virtual void SetActive(bool active) { isActive = active; } // Add virtual!

	void SetInteractable(bool interactable) 
	{ 
		isInteractable = interactable;
		if (!isInteractable)
		{
			AEGfxSetColorToMultiply(0.5f, 0.5f, 0.5f, 1.f); // gay out the button lalala
		}
	}

	void AddChild(UIElement* child) // for kiddos, mostly for popup to attach
	{
		children.push_back(child);
		child->SetActive(false);
	}

	bool IsActive() const { return isActive; }
	bool IsInteractable() const { return isInteractable; }
	bool IsPointInside(float x, float y) const;
	bool ToggleActive(u8 key);

	virtual void Update(float dt) {}
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
	void Draw() const; 
	void HandleClick();
	void Disable();
	void Exit(); // for the VECTORS
};

///
/// elements heeere
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

		if (onClick) onClick();
	}

private:
	std::function<void()> onClick;
};

class PopUp : public UIElement
{
public:
	using UIElement::UIElement;

	//PopUp() = default;

	// overrides setactive for the kiddos
	void SetActive(bool active) override
	{
		isActive = active;
		for (auto* child : children) 
		{
			child->SetActive(active);
			child->SetInteractable(active);
		}
	}
};

#endif // !_UI_Manager_Header_