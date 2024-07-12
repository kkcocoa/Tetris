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
	void init_game(); // 加载游戏的资源

private:
	void starter(IMAGE background);    // 加载开始界面
	void play();
	int gettime(); // 用来获取时差
	void refresh(); // 更新屏幕
	void drop(); // 方块下落
	void keyevent(); // 检测按键
	void clearline(); 
	void Movelr(int offset);
	void rotate();
	void drawScore(); // 画出得分
	void checkover(); // 检查游戏是否结束
	void savescore();
	void displayOver(); // 结束画面
	bool inarea(int mx, int my, int x, int y, int w, int h);
	bool button(int x, int y, int w, int h, const char* str); // 制作按钮
	void levelwidget();
	void settext(int x, int y, int w, int h, const char* str); // 设置界面上的文本
	void overwidget();
	void pausewidget();
	
private:
	// 刷新计时
	int delay;
	int delay_view;
	int timer;
	// 载入进度条
	void showProgressBar(int progress, int total);
	// 更新标记
	bool update;
	int score;
	int level;
	int linecount;
	int highscore;
	// 创建窗口尺寸
	int row, col, left_margin, top_margin, block_size;
	// 游戏结束标记
	bool gameover;
	bool gamestart;
	bool setlevel; // 设置难度标记
	bool setover; // 菜单结束
	bool ispause;
	// 0 为背景 5 为俄罗斯方块
	vector<vector<int> > map;
	//  当前方块和下一个方块
	CBlock* current_block;
	CBlock* next_block;
	CBlock bak_block; // 用来备份下降的方块
	// 正常，结束，赢游戏的图片
	IMAGE imgbg;
	IMAGE over;
	IMAGE win;
	IMAGE background;
	ExMessage msg;  // 接受消息
};

