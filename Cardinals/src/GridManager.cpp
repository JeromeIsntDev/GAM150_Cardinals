#include "GridManager.hpp"
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
			cells[i][j].column = static_cast<float>(j);
			cells[i][j].row = static_cast<float>(i);
		}
	}
}

void Grid::Load(Texture* sheet, int sheetW, int sheetH, int tileW, int tileH) 
{
	tileMesh = RectMeshCenterSliced(3, 3);

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

}

void Grid::Clear()
{
	if(tileMesh) AEGfxMeshFree(tileMesh);
	//tileTexture.Unload_Image();
	objs.clear();
	for (u32 i = 0; i < rows; ++i) {
		delete[] cells[i];
	}
	delete[] cells;          
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

void Grid::SpawnNewTile(int const& tag, f32 x, f32 y)
{
	GridCell* cell = GetCell(y, x);

	if (cell->obj) {
		//PRINT("[DEBUG] => Cell Filled Already!\n");
		//PRINT("[DEBUG] => Cell Tag: %d\n", cell->tag);
		f32 ax{ cell->position.x }, ay{ cell->position.y };
		PRINT("[DEBUG] => Cell Grid Position: %f, %f\n", ax, ay);
		PRINT("TO\n");
		GridToWorld(&ax, &ay);
		PRINT("[DEBUG] => Cell World Position: %f, %f\n\n", ax, ay);
		return;
	}

	//editorText.Unload_Image();
	////qstd::cout << "Spawning: " << tileTag << std::endl;
	//switch (static_cast<GameObjectTag>(tag))
	//{
	//case GameObjectTag::GravUp:
	//	PRINT("GRAVUP TILE\n");
	//	editorText.Load_Image(Images::Lollipop);
	//	break;
	//case GameObjectTag::GravDown:
	//	editorText.Load_Image(Images::Lollipop);
	//	break;
	//case GameObjectTag::GravLeft:
	//	editorText.Load_Image(Images::Lollipop);
	//	break;
	//case GameObjectTag::Gravright:
	//	editorText.Load_Image(Images::Lollipop);
	//	break;
	//case GameObjectTag::Win:
	//	PRINT("WIN TILE\n");
	//	editorText.Load_Image(Images::Placeholder);
	//	break;
	//default:
	//	editorText.Load_Image(Images::Placeholder);
	//	break;
	//}

	/*PRINT("[DEBUG] => Spawn at Grid Coordinates: %f,%f\n", cell->position.x, cell->position.y);
	f32 ax = cell->position.x;
	f32 ay = cell->position.y;
	GridToWorld(&ax, &ay);
	PRINT("[DEBUG] => Spawn at World Coordinates: %f,%f\n", ax,ay);*/

	GameObject* obj = new GameObject{};
	obj->SetMesh(tileMesh);
	obj->SetTexture(&tileTexture);
	obj->UpdateScale({ 50.f,50.f });
	f32 ax{ cell->position.x }, ay{ cell->position.y };
	GridToWorld(&ax,&ay);
	obj->UpdatePosition({ ax , ay });
	obj->_tag = static_cast<GameObjectTag>(tag);

	cell->tag = obj->_tag;

	// leaving the tagging alone cuz im eepy
	TileUV uv{};
	if (spriteSheet.GetUV(tag, uv))
	{
		obj->uvX = uv.u;
		obj->uvY = uv.v;
	}

	cell->obj = obj;
	objs.push_back(obj);

	//PRINT("[DEBUG] => Created New Tile at (%f, %f). With tag: %d\n", obj->GetPosition().x, obj->GetPosition().y, cell->tag);
}

void Grid::SnapToCell(f32* val)
{
	////std::cout << "snap Val: " << *val << std::endl;
	//f32 newVal = *val / 50.f;
	////std::cout << "newVal: " << newVal << std::endl;
	//s32 integral = static_cast<s32>(newVal);
	//*val = (static_cast<f32>(integral) * 50.f);
	*val = std::floor(*val / 50.f) * 50.f;
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

GridCell* Grid::GetCell(f32 x, f32 y)
{
	s32 column = x / 50.f;
	s32 row = y / 50.f;

	if (x >= 900 || y >= 1600) return nullptr;

	return &cells[column][row];
}

//std::vector<GameObject*> Grid::GetCellGameObject() const
//{
//	std::vector<GameObject*> objs;
//	for (u32 i{}; i < rows; ++i) {
//		for (u32 j{}; j < columns; ++j) {
//			if (cells[i][j].obj == nullptr) continue;
//			objs.push_back(cells[i][j].obj);
//		}
//	}
//	return objs;
//}

GridCell::GridCell()
	: column{ 0 }, row{ 0 }, position {0.f, 0.f}, obj{nullptr}
{
}

GridCell::~GridCell()
{
	delete obj;
}
