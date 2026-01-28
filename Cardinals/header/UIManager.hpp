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
#include <functional>

#include "../header/GameObject.hpp"
#include "../header/InputHandler.hpp"

// this guy IS every ui element
class UIElement : public GameObject 
{
protected:
	bool isActive = false;
	bool isInteractable = false;
	Color tint = Colors::White; // full brightness
	std::vector<UIElement*> children;

public:

	// inheriting gameobject constructors
	using GameObject::GameObject;
	virtual ~UIElement() = default; // destructor

	UIElement(AEGfxVertexList* mesh, AEVec2 pos, float rot, AEVec2 scale) : GameObject(mesh, pos, rot, scale) {}

	virtual void SetActive(bool active) { isActive = active; }

	// doesnt work
	void SetInteractable(bool interactable) 
	{ 
		isInteractable = interactable;
		if (!isInteractable)
		{
			tint = { 0.f, 0.f, 0.f, 1.0f };
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
	Color GetTint() const { return tint; }

	virtual void Update(float dt) {}
	virtual void OnClick() {}
};

// this guy just handles all the diff elements
// kinda stores everything and thats it
class UIManager {

private:
	// dynamic array to store different elements
	std::vector<UIElement*> uiElements;

public:

	virtual ~UIManager() = default; // destructor!! IMPORTANT!!!

	static AEVec2 GetNewMousePos();

	void Add(UIElement* element); // just adds it so it Exists
	void Update(float dt); // loops thru all elements and calls their updaate
	void Draw() const; 

	void Freeze();
	void HandleClick();
	void Exit(); // for the VECTORS
};

///
/// elements heeere
/// 

class Button : public UIElement
{
private:
	std::function<void()> onClick;

public:
	using UIElement::UIElement;
	Button() = default;

	void SetOnClick(std::function<void()> func) { onClick = func; }
	void Update(float dt) override;

	void OnClick() override
	{
		if (!isActive || !isInteractable)
			return;

		if (onClick) onClick();
	}
};

class PopUp : public UIElement
{
public:
	using UIElement::UIElement;

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