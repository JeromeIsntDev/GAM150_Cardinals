/**
 * @file		GameObject.hpp
 * @brief		The GameObject class that almost every other object derives from
 * @details		Handles Position, Rotation, Scale, Drawing, Updating and many other functions
 * @author		Jerome Lim
 * @date		12.01.2026
 * 
 */

#pragma once
#ifndef _Game_Object_Header_
#define _Game_Object_Header_

#include <AEEngine.h>
#include <iostream>
#include "MeshGenerator.hpp"
#include "TextureManager.hpp"
#include "Physics.hpp"
#include "GameObjectTag.hpp"

/**
 * @class GameObject
 * @brief The Main Class that almost all objects derive from
*/
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

/**
 * @brief		Concatanates the set matrices that will be used to draw 
 *				the mesh onto the screen
 */
	void ConcatTransform();

public:
	GameObjectTag _tag{ GameObjectTag::Static };

	virtual ~GameObject() = default; // destructor for polymorphism 

/**
 * @brief		Default Constructor of the GameObject
 *				Intializes the transform variables and matrices to default or 0
				Sets all pointers to nullptr
 */
	GameObject();

/**
 * @brief		Copy Constructor of the GameObject; only copies transform values
 */
	GameObject(const GameObject&);

/**
 * @brief		Single-argument Constructor that takes in a Mesh
 *				All other values are set to default constructor values
 */
	GameObject(AEGfxVertexList* mesh);

/**
 * @brief		"Multi-argument" Constructor that sets Mesh and Transform Values
 */
	GameObject(AEGfxVertexList* mesh, AEVec2 translate, f32 rotate, AEVec2 scale);

 /**
* @brief		Returns the collider of this GameObject
*/
	Collider GetCollider() const;

 /**
* @brief		Returns the Transform Position of the GameObject
*/
	AEVec2 GetPosition() const;

/**
* @brief		Returns the Transform Position of the GameObject
*/
	f32 GetRotation() const;

/**
* @brief		Returns the Transform Position of the GameObject
*/
	AEVec2 GetScale() const;

/**
* @brief		Returns the Reference to Transform Position of the GameObject
*/
	AEVec2& GetPosition();

/**
* @brief		Updates the GameObject Position with the given Vector
*/
	virtual void UpdatePosition(AEVec2 pos);

/**
* @brief		Updates the GameObject Rotation with the float
*/
	virtual void UpdateRotation(f32 rot);

/**
* @brief		Updates the GameObject Scale with the given Vector
*/
	virtual void UpdateScale(AEVec2 scale);

/**
* @brief		Set the GameObject Mesh
*/
	virtual void SetMesh(AEGfxVertexList* const mesh);

/**
* @brief		Set the Texture of the GameObject
*/
	virtual void SetTexture(Texture* const texture);

/**
* @brief		Set the number of UV slices of the GameObject; used for tilesheet
*/
	virtual void SetSlice(int uvSlice);

/**
* @brief		Get the UV slice of the GameObject
*/
	virtual int GetSlice() const;

/**
* @brief		Checks if the mouse is currently within the bounds of the current GameObject
*/
	bool HoverMesh(s32 mouseX, s32 mouseY) const;

/**
* @brief		Returns a pointer to the GameObject Texture 
*/
	Texture* GetTexture() const;

/**
* @brief		Returns a 3 by 3 matrix transform of the GameObject
*/
	AEMtx33 GetTransform() const;

/**
* @brief		Update Function of the GameObject; Used for UI
*/
	virtual void Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

/**
* @brief		Draw the GameObject onto the screen
*/
	virtual void Draw() const;

/**
* @brief		Draw the GameObject with the given tilesheet dimensions
*/
	virtual void DrawSprite(f32, f32) const;

/**
* @brief		Draw the GameObject's Debug Information such as Collider and Type
*/
	virtual void DebugDraw() const;

/**
* @brief		Used to check if this GameObject is a child GameObject
*/
	bool IsChild() const;

/**
* @brief		Friend Function to output to debug
*/
	friend std::ostream& operator<<(std::ostream& os, const GameObject& go);

	f32 uvX{ 0.f }, uvY{ 0.f };
/**
* @brief		Set the UV Offset
*/
	virtual void SetUVOffset(f32 u, f32 v) { uvX = u; uvY = v; }
};

#endif // !_Game_Object_Header_
