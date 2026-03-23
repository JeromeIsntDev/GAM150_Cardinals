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
#include "MeshGenerator.hpp"
#include "TextureManager.hpp"
#include "Physics.hpp"
#include "GameObjectTag.hpp"

class GameObject {
private:
	AEMtx33 mtx_translation;
	AEMtx33 mtx_rotation;
	AEMtx33 mtx_scale;
	AEMtx33 mtx_transform;
	Texture* go_texture;

//Variables
protected:
	AEGfxVertexList* mesh;
	AEVec2 globalPosition;
	f32 globalRotation;
	AEVec2 globalScale;
	bool isChild;
	Collider collider;
	int slice;

//Functions
private:
	void ConcatTransform();

public:
	GameObjectTag _tag{ GameObjectTag::Static };

	virtual ~GameObject() = default; // destructor for polymorphism 

	GameObject();
	GameObject(const GameObject&);
	GameObject(AEGfxVertexList* mesh);
	GameObject(AEGfxVertexList* mesh, AEVec2 translate, f32 rotate, AEVec2 scale);

	Collider GetCollider() const;

	AEVec2 GetPosition() const;
	f32 GetRotation() const;
	AEVec2 GetScale() const;

	AEVec2& GetPosition();

	virtual void UpdatePosition(AEVec2 pos);
	virtual void UpdateRotation(f32 rot);
	virtual void UpdateScale(AEVec2 scale);
	virtual void SetMesh(AEGfxVertexList* const mesh);
	virtual void SetTexture(Texture* const texture);
	virtual void SetSlice(int uvSlice);
	virtual int GetSlice() const;

	bool HoverMesh(s32 mouseX, s32 mouseY) const;

	Texture* GetTexture() const;
	AEMtx33 GetTransform() const;

	// for UI, updates behavior
	virtual void Update(float dt) 
	{
		UNREFERENCED_PARAMETER(dt);
	}
	virtual void Draw() const;
	virtual void DrawSprite(f32, f32) const;
	virtual void DebugDraw() const;

	bool IsChild() const;

	friend std::ostream& operator<<(std::ostream& os, const GameObject& go);

	f32 uvX{ 0.f }, uvY{ 0.f };
	virtual void SetUVOffset(f32 u, f32 v) { uvX = u; uvY = v; }
};

#endif // !_Game_Object_Header_
