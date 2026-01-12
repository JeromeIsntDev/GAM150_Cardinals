#pragma once
#ifndef _Game_Object_Header_

#include <AEEngine.h>

class GameObject {
//Variables
private:
	AEVec2 globalPosition, localPosition;
	f32 globalRotation, localRotation;
	AEVec2 globalScale, localScale;
	AEMtx33 mtx_translation;
	AEMtx33 mtx_rotation;
	AEMtx33 mtx_scale;
	AEMtx33 mtx_transform;
	bool isChild;
	//To add texture/image and/or mesh

//Functions
private:
	void ConcatTransform();
	AEMtx33 GetTransform() const;

public:
	GameObject();
	GameObject(AEVec2 translate, f32 rotate, AEVec2 scale);

	AEVec2 GetGlobalPosition() const;
	f32 GetGlobalRotatation() const;
	AEVec2 GetGlobalScale() const;

	AEVec2 GetLocalPosition() const;
	f32 GetLocalRotatation() const;
	AEVec2 GetLocalScale() const;

	void UpdatePosition(AEVec2 pos);
	void UpdateRotation(f32 rot);
	void UpdateScale(AEVec2 scale);

	void Draw() const;

	bool IsChild() const;
};

#endif // !_Game_Object_Header_
