#include "GridManager.hpp"

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
		std::cout << "Cell Filled Already!" << std::endl;
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

	GameObject* obj = new GameObject{};
	obj->SetMesh(tileMesh);
	obj->SetTexture(&tileTexture);
	obj->UpdateScale({ 50.f,50.f });
	obj->UpdatePosition({ cell->position.x - 800.f , 450.f - cell->position.y });
	obj->_tag = static_cast<GameObjectTag>(tag);

	// leaving the tagging alone cuz im eepy
	TileUV uv{};
	if (spriteSheet.GetUV(tag, uv))
	{
		obj->uvX = uv.u;
		obj->uvY = uv.v;
	}

	cell->obj = obj;
	objs.push_back(obj);

	PRINT("[DEBUG] => Created New Tile at (%f, %f).\n", obj->GetPosition().x, obj->GetPosition().y);
}

void Grid::SnapToCell(f32* val)
{
	f32 newVal = *val / 50.f;
	s32 integral = static_cast<s32>(newVal);
	*val = (static_cast<f32>(integral) + .5f) * 50.f;
}

void Grid::DeleteTile(f32 x, f32 y)
{
	GridCell* cell = GetCell(y, x);

	if (cell->obj == nullptr) {
		std::cout << "Cell Empty!" << std::endl;
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
