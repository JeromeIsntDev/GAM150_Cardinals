#include "GridManager.hpp"

Grid::Grid()
{
	grid = new GridCell* [rows];
	for (u32 i{}; i < rows; ++i) {
		grid[i] = new GridCell[columns];
	}

	for (u32 i{}; i < rows; ++i) {
		for (u32 j{}; j < columns; ++j) {
			grid[i][j].position = {j*50.f + 25.f, i*50.f + 25.f};
		}
	}
}

Grid::~Grid()
{
	for (u32 i = 0; i < rows; ++i) {
		delete[] grid[i];
	}
	delete[] grid;          
}

void Grid::Draw() {

}

GridCell Grid::GetCell(f32 x, f32 y)
{
	s32 column = x / 50.f;
	s32 row = y / 50.f;

	return grid[column][row];
}
