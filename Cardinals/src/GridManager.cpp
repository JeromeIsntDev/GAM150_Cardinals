/*!***********************************************************************
\file	GridManager.cpp
\author	Jerome Lim
\date	29/02/2026
\par	Project: Cardinals

\brief	
**************************************************************************/
#include "GridManager.hpp"
#include "GameEditor.hpp"
#include <cmath>

std::ostream& operator<<(std::ostream& os, const GridCell& cell) {
	os << "GridCell[" << cell.column << "|" << cell.row << "]: "
		<< "Position: " << cell.position.x << "," << cell.position.y;

	if (cell.obj != nullptr) {
		os << "Cell Object =>" << cell.obj;
	}

	return os;
}

void Grid::Initialize()
{
	cells = new GridCell* [rows];
	for (u32 i{}; i < rows; ++i) {
		cells[i] = new GridCell[columns];
	}

	for (u32 i{}; i < rows; ++i) {
		for (u32 j{}; j < columns; ++j) {
			cells[i][j].position = {j*50.f + 25.f, i*50.f + 25.f};
			cells[i][j].column = static_cast<u32>(j);
			cells[i][j].row = static_cast<u32>(i);
		}
	}
}

void Grid::Load(Texture* sheet, int sheetW, int sheetH, int tileW, int tileH) 
{
	tileMesh = RectMeshCenterSliced(3, 3);
	standardMesh = RectMeshCenter();

	if (sheet) {
		tileTexture = *sheet;
	}
	else {
		tileTexture.Load_Image(Images::Placeholder);
	}

	spriteSheet.sheetW = sheetW; spriteSheet.sheetH = sheetH;
	spriteSheet.tileW = tileW; spriteSheet.tileH = tileH;
	spriteSheet.cols = sheetW / tileW; spriteSheet.rows = sheetH / tileH;
	spriteSheet.uvW = (f32)tileW / sheetW; spriteSheet.uvH = (f32)tileH / sheetH;
	
	spriteSheet.Slice(tileTexture);
	spriteSheet.RegisterAllTiles();

	obstacleTexture.Load_Image(Images::CandyCorn);
	gravTexture.Load_Image(Images::GravImg);
	winTexture.Load_Image(Images::WinPortal);
}

void Grid::Clear()
{
	if(tileMesh) AEGfxMeshFree(tileMesh);
	if (standardMesh) AEGfxMeshFree(standardMesh);
	obstacleTexture.Unload_Image();
	gravTexture.Unload_Image();
	winTexture.Unload_Image();
	//tileTexture.Unload_Image();
	objs.clear();
	for (u32 i = 0; i < rows; ++i) {
		delete[] cells[i];
	}
	delete[] cells;          
	cells = nullptr;
}

void Grid::Draw() const {
	for (u32 i{}; i < rows; ++i) {
		for (u32 j{}; j < columns; ++j) {
			if (cells[i][j].obj == nullptr) continue;
			cells[i][j].obj->DrawSprite(
				cells[i][j].obj->uvX,
				cells[i][j].obj->uvY
			);
		}
	}
}

void Grid::SpawnNewTile(int const& tag, int const& slice, f32 x, f32 y, f32 rot)
{
	GridCell* cell = GetCell(y, x);
	if (cell == nullptr) {
		PRINT("[WARNING] => Attempting to spawn on NULL Cell!\n");
		return;
	}
	if (cell->obj) {
		f32 ax{ cell->position.x }, ay{ cell->position.y };
		PRINT("[DEBUG] => Cell Grid Position: %f, %f\n", ax, ay);
		PRINT("TO\n");
		GridToWorld(&ax, &ay);
		PRINT("[DEBUG] => Cell World Position: %f, %f\n\n", ax, ay);

		ChangeTile(cell);
		return;
	}

	GameObject* obj = new GameObject{};
	obj->UpdateScale({ tileScale,tileScale });
	f32 ax{ cell->position.x }, ay{ cell->position.y };
	GridToWorld(&ax,&ay);
	obj->UpdatePosition({ ax , ay });
	obj->_tag = static_cast<GameObjectTag>(tag);
	obj->UpdateRotation(rot);
	
	TileUV uv{};
	switch (obj->_tag)
	{
	case GameObjectTag::Obstacle:
		obj->SetMesh(standardMesh);
		obj->SetTexture(&obstacleTexture);
		break;
	case GameObjectTag::Win:
		obj->SetMesh(standardMesh);
		obj->SetTexture(&winTexture);
		break;
	case GameObjectTag::GravUp:
		obj->SetMesh(standardMesh);
		obj->SetTexture(&gravTexture);
		break;
	case GameObjectTag::GravDown:
		obj->SetMesh(standardMesh);
		obj->SetTexture(&gravTexture);
		obj->UpdateRotation(PI);
		break;
	case GameObjectTag::GravLeft:
		obj->SetMesh(standardMesh);
		obj->SetTexture(&gravTexture);
		obj->UpdateRotation(PI/2.f);
		break;
	case GameObjectTag::Gravright:
		obj->SetMesh(standardMesh);
		obj->SetTexture(&gravTexture);
		obj->UpdateRotation(-(PI / 2.f));
		break;
	case GameObjectTag::Static:
	default:
		obj->SetMesh(tileMesh);
		obj->SetTexture(&tileTexture);
		obj->SetSlice(slice);
		if (spriteSheet.GetUV(slice, uv))
		{
			obj->uvX = uv.u;
			obj->uvY = uv.v;
		}
		break;
	}

	cell->tag = obj->_tag;
	cell->obj = obj;
	objs.push_back(obj);
}

void Grid::SnapToCell(f32* val)
{
	int sign = *val > 0 ? 1 : -1;
	int newVal = static_cast<int>((*val)/50.f);
	*val = static_cast<f32>(newVal) * 50.f + 25.f * static_cast<f32>(sign);
}
/*!
	\brief Converts grid coordinates to world coordinates
*/
void Grid::GridToWorld(f32* x, f32* y)
{
	f32 prevX{ *x }, prevY{ *y };
	*x = prevX - 800.f;
	*y = -(prevY - 450.f);
}

/*!
	\brief Converts world coordinates to grid coordinates
*/
void Grid::WorldToGrid(f32* x, f32* y)
{
	f32 prevX{ *x }, prevY{ *y };
	*x = prevX + 800.f;
	*y = 450.f - prevY;
}

void Grid::DeleteTile(f32 x, f32 y)
{
	GridCell* cell = GetCell(y, x);
	cell->tag = GameObjectTag::GameObjectTagNumber;

	if (cell->obj == nullptr) {
		//std::cout << "Cell Empty!" << std::endl;
		//PRINT("[DEBUG] => Cell Tag: %d\n", cell->tag);
		f32 ax{ cell->position.x }, ay{ cell->position.y };
		PRINT("[DEBUG] => Cell | Grid Position: %f, %f\n", ax, ay);
		PRINT("TO\n");
		GridToWorld(&ax, &ay);
		PRINT("[DEBUG] => Cell | World Position: %f, %f\n\n", ax, ay);
		return;
	}

	GameObject* toRemove = cell->obj;
	objs.erase(std::remove(objs.begin(), objs.end(), toRemove), objs.end());

	delete cell->obj;
	cell->obj = nullptr;
}

void Grid::ChangeTile(GridCell* cell) const
{
	TileUV uv{};
	int slice{ 0 };
	switch (cell->obj->_tag)
	{
	case GameObjectTag::Static:
		slice = cell->obj->GetSlice();
		++slice;
		if (slice > spriteSheet.slices.size() - 1) slice = 0;
		cell->obj->SetSlice(slice);
		if (spriteSheet.GetUV(slice, uv))
		{
			cell->obj->uvX = uv.u;
			cell->obj->uvY = uv.v;
		}
		break;
	case GameObjectTag::GravUp:
		cell->obj->_tag = GameObjectTag::GravLeft;
		break;
	case GameObjectTag::GravDown:
		cell->obj->_tag = GameObjectTag::Gravright;
		break;
	case GameObjectTag::GravLeft:
		cell->obj->_tag = GameObjectTag::GravDown;
		break;
	case GameObjectTag::Gravright:
		cell->obj->_tag = GameObjectTag::GravUp;
		break;
	default:
		break;
	}

	f32 rot = cell->obj->GetRotation() + PI / 2;
	if (rot >= PI * 2) rot = 0;
	if(cell->obj->_tag != GameObjectTag::Static)
		cell->obj->UpdateRotation(rot);
}

GridCell* Grid::GetCell(f32 x, f32 y)
{
	if (!cells) {
		PRINT("[WARNING] => Attemping to get Cells when not-allocated!\n");
		return nullptr;
	}

	s32 column = static_cast<s32>(x / 50.f);
	s32 row = static_cast<s32>(y / 50.f);

	if (x >= 900 || y >= 1600) {
		PRINT("[WARNING] => Attemping to go beyond Grid dimensions!\n");
		return nullptr;
	}

	return &cells[column][row];
}

GridCell::GridCell()
	: column{ 0 }, row{ 0 }, position {0.f, 0.f}, obj{nullptr}
{
}

GridCell::~GridCell()
{
	delete obj;
}
