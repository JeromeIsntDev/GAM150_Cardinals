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
	:	
		globalPosition(AEVec2{ 0.f,0.f }),	
		globalRotation(0.f),				
		globalScale(AEVec2{ 0.f,0.f }),		
		mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
		isChild(false), mesh(nullptr), _tag(GameObjectTag::Static)
{
	AEMtx33Trans(&mtx_translation, globalPosition.x, globalPosition.y);
	AEMtx33Rot(&mtx_rotation, globalRotation);
	AEMtx33Scale(&mtx_scale, globalScale.x, globalScale.y);
	ConcatTransform();
}

GameObject::GameObject(const GameObject& rhs)
	:
		globalPosition(rhs.GetPosition()),
		globalRotation(rhs.GetRotatation()),
		globalScale(rhs.GetScale()),
		mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
		isChild(rhs.IsChild()), mesh(rhs.mesh), _tag(rhs._tag)
{
	AEMtx33Trans(&mtx_translation, globalPosition.x, globalPosition.y);
	AEMtx33Rot(&mtx_rotation, globalRotation);
	AEMtx33Scale(&mtx_scale, globalScale.x, globalScale.y);
	ConcatTransform();
}

GameObject::GameObject(AEGfxVertexList* mesh)
	: 
	globalPosition(AEVec2{ 0.f,0.f }),
	globalRotation(0.f), 
	globalScale(AEVec2{ 0.f,0.f }), 
	mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
	isChild(false), mesh(mesh), _tag(GameObjectTag::Static)
{
	AEMtx33Trans(&mtx_translation, globalPosition.x, globalPosition.y);
	AEMtx33Rot(&mtx_rotation, globalRotation);
	AEMtx33Scale(&mtx_scale, globalScale.x, globalScale.y);
	ConcatTransform();
}

GameObject::GameObject(AEGfxVertexList* mesh, AEVec2 translate, f32 rotate, AEVec2 scale)
	:	
		globalPosition(translate),	
		globalRotation(rotate),		
		globalScale(scale),			
		mtx_translation({ 0 }), mtx_rotation({ 0 }), mtx_scale({ 0 }), mtx_transform({ 0 }),
		isChild(false), mesh(mesh), _tag(GameObjectTag::Static)
{
	AEMtx33Trans(&mtx_translation, translate.x, translate.y);
	AEMtx33Rot(&mtx_rotation, rotate);
	AEMtx33Scale(&mtx_scale, scale.x, scale.y);
	ConcatTransform();
}

Collider GameObject::GetCollider() const {
	return collider;
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

void GameObject::SetID(u64 id)
{
	_id = id;
}

u64 GameObject::GetID() const
{
	return _id;
}

void GameObject::UpdatePosition(AEVec2 pos)
{
	globalPosition = pos;
	AEMtx33Trans(&mtx_translation, pos.x, pos.y);
	ConcatTransform();
	collider.UpdateCollider(globalPosition,GetScale().x,GetScale().y);
}

void GameObject::UpdateRotation(f32 rot)
{
	globalRotation = rot;
	AEMtx33Rot(&mtx_rotation, rot);
	ConcatTransform();
}

void GameObject::UpdateScale(AEVec2 scale)
{
	globalScale = scale;
	AEMtx33Scale(&mtx_scale, scale.x, scale.y);
	ConcatTransform();
	collider.UpdateCollider(globalPosition, globalScale.x, globalScale.y);
}

void GameObject::SetMesh(AEGfxVertexList* const objectmesh)
{
	mesh = objectmesh;
}

void GameObject::SetTexture(Texture* const texture)
{
	tex = texture;
}

bool GameObject::HoverMesh(s32 mouseX, s32 mouseY) const
{
	s32 mouX = mouseX - 800;
	s32 mouY = 450 - mouseY;
	f32 minX = globalPosition.x - (globalScale.x / 2.f);
	f32 maxX = globalPosition.x + (globalScale.x / 2.f);
	f32 minY = globalPosition.y - (globalScale.y / 2.f);
	f32 maxY = globalPosition.y + (globalScale.y / 2.f);

	return mouX > minX && mouX < maxX && mouY > minY && mouY < maxY;
}

void GameObject::Draw() const
{
	AEGfxTextureSet(tex->Get_Image(), 0.f, 0.f);
	AEGfxSetTransform(GetTransform().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void GameObject::DebugDraw() const
{
	switch (_tag)
	{
	case GameObjectTag::Static:
		break;
	case GameObjectTag::Dynamic:
		break;
	case GameObjectTag::Platform:
		AEGfxSetColorToMultiply(1.f, 0.f, 0.f, 0.5f);
		break;
	case GameObjectTag::Player:
		AEGfxSetColorToMultiply(2.f, 5.f, 2.f, 0.5f);
		break;
	case GameObjectTag::Win:
		AEGfxSetColorToMultiply(0.f, 3.f, 0.f, 0.5f);
		break;
	case GameObjectTag::GravUp:
		AEGfxSetColorToMultiply(0.f, 0.f, 1.f, 0.5f);
		break;
	case GameObjectTag::GravDown:
		AEGfxSetColorToMultiply(0.f, 0.f, 1.f, 0.5f);
		break;
	case GameObjectTag::GravLeft:
		AEGfxSetColorToMultiply(0.f, 0.f, 1.f, 0.5f);
		break;
	case GameObjectTag::Gravright:
		AEGfxSetColorToMultiply(0.f, 0.f, 1.f, 0.5f);
		break;
	default:
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.5f);
		break;
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(GetTransform().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

bool GameObject::IsChild() const {
	return isChild;
}

std::ostream& operator<<(std::ostream& os, const GameObject& go) {
	os	<< "GameObject[" << go._id << "]: "
		<< "Position: " << go.globalPosition.x << "," << go.globalPosition.y << "|"
		<< "Rotation: " << go.globalRotation << "|"
		<< "Scale: " << go.globalScale.x << "," << go.globalScale.y;
	return os;
}