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

// this guy IS every ui element
class UIElement : public GameObject
{
protected:
	bool isActive = false;
	bool isInteractable = false;

	Color tint = Colors::White;
	std::vector<UIElement*> children;

	std::string tooltipToDraw = "";
	AEVec2 tooltipPos = { 0,0 };
	bool isToolTipShown = false;

public:

	// inheriting gameobject constructors
	using GameObject::GameObject;
	UIElement(AEGfxVertexList* mesh, AEVec2 pos, float rot, AEVec2 scale) : GameObject(mesh, pos, rot, scale) {}

	virtual void SetActive(bool active);
	void SetInteractable(bool interactable);
	void SetTooltip(std::string text, AEVec2 pos);

	void AddChild(UIElement* child);

	// regular getters
	bool IsActive() const { return isActive; }
	bool IsInteractable() const { return isInteractable; }
	bool IsPointInside(float x, float y) const;
	bool ToggleActive(u8 key);
	Color GetTint() const { return tint; }

	// tool tip getters
	bool IsShowingTooltip() const { return isToolTipShown; }
	std::string GetTooltipText() const { return tooltipToDraw; }
	AEVec2 GetTooltipPos() const { return tooltipPos; }
	void ResetTooltip();

	virtual void Update(float dt) {}
	virtual void OnClick() {}

	virtual ~UIElement() 
	{
		for (auto* child : children)
		{
			delete child;
			children.clear();
		}
	}
};

// this guy just handles all the diff elements
// kinda stores everything and thats it
class UIManager 
{

private:
	// dynamic array to store different elements
	std::vector<UIElement*> uiElements;

	// cuz text rn is const and . i need to tickle it
	// apparently mutable exists to edit const variables anyway
	// i spent too long wondering how to wiggle around this sauuur
	mutable Text tooltipRenderer;
	bool isFontLoaded = false;

public:

	virtual ~UIManager() = default; // destructor!! IMPORTANT!!!
	static AEVec2 GetNewMousePos(); // ssingleton

	void InitTooltipFont(const FontFile& font, float size);

	void Add(UIElement* element); // just adds it so it Exists
	void Update(float dt); // loops thru all elements and calls their updaate
	void Draw() const;
	void HandleClick();
	void Exit(); // for the VECTORS
	void ForceResetToolTips();
};

///
/// elements heeere
/// 

class Button : public UIElement
{
private:
	std::function<void()> onClick;
	std::string toolTipText = "u didn't gimme a name";
	float hoverTimer = 0.0f;
	AEVec2 initialScale = GetScale();

public:
	using UIElement::UIElement;
	Button() = default;

	// use like startBtn->SetTooltipText("ur mom"); okayyy :3
	void SetTooltipText(const std::string& text);
	void SetOnClick(std::function<void()> func);

	void Update(float dt) override;
	void OnClick() override;
};

class PopUp : public UIElement
{
public:
	using UIElement::UIElement;

	// overrides setactive for the kiddos
	// refactor stuff using this in the future -2/2/26
	void SetActive(bool active) override;
};

#endif // !_UI_Manager_Header_