#include "GridManager.hpp"

std::ostream& operator<<(std::ostream& os, const GridCell& cell) {
	os << "GridCell[" << cell.column << "|" << cell.row << "]: "
		<< "Position: " << cell.position.x << "," << cell.position.y;

	if (cell.obj != nullptr) {
		os << "Cell Object =>" << cell.obj;
	}

	return os;
}

Grid::Grid()
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

Grid::~Grid()
{
	for (u32 i = 0; i < rows; ++i) {
		delete[] cells[i];
	}
	delete[] cells;          
}

void Grid::Draw() const {
	for (u32 i{}; i < rows; ++i) {
		for (u32 j{}; j < columns; ++j) {
			if (cells[i][j].obj == nullptr) continue;
			std::cout << "Drawing" << "\n";
			cells[i][j].obj->Draw();
		}
	}
}

GridCell* Grid::GetCell(f32 x, f32 y)
{
	s32 column = x / 50.f;
	s32 row = y / 50.f;

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
