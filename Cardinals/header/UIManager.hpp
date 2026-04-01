/**
 * @file		UIManager.hpp
 * @brief		The UIManager is responsible for the UI interfaces.
 * @details		Handles everything related to UI and is inherited off GameObjects.
 * @author		Chua Jia Xuan Breanne
 * @date		16.01.2026
 */

#pragma once
#ifndef _UI_Manager_Header_
#define _UI_Manager_Header_

#include "GameObject.hpp"
#include "AudioManager.hpp"
#include "InputHandler.hpp"

#include <AEEngine.h>
#include <vector>
#include <functional>

 /**
  * @brief Base class for all UI components, inheriting from GameObject
  * Handles visibility, interactivity, parent-child relationships,
  * and tooltip data storage.
  */
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

	using GameObject::GameObject;
	UIElement(AEGfxVertexList* mesh, AEVec2 pos, float rot, AEVec2 scale) : GameObject(mesh, pos, rot, scale) {}

	/**
	 * @brief Sets whether the element should be rendered and updated
	 * @param active Boolean status for visibility/activity
	 * @return void
	 */
	virtual void SetActive(bool active);

	/**
	 * @brief Sets whether the element responds to mouse clicks/hovers
	 * @param interactable Boolean status for interaction
	 * @return void
	 */
	void SetInteractable(bool interactable);

	/**
	 * @brief Checks if the UI element is currently active
	 * @return bool True if the element is active and should be updated/drawn
	 */
	bool IsActive() const { return isActive; }

	/**
	 * @brief Checks if the UI element is set to receive user input
	 * @return bool True if the element can be hovered or clicked
	 */
	bool IsInteractable() const { return isInteractable; }

	/**
	 * @brief Configures the tooltip text and position
	 * @param text The string to display when hovered
	 * @param pos The relative or absolute position for the tooltip
	 * @return void
	 */
	void SetTooltip(std::string text, AEVec2 pos);

	/**
	 * @brief Attaches a child element to this UI component
	 * @param child Pointer to the UIElement to be managed as a child
	 * @return void
	 */
	void AddChild(UIElement* child);

	/**
	 * @brief Checks if a given coordinate is within the element's bounds
	 * @param x Horizontal coordinate
	 * @param x Horizontal coordinate
	 * @param y Vertical coordinate
	 * @return bool True if point is inside, false otherwise
	 */
	bool IsPointInside(float x, float y) const;

	/**
	 * @brief Toggles the active state based on a specific key trigger
	 * @param key The AEInput key code to check
	 * @return bool The new active state
	 */
	bool ToggleActive(u8 key);
	Color GetTint() const { return tint; }

	// tool tip getters
	bool IsShowingTooltip() const { return isToolTipShown; }
	std::string GetTooltipText() const { return tooltipToDraw; }
	AEVec2 GetTooltipPos() const { return tooltipPos; }
	void ResetTooltip();

	/**
	 * @brief Virtual function triggered when a valid click is detected
	 * To be overridden by derived classes like Button
	 * @return void
	 */
	virtual void OnClick() {}

	/**
	 * @brief Destructor for UIElement
	 * Recursively deletes all registered child elements to prevent memory leaks
	 * @return void
	 */
	virtual ~UIElement()
	{
		for (auto* child : children)
		{
			delete child;
		}

		children.clear();
	}
};

/**
 * @brief Singleton manager that coordinates UI lifecycle and rendering
 * * Responsible for updating all UI elements, handling global click
 * events, and managing tooltip rendering logic.
 */
class UIManager 
{

private:
	std::vector<UIElement*> uiElements;

	mutable Text tooltipRenderer;
	bool isFontLoaded = false;
	AudioManager& bgm = AudioManager::Instance();

public:

	virtual ~UIManager() = default;

	/**
	 * @brief Converts raw screen mouse coordinates to world/UI space
	 * @return AEVec2 The translated mouse position
	 */
	static AEVec2 GetNewMousePos();

	/**
	 * @brief Initializes the font used for rendering UI tooltips
	 * @param font The FontFile struct containing path data
	 * @param size The point size of the font
	 * @return void
	 */
	void InitTooltipFont(const FontFile& font, float size);

	/**
	 * @brief Registers a new UI element with the manager
	 * @param element Pointer to the UIElement to be managed
	 * @return void
	 */
	void Add(UIElement* element);

	/**
	 * @brief Processes input and updates state for all active UI elements
	 * @param dt Delta time since the last frame
	 * @return void
	 */
	virtual void Update(float dt);

	/**
	 * @brief Renders all active UI elements and active tooltips
	 * @return void
	 */
	void Draw() const;

	/**
	 * @brief Checks for mouse clicks and triggers OnClick for the hovered element
	 * @return void
	 */
	void HandleClick();

	/**
	 * @brief Shuts down the UI manager and clears all element pointers
	 * @return void
	 */
	void Exit();

	/**
	 * @brief Forcefully hides all tooltips regardless of hover state
	 * @return void
	 */
	void ForceResetToolTips();
};

///
/// ELEMENTS
///

/**
 * @brief Interactive button class with hover effects and callbacks
 */
class Button : public UIElement
{
private:
	std::function<void()> onClick;
	std::string toolTipText = "WARNING: NO TOOLTIP GIVEN";
	float hoverTimer = 0.0f;
	AEVec2 initialScale = GetScale();

public:
	using UIElement::UIElement;
	Button() = default;

	/**
	 * @brief Sets the text to be displayed in the tooltip
	 * @param text The description string
	 * @return void
	 */
	void SetTooltipText(const std::string& text);

	/**
	 * @brief Assigns a function or lambda to be called when clicked
	 * @param func The function/callback to store
	 * @return void
	 */
	void SetOnClick(std::function<void()> func);

	/**
	 * @brief Handles hover logic, scaling effects, and click execution
	 * @param dt Delta time for the hover timer
	 * @return void
	 */
	void Update(float dt) override;
	
	/**
	 * @brief Executes the assigned onClick callback and plays feedback audio
	 * @return void
	 */
	void OnClick() override;
};

#endif // !_UI_Manager_Header_