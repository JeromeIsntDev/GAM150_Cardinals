/*!***********************************************************************
\file    UIManager.cpp
\author Breanne Chua
\date    16/01/2026
\par    Last Updated by: Breanne Chua
\par    Project: Cardinals

\brief    UIManager is responsible for the UI interfaces.

\par    Changes: None
**************************************************************************/

#include "../header/UIManager.hpp"

// adds every new element to the dynamic array 
// so we can explode ALL the UIs whenever we want
// all of this is rudimentary as fuck rn so..

void UIManager::Add(UIElement* element)
{
	uiElements.push_back(element);
}

void UIManager::Update(float dt) 
{
    for (UIElement* element : uiElements)
    {
        if (element->IsActive()) {
            element->Update(dt); // update for the hover
        }
    }

    HandleClick();
}

void UIManager::Draw() const
{
    for (UIElement* e : uiElements) 
    {
        if (e->IsActive())
        {
            AEGfxSetColorToMultiply(e->GetTint().r, e->GetTint().g, e->GetTint().b, e->GetTint().a); // APPLY THE TINT RIGHT BEFORE DRAWING RAHHHH
            e->Draw();

            AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f); // reset
        }
    }
}

void UIManager::Freeze()
{

}

// omg it handles
void UIManager::HandleClick()
{
    // just tells the compiler to find the instance using referencing olololol
    // thank you prasanna
    auto& input = InputHandler::GetInstance();

    // mouse pointing calculation because screen view is NNNOOTT the same as game view iykwim
    // yk idk if this was a problem i just coded it cuz i felt like it wld have an issue lol
    if (input.isMouseBtnTriggered(AEVK_LBUTTON))
    {
        float rawX, rawY;
        input.getMousePos(rawX, rawY);

        float windowWidth = (float)AEGfxGetWindowWidth();
        float windowHeight = (float)AEGfxGetWindowHeight();

        // convert top left to center
        float translatedX = rawX - (windowWidth / 2.0f);
        float translatedY = (windowHeight / 2.0f) - rawY;

        for (auto* element : uiElements)
        {
            // use the trnalsated mouse coordinates
            if (element->IsActive() && element->IsInteractable())
            {
                if (element->IsPointInside(translatedX, translatedY))
                {
                    element->OnClick();
                    std::cout << "Element clicked at: " << translatedX << ", " << translatedY << std::endl;
                    break;
                }
            }
        }
    }
}

void UIManager::Exit() 
{
    uiElements.clear(); // dangl;ing pointers fark you
}

///
/// element-related stuff heeere
/// 

bool UIElement::IsPointInside(float x, float y) const {
	// calling from the parent class woawow
	AEVec2 currentScale = GetScale();
	AEVec2 currentPos = GetPosition();

	float halfW = currentScale.x / 2.0f, halfH = currentScale.y / 2.0f;

	// wowww calculations zzzzz
	return (x >= currentPos.x - halfW && x <= currentPos.x + halfW &&
		y >= currentPos.y - halfH && y <= currentPos.y + halfH);
}

bool UIElement::ToggleActive(u8 key) 
{
    if (AEInputCheckTriggered(key))
    {
        bool newState = !this->IsActive();
        this->SetActive(newState);

        // sets the kids too
        for (auto child : children) {
            child->SetActive(newState);
            child->SetInteractable(newState);
        }

        return newState;
    }

    return this->IsActive();
}

void Button::Update(float dt) 
{
    // for button hover

    if (!isActive || !isInteractable) return;

    auto& input = InputHandler::GetInstance();
    float rawX, rawY;
    input.getMousePos(rawX, rawY);
    //std::cout << "bruh " << rawX << " " << rawY << std::endl;

    // conv into game coordinates
    float windowWidth = (float)AEGfxGetWindowWidth(), windowHeight = (float)AEGfxGetWindowHeight();
    float translatedX = rawX - (windowWidth / 2.0f), translatedY = (windowHeight / 2.0f) - rawY;

    if (IsPointInside(translatedX, translatedY)) 
    {
        tint = { 0.8f, 0.8f, 1.0f, 1.0f };
    }
    else 
    {
        tint = { 1.0f, 1.0f, 1.0f, 1.0f };
    }
}