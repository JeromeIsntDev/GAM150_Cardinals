/*!***********************************************************************
\file	GameObject.cpp
\author	Jerome Lim
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	SOURCE File for GameObject

\par	Changes: None
**************************************************************************/
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
		isChild(false), mesh(nullptr)
{
	ConcatTransform();
}

GameObject::GameObject(AEGfxVertexList* mesh)
	: globalPosition(AEVec2{ 0.f,0.f }), localPosition(AEVec2{ 0.f,0.f }),
	globalRotation(0.f), localRotation(0.f),
	globalScale(AEVec2{ 0.f,0.f }), localScale(AEVec2{ 0.f,0.f }),
	mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
	isChild(false), mesh(mesh)
{
	ConcatTransform();
}

GameObject::GameObject(AEGfxVertexList* mesh, AEVec2 translate, f32 rotate, AEVec2 scale)
	:	globalPosition(translate),	localPosition(translate),
		globalRotation(rotate),		localRotation(rotate),
		globalScale(scale),			localScale(scale),
		mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
		isChild(false), mesh(mesh)
{
	AEMtx33Trans(&mtx_translation, translate.x, translate.y);
	AEMtx33Rot(&mtx_rotation, rotate);
	AEMtx33Scale(&mtx_scale, scale.x, scale.y);
	ConcatTransform();
}

AEVec2 GameObject::GetPosition() const {
	return globalPosition;
}

AEVec2& GameObject::GetPosition() {
	return globalPosition;
}

f32 GameObject::GetRotatation() const {
	return globalRotation;
}

AEVec2 GameObject::GetScale() const {
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
	AEMtx33Trans(&mtx_translation, pos.x, pos.y);
	localPosition = isChild ? /*replace this with the local*/ pos : pos;
	ConcatTransform();
}

void GameObject::UpdateRotation(f32 rot)
{
	globalRotation = rot;
	AEMtx33Rot(&mtx_rotation, rot);
	localRotation = isChild ? /*replace this with the local*/ rot : rot;
	ConcatTransform();
}

void GameObject::UpdateScale(AEVec2 scale)
{
	globalScale = scale;
	AEMtx33Scale(&mtx_scale, scale.x, scale.y);
	localScale = isChild ? /*replace this with the local*/ scale : scale;
	ConcatTransform();
}

void GameObject::SetMesh(AEGfxVertexList* objectmesh)
{
	mesh = objectmesh;
}

void GameObject::SetTexture(const Texture texture)
{
	tex = texture;
}

void GameObject::Draw() const
{
	AEGfxTextureSet(tex.Get_Image(), 0.f, 0.f);
	AEGfxSetTransform(GetTransform().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

}

bool GameObject::IsChild() const {
	return isChild;
}

std::ostream& operator<<(std::ostream& os, const GameObject& go) {
	os << go.globalPosition.x << "," << go.globalPosition.y << std::endl;
	return os;
}
