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

enum class GameObjectTag : int {
	Static = 0,
	Dynamic = 1,
	Platform = 2,
	Player = 3,
	Win = 4,
	GravUp = 5,
	GravDown = 6,
	GravLeft = 7,
	Gravright = 8,
	Obstacle = 9,

	GameObjectTagNumber
};

class GameObject {
private:
	AEMtx33 mtx_translation;
	AEMtx33 mtx_rotation;
	AEMtx33 mtx_scale;
	AEMtx33 mtx_transform;

//Variables
protected:
	AEGfxVertexList* mesh;
	AEVec2 globalPosition;
	f32 globalRotation;
	AEVec2 globalScale;
	bool isChild;
	Texture* tex;
	Collider collider;

//Functions
private:
	void ConcatTransform();

public:
	GameObjectTag _tag;

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

	bool HoverMesh(s32 mouseX, s32 mouseY) const;

	Texture* GetTexture() const { return tex; }
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
