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

Texture* GameObject::GetTexture() const
{
	return go_texture;
}

AEMtx33 GameObject::GetTransform() const
{
	return mtx_transform;
}

GameObject::GameObject()
	:	
	globalPosition{ 0.f,0.f },	
	globalRotation{ 0.f },				
	globalScale{ 0.f,0.f },		
	mtx_translation{ 0 }, mtx_rotation{ 0 }, mtx_scale{ 0 }, mtx_transform{ 0 },
	isChild{ false }, mesh{ nullptr }, _tag{ GameObjectTag::Static }, slice{ 0 },
	go_texture{nullptr}
{
	AEMtx33Trans(&mtx_translation, globalPosition.x, globalPosition.y);
	AEMtx33Rot(&mtx_rotation, globalRotation);
	AEMtx33Scale(&mtx_scale, globalScale.x, globalScale.y);
	ConcatTransform();
}

GameObject::GameObject(const GameObject& rhs)
	:
	globalPosition{ rhs.GetPosition() },
	globalRotation{ rhs.GetRotation() },
	globalScale{ rhs.GetScale() },
	mtx_translation{ 0 }, mtx_rotation{ 0 }, mtx_scale{ 0 }, mtx_transform{ 0 },
	isChild{ rhs.IsChild() }, mesh{ rhs.mesh }, _tag{ rhs._tag }, slice{ rhs.slice },
	go_texture{ nullptr }
{
	AEMtx33Trans(&mtx_translation, globalPosition.x, globalPosition.y);
	AEMtx33Rot(&mtx_rotation, globalRotation);
	AEMtx33Scale(&mtx_scale, globalScale.x, globalScale.y);
	ConcatTransform();
}

GameObject::GameObject(AEGfxVertexList* mesh)
	: 
	globalPosition{ 0.f,0.f },
	globalRotation{ 0.f }, 
	globalScale{ 0.f,0.f }, 
	mtx_translation{ 0 }, mtx_rotation{ 0 }, mtx_scale{ 0 }, mtx_transform{ 0 },
	isChild{ false }, mesh{ mesh }, _tag{ GameObjectTag::Static }, slice{ 0 },
	go_texture{ nullptr }
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
		isChild(false), mesh(mesh), _tag(GameObjectTag::Static), go_texture{ nullptr }
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

f32 GameObject::GetRotation() const {
	return globalRotation;
}

AEVec2 GameObject::GetScale() const {
	return globalScale;
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
	this->go_texture = texture;
}

void GameObject::SetSlice(int uvSlice)
{
	slice = uvSlice;
}

int GameObject::GetSlice() const
{
	return slice;
}

bool GameObject::HoverMesh(s32 mouseX, s32 mouseY) const
{
	f32 halfW = globalScale.x / 2.f;
	f32 halfH = globalScale.y / 2.f;

	return mouseX >= globalPosition.x - halfW 
		&& mouseX <= globalPosition.x + halfW 
		&& mouseY >= globalPosition.y - halfH 
		&& mouseY <= globalPosition.y + halfH;
}

void GameObject::Draw() const
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(go_texture->Get_Image(), 0.f, 0.f);
	AEGfxSetTransform(GetTransform().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void GameObject::DrawSprite(f32 offX, f32 offY) const
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(go_texture->Get_Image(), offX, offY);
	AEGfxSetTransform(GetTransform().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void GameObject::DebugDraw() const
{
	AEVec2 maxCollider = GetCollider().p_maxPoint;
	AEVec2 minCollider = GetCollider().p_minPoint;
	AEVec2 scale;
	switch (_tag)
	{
	case GameObjectTag::Platform: 
		AEGfxSetColorToMultiply(1.f, 0.f, 0.f, 0.5f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	case GameObjectTag::Player:
		AEGfxSetColorToAdd(0.f, .25f, .25f, 0.f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	case GameObjectTag::Win:
		AEGfxSetColorToMultiply(0.f, 3.f, 0.f, 0.5f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	case GameObjectTag::GravUp:
		AEGfxSetColorToAdd(0.f, .65f, 0.f, 0.f);
		AEGfxSetColorToMultiply(1.f, 0.f, 1.f, 0.5f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	case GameObjectTag::GravDown:
		AEGfxSetColorToAdd(.65f, 0.f, 0.f, 0.f);
		AEGfxSetColorToMultiply(1.f, 0.f, 1.f, 0.5f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	case GameObjectTag::GravLeft:
		AEGfxSetColorToMultiply(1.f, 0.f, 1.f, 0.5f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	case GameObjectTag::Gravright:
		AEGfxSetColorToMultiply(1.f, 0.f, 1.f, 0.5f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	case GameObjectTag::Obstacle:
		AEGfxSetColorToAdd(.65f, 0.f, 0.f, 0.f);
		AEVec2Set(&scale, maxCollider.x - minCollider.x, maxCollider.y - minCollider.y);
		break;
	default:
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.5f);
		AEVec2Set(&scale, globalScale.x, globalScale.y);
		break;
	}

	AEMtx33 transform, scl;
	AEMtx33Scale(&scl, scale.x, scale.y);

	AEMtx33Concat(&transform, &mtx_rotation, &scl);
	AEMtx33Concat(&transform, &mtx_translation, &transform);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);
}

bool GameObject::IsChild() const {
	return isChild;
}

std::ostream& operator<<(std::ostream& os, const GameObject& go) {
	os	
	<< "Position: " << go.globalPosition.x << "," << go.globalPosition.y << "|"
	<< "Rotation: " << go.globalRotation << "|"
	<< "Scale: " << go.globalScale.x << "," << go.globalScale.y;
	return os;
}