#include "../header/GameObject.hpp"

void GameObject::ConcatTransform()
{
	AEMtx33Concat(&mtx_transform, &mtx_rotation, &mtx_scale);
	AEMtx33Concat(&mtx_transform, &mtx_translation, &mtx_transform);
}

AEMtx33 GameObject::GetTransform() const
{
	return mtx_transform;
}

GameObject::GameObject()
	:	globalPosition(AEVec2{ 0.f,0.f }),	localPosition(AEVec2{ 0.f,0.f }),
		globalRotation(0.f),				localRotation(0.f),
		globalScale(AEVec2{ 0.f,0.f }),		localScale(AEVec2{ 0.f,0.f }),
		mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
		isChild(false)
{
	ConcatTransform();
}

GameObject::GameObject(AEVec2 translate, f32 rotate, AEVec2 scale)
	:	globalPosition(translate),	localPosition(translate),
		globalRotation(rotate),		localRotation(rotate),
		globalScale(scale),			localScale(scale),
		mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
		isChild(false)
{
	AEMtx33Trans(&mtx_translation, translate.x, translate.y);
	AEMtx33Rot(&mtx_rotation, rotate);
	AEMtx33Scale(&mtx_scale, scale.x, scale.y);
	ConcatTransform();
}

AEVec2 GameObject::GetGlobalPosition() const {
	return globalPosition;
}

f32 GameObject::GetGlobalRotatation() const {
	return globalRotation;
}

AEVec2 GameObject::GetGlobalScale() const {
	return globalScale;
}

AEVec2 GameObject::GetLocalPosition() const {
	return localPosition;
}

f32 GameObject::GetLocalRotatation() const {
	return localRotation;
}

AEVec2 GameObject::GetLocalScale() const {
	return localScale;
}

void GameObject::UpdatePosition(AEVec2 pos)
{
	globalPosition = pos;
	localPosition = isChild ? /*replace this with the local*/ pos : pos;
	ConcatTransform();
}

void GameObject::UpdateRotation(f32 rot)
{
	globalRotation = rot;
	localRotation = isChild ? /*replace this with the local*/ rot : rot;
	ConcatTransform();
}

void GameObject::UpdateScale(AEVec2 scale)
{
	globalScale = scale;
	localScale = isChild ? /*replace this with the local*/ scale : scale;
	ConcatTransform();
}

void GameObject::Draw() const
{
	AEGfxSetTransform(GetTransform().m);
	//AEGfxMeshDraw(_pMesh, AE_GFX_MDM_TRIANGLES);
}

bool GameObject::IsChild() const {
	return isChild;
}
