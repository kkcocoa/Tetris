#include "CBlock.h"
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>

IMAGE* CBlock::imgs[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
int CBlock::size = 36;

CBlock::CBlock()
{
	if (imgs[0] == NULL) {
		IMAGE imgTmp;
		loadimage(&imgTmp, "images/tiles.png");
		SetWorkingImage(&imgTmp);
		for (int i = 0; i < 7; i++) {
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}
		SetWorkingImage();
		srand(time(NULL));
	}
	// 初始化blocktype
	blocktype = 1 + rand() % 7;
	img = imgs[blocktype - 1];

	int blocks[7][4] = {
		1, 3, 5, 7, // I
		2, 4, 5, 7, // Z 1
		3, 5, 4, 6, // Z 2
		3, 5, 4, 7, // T
		2, 3, 5, 7, // L 
		3, 5, 7, 6, // J
		2, 3, 4, 5, // 田
	};

	// 初始化smallBlock
	for (int i = 0; i < 4; i++)
	{
		int value = blocks[blocktype - 1][i];
		smallBlock[i].row = value / 2;
		smallBlock[i].col = value % 2 + 4;
	}


}

void CBlock::drop()
{
	for (int i = 0; i < 4; i++)
	{
		smallBlock[i].row++;
	}
}

void CBlock::draw(int left_margin, int top_margin)
{
	for (int i = 0; i < 4; i++) {
		int x = smallBlock[i].col * size + left_margin;
		int y = smallBlock[i].row * size + top_margin;
		putimage(x, y, img);
	}
	
}

void CBlock::rotate()
{
	Point p = smallBlock[1];

	for (int i = 0; i < 4; i++)
	{
		Point tmp = smallBlock[i];
		smallBlock[i].col = p.col - tmp.row + p.row;
		smallBlock[i].row = p.row + tmp.col - p.col;
	}
}

void CBlock::move(int offset)
{
	for (int i = 0; i < 4; i++)
	{
		smallBlock[i].col += offset;
	}
}

IMAGE** CBlock::getimages()
{
	return imgs;
}

CBlock& CBlock::operator=(const CBlock& other)
{
	if (this == &other) return *this;

	this->blocktype = other.blocktype;
	
	for (int i = 0; i < 4; i++)
	{
		this->smallBlock[i] = other.smallBlock[i];	
	}
	return *this;
}

bool CBlock::blockinmap(const vector<vector<int>>& map)
{
	int rows = map.size();
	int cols = map[0].size();
	for (int i = 0; i < 4; i++)
	{
		if (smallBlock[i].col < 0 || smallBlock[i].row < 0 ||
			smallBlock[i].col >= cols || smallBlock[i].row >= rows || 
			map[smallBlock[i].row][smallBlock[i].col])
			return false;
	}

	return true;
}

void CBlock::solidify(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++)
	{
		map[smallBlock[i].row][smallBlock[i].col] = blocktype;
	}
}

// CBlock::~CBlock()
//{
//	delete img;
//	delete[] imgs;
//}
