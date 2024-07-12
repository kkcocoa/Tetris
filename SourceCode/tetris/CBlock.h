#pragma once
#include <graphics.h> // ��Ҫ��װessyxͼ�ο�
#include <vector>
using std::vector;

struct Point {
	int row, col;
};

class CBlock
{
public:
	CBlock();
	void drop();
	void draw(int left_margin, int top_margin);
	void rotate();
	void move(int offset);
	static IMAGE** getimages();
	CBlock& operator=(const CBlock& other);
	bool blockinmap(const vector<vector<int>>& map);
	void solidify(vector<vector<int> >& map);
	//~CBlock();
private:
	int blocktype;
	Point smallBlock[4]; // �洢�������ĸ��������״
	IMAGE *img;
	static IMAGE* imgs[7];
	static int size;
};

