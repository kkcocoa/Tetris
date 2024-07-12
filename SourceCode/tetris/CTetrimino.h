#pragma once
#include <graphics.h>
#include <vector>
#include "CBlock.h"
using namespace std;

class CTetrimino
{
public:
	CTetrimino(int row, int col, int left_margin, int top_margin, int blocksize);
	void init();
	void init_game(); // ������Ϸ����Դ

private:
	void starter(IMAGE background);    // ���ؿ�ʼ����
	void play();
	int gettime(); // ������ȡʱ��
	void refresh(); // ������Ļ
	void drop(); // ��������
	void keyevent(); // ��ⰴ��
	void clearline(); 
	void Movelr(int offset);
	void rotate();
	void drawScore(); // �����÷�
	void checkover(); // �����Ϸ�Ƿ����
	void savescore();
	void displayOver(); // ��������
	bool inarea(int mx, int my, int x, int y, int w, int h);
	bool button(int x, int y, int w, int h, const char* str); // ������ť
	void levelwidget();
	void settext(int x, int y, int w, int h, const char* str); // ���ý����ϵ��ı�
	void overwidget();
	void pausewidget();
	
private:
	// ˢ�¼�ʱ
	int delay;
	int delay_view;
	int timer;
	// ���������
	void showProgressBar(int progress, int total);
	// ���±��
	bool update;
	int score;
	int level;
	int linecount;
	int highscore;
	// �������ڳߴ�
	int row, col, left_margin, top_margin, block_size;
	// ��Ϸ�������
	bool gameover;
	bool gamestart;
	bool setlevel; // �����Ѷȱ��
	bool setover; // �˵�����
	bool ispause;
	// 0 Ϊ���� 5 Ϊ����˹����
	vector<vector<int> > map;
	//  ��ǰ�������һ������
	CBlock* current_block;
	CBlock* next_block;
	CBlock bak_block; // ���������½��ķ���
	// ������������Ӯ��Ϸ��ͼƬ
	IMAGE imgbg;
	IMAGE over;
	IMAGE win;
	IMAGE background;
	ExMessage msg;  // ������Ϣ
};

