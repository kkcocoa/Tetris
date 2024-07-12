#include "CTetrimino.h"
#include <time.h>
#include <stdlib.h>
#include <graphics.h>
#include <iostream>
#include <vector>
#include "CBlock.h"
#include <conio.h>
#include <mmsystem.h>
#include <fstream>
#include <chrono>
#include <thread>

#pragma comment(lib, "winmm.lib") 

#define high_score 300

const int Hard = 100;
const int speed_swi[5] = { 1000, 800, 500, 300, 100 };	 

CTetrimino::CTetrimino(int row, int col, int left_margin, int top_margin, int blocksize)
{
	this->row = row;
	this->col = col;
	this->left_margin = left_margin;
	this->top_margin = top_margin;
	block_size = blocksize;
	
	for (int i = 0; i < row; i++)
	{
		vector<int> maprow;
		for (int j = 0; j < col; j++)
			maprow.push_back(0);
		map.push_back(maprow);
	}
}
// ������Ϸ����ĳ�ʼ�� ���캯�����ǶԴ��ںͷ���������
void CTetrimino::init()
{
	mciSendString("play images/bg.mp3 repeat", 0, 0, 0);

	delay = speed_swi[0];
	// �����������
	srand(time(NULL));
	// ���ر���
	loadimage(&imgbg, "images/bg2.png");
	loadimage(&over, "images/over.png");
	loadimage(&win, "images/win.png");
	score = 0;
	linecount = 0;
	// ���δ�����Ѷ���Ĭ���Ѷ�Ϊ1
	if (level == 0) level = 1;
	gameover = false;
	gamestart = false;
	this->msg = { 0 };
	// ��ʼ����ͼ����
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			map[i][j] = 0;
	}
	// ������߷�
	ifstream file("./record.txt");
	if (!file.is_open())
	{
		cout << "Error" << endl;
		highscore = 0;
	}
	else
	{
		file >> highscore;
	}
	file.close();
}
// ��Ϸ�����������
void CTetrimino::play()
{
	init();
	// ������ʱ��
	timer = 0;

	next_block = new CBlock;
	current_block = next_block;
	next_block = new CBlock;

	while (1)
	{
		timer += gettime();
		//  �ȴ��û�����
		keyevent();
		// ����update���뷨�ǿ��������������и��ģ��������keyevent������updateʹ��ֱ�Ӹ���
		if (timer > delay)
		{
			update = true;
			// ÿһ��ˢ�£����鶼Ҫ�½�����������timer
			drop();
			timer = 0;
		}

		if (update)
		{
			update = false;
			
			// ˢ����Ϸ����
			refresh();
			// ˢ����Ϸ����
			clearline();
		}
		
		if (gameover)
		{
			// �������
			savescore();
			//������Ϸ����
			displayOver();
			settextstyle(50, 0, "���Ĳ���");
			outtextxy(275, 700, "�������������");
			system("pause");
			
			pausewidget();
		}
	}
}

int CTetrimino::gettime()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0)
	{
		lastTime = currentTime;
		return 0;
	}
	else
	{
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void CTetrimino::refresh()
{
	
	//  ���Է��� 
	//CBlock block;
	//block.draw(263, 133);

	IMAGE** imgs = CBlock::getimages();
	BeginBatchDraw();
	// ���ñ��� ͼƬ
	putimage(0, 0, &imgbg);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (map[i][j] == 0) continue;

			int x = j * block_size + left_margin;
			int y = i * block_size + top_margin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}
	current_block->draw(left_margin, top_margin);
	next_block->draw(689, 150);

	drawScore();

	EndBatchDraw();
}

void CTetrimino::drop()
{
	bak_block = *current_block;
	current_block->drop();

	if (!current_block->blockinmap(map))
	{
		bak_block.solidify(map);
		delete current_block;
		current_block = next_block;
		next_block = new CBlock;

		checkover();
	}
	delay = speed_swi[level - 1];
}

void CTetrimino::keyevent()
{
	ispause = false;
	char ch; 
	bool isrotate = false;
	int dx = 0;
	
	if (_kbhit()) // ��ⰴ��
	{
		ch = _getch();
		if (ch == -32) // ���·������ϵͳ�᷵������ֵ��һ��ֵΪ-32
		{
			ch = _getch();
			switch (ch)
			{
			case 72: // ����
				isrotate = true;
				break;
			case 80: // ���� ��������
				delay = Hard;
				break;
			case 75: // ����
				dx = -1; // �������ұ�����һ����ֻ��ƫ������ͬ
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}

			if (isrotate)
			{
				rotate();
				update = true;
				// ��ת����
			}
			if (dx)
			{
				Movelr(dx);
				update = true;
			}
		}
		if (ch == 27)
		{
			ispause = true;
		}

		if (ispause)
		{
			pausewidget();
			ispause = false;
		}
		
	}
}

void CTetrimino::clearline()
{
	int lines = 0;
	int k = row - 1;
	for (int i = row - 1; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < col; j++)
		{
			if (map[i][j])
			{
				count++;
			}
			map[k][j] = map[i][j];
		}

		if (count < col)
			k--;
		else {
			
			lines++;
		}
		
	}

	if (lines > 0)
	{
		// ����÷�
		int adds[4] = { 10, 30, 60, 100 };
		score += adds[lines - 1];

		mciSendString("play images/xiaochu2.mp3", 0, 0, 0);
		update = true;

		if (score > 300)
		{
			gameover = true;
		}
		linecount += lines;
	}
}

void CTetrimino::Movelr(int offset)
{
	bak_block = *current_block;
	current_block->move(offset);
	if (!current_block->blockinmap(map))
	{
		*current_block = bak_block;
	}
}

void CTetrimino::rotate()
{
	bak_block = *current_block;
	current_block->rotate();

	if (!current_block->blockinmap(map))
	{
		*current_block = bak_block;
	}
}

void CTetrimino::drawScore()
{
	char scoretext[32];
	sprintf_s(scoretext, sizeof(scoretext), "%d", score);
	setcolor(RGB(180, 180, 180));

	LOGFONT f;
	gettextstyle(&f);
	f.lfWeight = 30;
	f.lfHeight = 60;
	f.lfQuality = ANTIALIASED_QUALITY;
	//  ���Ʒ���
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);

	setbkmode(TRANSPARENT);
	outtextxy(670, 727, scoretext);
	// ���������˶�����
	sprintf_s(scoretext, sizeof(scoretext), "%d", linecount);
	gettextstyle(&f);
	outtextxy(224 - 30, 817, scoretext);

	// ���Ƶڼ���
	sprintf_s(scoretext, sizeof(scoretext), "%d", level);
	outtextxy(224 - 30, 727, scoretext);
	// ������߷�
	sprintf_s(scoretext, sizeof(scoretext), "%d", highscore);
	outtextxy(670, 817, scoretext);
}

void CTetrimino::checkover()
{
	gameover = (current_block->blockinmap(map) == false);
}

void CTetrimino::savescore()
{
	if (score > highscore)
	{
		highscore = score;

		ofstream file("./record.txt");
		file << highscore;
		file.close();
	}
}

void CTetrimino::displayOver()
{
	mciSendString("stop images/bg.mp3", 0, 0, 0);
	if (score <= 300)
	{
		putimage(262, 361, &over);
		mciSendString("play images/over.mp3", 0, 0, 0);
	}
	else
	{
		putimage(262, 361, &win);
		mciSendString("play images/win.mp3", 0, 0, 0);
	}
}

bool CTetrimino::inarea(int mx, int my, int x, int y, int w, int h)
{
	if (mx > x && mx<x + w && my>y && my < y + h) {
		return true;
	}
	return false;
}

bool CTetrimino::button(int x, int y, int w, int h, const char* str)
{
	if (inarea(msg.x, msg.y, x, y, w, h)) {
		setfillcolor(RGB(197, 147, 99));
	}
	else {
		setfillcolor(RGB(91, 165, 133));
	}
	fillroundrect(x, y, x + w, y + h, 50, 50);//���ƾ���
	settextstyle(60, 0, "���Ĳ���");
	int cx, cy;//ʹ���־���
	cx = (w - textwidth(str)) / 2;
	cy = (h - textheight(str)) / 2;
	settextcolor(BLACK);
	outtextxy(x + cx, y + cy, str);
	if (msg.message == WM_LBUTTONDOWN && inarea(msg.x, msg.y, x, y, w, h)) {
		return true;
	}
	return false;
}

void CTetrimino::levelwidget()
{
	bool isreturn = false;
	IMAGE l_image;
	loadimage(&l_image, "images/s6.jpeg", 938, 896); //��ͼƬ�ļ���ȡͼ��
	setbkmode(TRANSPARENT);//���ñ�����ģʽ
	
	while (true)
	{
		peekmessage(&msg, EX_MOUSE);//��ȡ�����Ϣ
		BeginBatchDraw();

		putimage(0, 0, &l_image);
		settext(310, 150, 370, 80, "�����Ѷ�");
		if (button(330, 280, 300, 70, "LEVEL_1")) {
			isreturn = true;
			level = 1;
		}
		else if (button(330, 380, 300, 70, "LEVEL_2"))
		{
			isreturn = true;
			level = 2;
		}
		else if (button(330, 480, 300, 70, "LEVEL_3"))
		{

			isreturn = true;
			level = 3;
		}
		else if (button(330, 580, 300, 70, "LEVEL_4"))
		{

			isreturn = true;
			level = 4;
		}
		else if (button(330, 680, 300, 70, "LEVEL_5"))
		{
			isreturn = true;
			level = 5;
		}
		else if (button(330, 780, 300, 70, "����"))
		{
			isreturn = true;
		}

		EndBatchDraw();
		msg.message = 0;

		if (isreturn)
		{
			setlevel = false;
			return;
		}
	}
	getchar();
}

void CTetrimino::settext(int x, int y, int w, int h, const char* str)
{
	settextstyle(100, 0, "΢���ź�");
	int cx, cy;//ʹ���־���
	cx = (w - textwidth(str)) / 2;
	cy = (h - textheight(str)) / 2;
	settextcolor(RGB(182, 91, 54));
	outtextxy(x + cx, y + cy, str);
}

void CTetrimino::overwidget()
{
	bool iscancel = false;
	bool isout = false;
	IMAGE l_image;
	loadimage(&l_image, "images/s6.jpeg", 938, 896); //��ͼƬ�ļ���ȡͼ��
	setbkmode(TRANSPARENT);//���ñ�����ģʽ

	while (true)
	{
		peekmessage(&msg, EX_MOUSE);//��ȡ�����Ϣ
		BeginBatchDraw();

		putimage(0, 0, &l_image);
		settext(335, 150, 370, 80, "�Ƿ��˳���");
		if (button(330, 480, 150, 70, "��")) {
			isout = true;
		}
		else if (button(480, 480, 150, 70, "��")) {
			iscancel = true;
		}
		

		EndBatchDraw();
		msg.message = 0;

		if (iscancel)
		{
			setover = false;
			return;
		}
		if (isout)
			exit(0);
	}
	getchar();
}

void CTetrimino::pausewidget()
{
	timer = 0;
	bool isbackbegin = false;
	mciSendString("stop images/bg.mp3", 0, 0, 0);

	while (true)
	{
		timer += gettime();

		peekmessage(&msg, EX_MOUSE);//��ȡ�����Ϣ
		if (timer > delay_view)
		{
			BeginBatchDraw();
			cleardevice();
			putimage(0, 0, &background);
			settext(280, 150, 370, 80, "��ͣ");
			if (button(310, 333, 300, 70, "������Ϸ")) {
				mciSendString("play images/bg.mp3 repeat", 0, 0, 0);
				return;
			}
			else if (button(310, 503, 300, 70, "����������"))
			{
				isbackbegin = true;
			}
			else if (button(310, 673, 300, 70, "�˳���Ϸ"))
			{
				setover = true;
			}

			if (isbackbegin)
			{
				starter(background);
				ispause = false;
				return;
			}
			if (setover)
			{
				overwidget();
				ispause = false;
				return;
			}

			EndBatchDraw();
			msg.message = 0;
		}
		
	}
	getchar();
}

void CTetrimino::showProgressBar(int progress, int total)
{
	const int barWidth = 50;

	float ratio = static_cast<float>(progress) / total;
	int barLength = static_cast<int>(ratio * barWidth);

	
}

void CTetrimino::starter(IMAGE background)
{
	timer = 0;
	// cleardevice();//��������
	while (true)
	{
		timer += gettime();

		peekmessage(&msg, EX_MOUSE | EX_KEY);//��ȡ�����Ϣ
		if (timer > delay_view)
		{
			BeginBatchDraw();

			putimage(0, 0, &background);
			settext(310, 150, 370, 80, "����˹����");
			if (button(330, 433, 300, 70, "��ʼ��Ϸ")) {
				gamestart = true;
			}
			else if (button(330, 583, 300, 70, "�Ѷ�"))
			{
				setlevel = true;
			}
			else if (button(330, 738, 300, 70, "�˳���Ϸ"))
			{
				setover = true;
			}

			EndBatchDraw();
			msg.message = 0;

			if (gamestart)
			{
				for (int i = 1; i <= 9; i++)
				{
					cleardevice();
					putimage(0, 0, &background);
					settext(430, 383, 100, 20, "Loading");
					if (i % 3 == 1)
					{
						settext(590, 383, 100, 10, ".");
					}
					else if (i % 3 == 2)
					{
						settext(590, 383, 100, 10, ".");
						settext(605, 383, 100, 10, ".");
					}
					else {
						settext(590, 383, 100, 10, ".");
						settext(605, 383, 100, 10, ".");
						settext(620, 383, 100, 10, ".");
					}
					// �ȴ����ص�ʱ��ȴ�
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				play();
				gamestart = false;

			}
			else if (setlevel)
			{
				levelwidget();
				setlevel = false;
			}
			else if (setover)
			{
				overwidget();
				setover = false;
			}
		}
		
	}
	getchar();
}


void CTetrimino::init_game()
{
	initgraph(938, 896);//����һ������
	background;//����һ��ͼƬ��.
	loadimage(&background, "images/s6.jpeg", 938, 896); //��ͼƬ�ļ���ȡͼ��
	setbkmode(TRANSPARENT);//���ñ�����ģʽ
	delay_view = 500;
	starter(background);
}

