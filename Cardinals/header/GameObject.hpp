/*!***********************************************************************
\file	GameObject.hpp
\author	Jerome Lim
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	HEADER File for GameObject class

\par	Changes: None
**************************************************************************/
#pragma once
#ifndef _Game_Object_Header_
#define _Game_Object_Header_

#include <AEEngine.h>
#include <iostream>
#include "../header/TextureManager.hpp"

class GameObject {
//Variables
protected:
	AEVec2 globalPosition, localPosition;
	f32 globalRotation, localRotation;
	AEVec2 globalScale, localScale;
	AEMtx33 mtx_translation;
	AEMtx33 mtx_rotation;
	AEMtx33 mtx_scale;
	AEMtx33 mtx_transform;
	bool isChild;
	AEGfxVertexList* mesh;
	Texture tex { };

//Functions
private:
	void ConcatTransform();
	AEMtx33 GetTransform() const;

public:
	virtual ~GameObject() = default; // destructor for polymorphism 

	GameObject();
	GameObject(AEGfxVertexList* mesh);
	GameObject(AEGfxVertexList* mesh, AEVec2 translate, f32 rotate, AEVec2 scale);

	AEVec2 GetPosition() const;
	f32 GetRotatation() const;
	AEVec2 GetScale() const;

	AEVec2& GetPosition();

	AEVec2 GetLocalPosition() const;
	f32 GetLocalRotatation() const;
	AEVec2 GetLocalScale() const;

	virtual void UpdatePosition(AEVec2 pos);
	virtual void UpdateRotation(f32 rot);
	virtual void UpdateScale(AEVec2 scale);
	virtual void SetMesh(AEGfxVertexList* mesh);
	virtual void SetTexture(const Texture texture);

	bool HoverMesh(s32 mouseX, s32 mouseY);

	// for UI, updates behavior
	virtual void Update(float dt) {}
	virtual void Draw() const;

	bool IsChild() const;

	friend std::ostream& operator<<(std::ostream& os, const GameObject& go);
};

#endif // !_Game_Object_Header_
