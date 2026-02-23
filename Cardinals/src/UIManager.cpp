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
#include "../header/InputHandler.hpp"

///
/// UI ELEMENTS
/// 

void UIElement::SetActive(bool active) { isActive = active; }

void UIElement::SetInteractable(bool interactable)
{
    isInteractable = interactable;
    if (!isInteractable) { tint = { 0.f, 0.f, 0.f, 1.0f }; }
}

void UIElement::SetTooltip(std::string text, AEVec2 pos)
{
    tooltipToDraw = text; tooltipPos = pos; isToolTipShown = true;
}

void UIElement::AddChild(UIElement* child)
{
    // attach to parent, parent overrides kids
    children.push_back(child);
    child->SetActive(false);
}

void UIElement::ResetTooltip() { isToolTipShown = false; } // reset every frame so tooltips dont smooch

bool UIElement::IsPointInside(float x, float y) const 
{
    return HoverMesh(x, y);
}

bool UIElement::ToggleActive(u8 key)
{
    if (AEInputCheckTriggered(key))
    {
        bool newState = !this->IsActive();
        this->SetActive(newState);

        // sets the kids too
        for (auto child : children) 
        {
            child->SetActive(newState);
            child->SetInteractable(newState);
        }

        return newState;
    }

    return this->IsActive();
}

/// 
/// UI BUTTONS
/// 

// use like startBtn->SetTooltipText("ur mom"); okayyy :3
void Button::SetTooltipText(const std::string& text) { toolTipText = text; }
void Button::SetOnClick(std::function<void()> func) { onClick = func; }

void Button::Update(float dt)
{
    if (!isActive || !isInteractable)
    {
        ResetTooltip();
        hoverTimer = 0.0f;
        return;
    }

    float scaleMultiplier = 1.1f;
    AEVec2 mousePos = UIManager::GetNewMousePos();

    // hover code, combined tints + scales texture when mouse is within button pos
    if (IsPointInside(mousePos.x, mousePos.y))
    {
        //std::cout << "bruh " << mousePos.x << " " << mousePos.y << std::endl;
        tint = { Colors::PastelPurple };
        GameObject::UpdateScale({ initialScale.x * scaleMultiplier, initialScale.y * scaleMultiplier });

        // small delay because having it shoved in ur face is ass
        hoverTimer += dt;
        if (hoverTimer > 0.5f)
        {
            this->SetTooltip(toolTipText, mousePos);
        }
    }
    else
    {
        tint = { 1.0f, 1.0f, 1.0f, 1.0f };
        scaleMultiplier = 1.0f;

        GameObject::UpdateScale({ initialScale.x * scaleMultiplier, initialScale.y * scaleMultiplier });

        hoverTimer = 0.0f; // double check
        ResetTooltip();
    }
}

void Button::OnClick()
{
    // literally just error checks and then handles onclick lmao
    if (!isActive || !isInteractable)
        return;

    if (onClick) onClick();
}

///
/// UI POPUPS
/// 

void PopUp::SetActive(bool active)
{
    isActive = active;
    for (auto* child : children)
    {
        child->SetActive(active);
        child->SetInteractable(active);
    }
}

///
/// UI MANAGER
/// 

AEVec2 UIManager::GetNewMousePos()
{
    // just converts the shityy gglobal mousepos into actual. mousepos

    auto& input = InputHandler::GetInstance();
    float rawX, rawY;
    input.getMousePos(rawX, rawY);

    float windowWidth = (float)AEGfxGetWindowWidth(), windowHeight = (float)AEGfxGetWindowHeight();

    return AEVec2 { rawX - (windowWidth / 2.0f), (windowHeight / 2.0f) - rawY };
}

void UIManager::InitTooltipFont(const FontFile& font, float size)
{
    tooltipRenderer.Load_Font(font, size);
    isFontLoaded = true;
}

void UIManager::Add(UIElement* element)
{
	uiElements.push_back(element);
}

void UIManager::Update(float dt) 
{
    for (UIElement* e : uiElements)
    {
        if (e->IsActive()) 
        {
            e->ResetTooltip(); // turn off by default
            e->Update(dt); // update for the hover
        }
    }

    HandleClick();
}

void UIManager::Draw() const
{
    // tint
    for (UIElement* e : uiElements) 
    {
        if (e->IsActive())
        {
            AEGfxSetColorToMultiply(e->GetTint().r, e->GetTint().g, e->GetTint().b, e->GetTint().a); // APPLY THE TINT RIGHT BEFORE DRAWING RAHHHH
            e->Draw();
        }
    }

    AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f); // reset

    if (!isFontLoaded) return; // fuck yo warnings

    // tool tip
    for (UIElement* e : uiElements) 
    {
        if (e->IsActive() && e->IsShowingTooltip()) 
        {
            AEVec2 pos = e->GetTooltipPos();

            // std::cout << "drawin tooltip: " << e->GetTooltipText() << " at " << e->GetTooltipPos().x << std::endl;
            tooltipRenderer.Input_Text(e->GetTooltipText().c_str());
            tooltipRenderer.Draw_Text(pos.x + 20.f, pos.y + 20.f);
        }
    }
}

void UIManager::HandleClick()
{
    // just tells the compiler to find the instance using referencing olololol
    // thank you prasanna
    auto& input = InputHandler::GetInstance();

    // mouse pointing calculation because screen view is NNNOOTT the same as game view iykwim
    // yk idk if this was a problem i just coded it cuz i felt like it wld have an issue lol
    if (input.isMouseBtnTriggered(AEVK_LBUTTON))
    {
        AEVec2 mousePos = GetNewMousePos();

        // can also use auto but imma use uielement for btr clarity
        for (UIElement* e : uiElements)
        {
            // use the trnalsated mouse coordinates
            if (e->IsActive() && e->IsInteractable())
            {
                if (e->IsPointInside(mousePos.x, mousePos.y))
                {
                    e->OnClick();
                    std::cout << "Element clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;
                    break;
                }
            }
        }
    }
}

void UIManager::Exit() 
{
    uiElements.clear(); // get rid of them dangling pointers 
    isFontLoaded = false;
}

void UIManager::ForceResetToolTips() 
{
    for (auto* e : uiElements) 
    {
        e->ResetTooltip();
    }
}