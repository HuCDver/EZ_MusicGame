/*接下来的修改：用msgbox来显示hint和info
用光标来标示选中的曲子或选项
增加音效
使用putImageTransparent来显示有透明背景的图片*/

#include"acllib.h"
#include<time.h>
#include<math.h>
#include<stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")

#define winWid 996
#define winHei 560
#define charSize 50
#define REALcharSize 50
#define buttonDtn 40
#define sl_song 10
#define sl_name 20
#define MTextS 20
#define MaxNum 8
#define RECORD_RAPID 15
#define CubeWid 160
#define CubeHei 99
#define CubePo_x 486
#define CubePo_y -116
#define Lcube_up -116
#define Lcube_down 676
#define Line_y 380

void paintBG(int control);
void SuperClear(void);
void MoveCube(void);
void PaintCube(int control,int i);
void PaintLine(int control);
void HitWatch(int control);
//功能函数

void MouseEvent1(int x, int y, int button, int event);
void START_menu(void);
void MUSIC_menu(void);
void PLAY_menu(void);
void HINT_menu(void);
void INFO_menu(void);
void QUIT_menu(void);
//开始菜单：鼠标控制，菜单函数

void MouseEvent2(int x, int y, int button, int event);
void SELECT_menu(void);
void RECORD_menu(void);
void KeyboardEventS(int key, int event);
void MouseEventS(int x, int y, int button, int event);
void MouseEventR(int x, int y, int button, int event);
void TimeEvent(int ID);
void KeyboardEventR(int key, int event);
void MusicList();
void PaintMusicList();
void MusicControl(const char* SONG);
void PaintSelectList(int s);
void MusicLoad();
void MusicInfo(char* SONG);
void MusicRecord(char* SONG);
//音乐菜单：鼠标控制，菜单函数，小范围键鼠和时间控制，功能函数

void GO_menu(void);
void MouseEvent3(int x, int y, int button, int event);
void MusicGo(char* SONG);
void MouseEventG(int x, int y, int button, int event);
void KeyboardEventG(int key, int event);
void MusicPlay(char* SONG);
void PaintScore(void);

ACL_Image START_Background;
ACL_Image START_Title;
ACL_Image MUSIC_Background;
ACL_Image PLAY_Background;
ACL_Image CubeN;
ACL_Image CubeR;
ACL_Image CubeH;
ACL_Sound snd1;

const char* mMenuB[5] = { "MUSIC", "PLAY", "HINT", "INFO", "QUIT" };
const char* uMenuB[3] = { "SELECT", "RECORD", "BACK" };
const char* pMenuB[3] = { "LOAD", "GO", "BACK" };

char ChosenM[sl_name];
char songlist[sl_song][sl_name];
int rangel = 0;
int selects = 0;
int mlen = 0;
FILE* fpR = NULL;
FILE* fpG = NULL;
int score=0;
char FileN[50];

struct cubee
{
	int cubeTYPE;	//0为normal，1为ready，2为hit
	int width, height;
	int x, y;
};
int dis = 7;
struct cubee* CubeNRH[MaxNum] = { NULL };

int RecordT=0;
int ReadT[2] = { 0,0 };

int Setup()
{
	initWindow("test", 0, 0, winWid, winHei);
	loadSound("tada.wav", &snd1);
	loadImage("START_Background.jpg", &START_Background);
	loadImage("MUSIC_Background.jpg", &MUSIC_Background);
	loadImage("PLAY_Background.jpg", &PLAY_Background);
	loadImage("CubeNormal.jpg", &CubeN);
	loadImage("CubeReady.jpg", &CubeR);
	loadImage("CubeHit.jpg", &CubeH);

	
	for (int i = 0; i < MaxNum; i++)
	{
		CubeNRH[i] = (cubee*)malloc(sizeof(cubee));
		CubeNRH[i]->cubeTYPE = 0;
		CubeNRH[i]->width = CubeWid;
		CubeNRH[i]->height = CubeHei;
		CubeNRH[i]->x = CubePo_x;
		CubeNRH[i]->y = CubePo_y+ i*CubeNRH[i]->height;
	}


	START_menu();

	return 0;
}


void SuperClear()
{
	beginPaint();
	clearDevice();
	endPaint();
}

void paintBG(int control)
{
	switch (control)
	{
	case 0:
	{
		beginPaint();
		putImage(&START_Background, 0, 0);
		endPaint();
		break;
	}
	case 1:
	{
		beginPaint();
		putImage(&MUSIC_Background, 0, 0);
		endPaint();
		break;
	}
	case 2:
	{
		beginPaint();
		putImage(&PLAY_Background, 0, 0);
		endPaint();
		break;
	}
	default:
		break;
	}
}

void PaintCube(int control,int i)
{
	switch (control)
	{
	case	0:
		beginPaint();
		putImage(&CubeN, CubeNRH[i]->x, CubeNRH[i]->y);
		endPaint();
		break;
	case	1:
		beginPaint();
		putImage(&CubeR, CubeNRH[i]->x, CubeNRH[i]->y);
		endPaint();
		break;
	case	2:
		beginPaint();
		putImage(&CubeH, CubeNRH[i]->x, CubeNRH[i]->y);
		endPaint();
		break;
	default:
		break;
	}
}

void MoveCube(void)
{
	for (int i = 0; i < MaxNum; i++)
	{
		CubeNRH[i]->y += dis;
		if (CubeNRH[i]->y>=Lcube_down)
		{
			CubeNRH[i]->y = Lcube_up;
			CubeNRH[i]->cubeTYPE = 0;
		}
	}
}

void PaintLine(int control)
{
	switch (control)
	{
	case 0:
		beginPaint();
		setPenColor(WHITE);
		line(0, Line_y, winWid, Line_y);
		endPaint();
		break;
	case 1:
		beginPaint();
		setPenColor(GREEN);
		line(0, Line_y, winWid, Line_y);
		endPaint();
		break;
	default:
		break;
	}
}

void START_menu()
{
	playSound(snd1, 0);
	SuperClear();
	paintBG(0);
	int i;
	beginPaint();
	setTextSize(charSize);
	setTextColor(WHITE);
	setTextBkColor(EMPTY);
	for (i = 0; i < 5; i++)
	{
		paintText(50, 50 + i * charSize + i * buttonDtn, mMenuB[i]);
	}
	endPaint();
	registerKeyboardEvent(NULL);
	registerMouseEvent(MouseEvent1);
}

void MouseEvent1(int x, int y, int button, int event)
{
	if (button == LEFT_BUTTON && event == BUTTON_DOWN) {
		if (x >= 50 && x <= 50 + 3 * REALcharSize)
		{
			if (y >= 50 && y <= 50 + 5 * (REALcharSize + buttonDtn) + REALcharSize)
			{
				if (y <= 50 + 1 * REALcharSize +0*buttonDtn)
				{
					MUSIC_menu();
				}
				else
				{
					if (y<=50+2* REALcharSize +1*buttonDtn)
					{
						PLAY_menu();
					}
					else
					{
						if (y<=50+3* REALcharSize +2*buttonDtn)
						{
							HINT_menu();
						}
						else
						{
							if (y<=50+4* REALcharSize +3*buttonDtn)
							{
								INFO_menu();
							}
							else
							{
								QUIT_menu();
							}
						}
					}
				}
			}
		}
	}
}

void HINT_menu()
{
	beginPaint();
	setTextColor(WHITE);
	setTextBkColor(GREEN);
	setTextSize(25);
	paintText(300, 200, "click to choose option");
	paintText(300, 225, "before playing should select and record first");
	paintText(300, 250, "tap UP/DOWN and ENTER to select music");
	paintText(300, 275, "when done with selection click once and tap ESC");
	endPaint();
}

void INFO_menu()
{
	beginPaint();
	setTextColor(WHITE);
	setTextBkColor(GREEN);
	setTextSize(30);
	paintText(350, 500, "developer:HuCDver");
	endPaint();
}
void QUIT_menu()
{
	exit(0);
}

void PLAY_menu(void)
{
	int i;
	playSound(snd1, 0);
	SuperClear();
	paintBG(2);
	beginPaint();
	setTextSize(charSize);
	setTextColor(WHITE);
	setTextBkColor(EMPTY);
	for (i = 0; i < 3; i++)
	{
		paintText(50, 50 + i * charSize + i * buttonDtn, pMenuB[i]);
	}
	endPaint();
	registerKeyboardEvent(NULL);
	registerMouseEvent(MouseEvent3);
}

void MouseEvent3(int x, int y, int button, int event)
{
	if (button == LEFT_BUTTON && event == BUTTON_DOWN) {
		if (x >= 50 && x <= 50 + 3 * REALcharSize)
		{
			if (y >= 50 && y <= 50 + 5 * (REALcharSize + buttonDtn) + REALcharSize)
			{
				if (y <= 50 + 1 * REALcharSize + 0 * buttonDtn)
				{
					SELECT_menu();
				}
				else
				{
					if (y <= 50 + 2 * REALcharSize + 1 * buttonDtn)
					{
						GO_menu();
					}
					else
					{
						if (y <= 50 + 3 * REALcharSize + 2 * buttonDtn)
						{
							START_menu();
						}
					}
				}
			}
		}
	}
}

void GO_menu()
{
	RecordT = 0;
	SuperClear();
	paintBG(2);
	MusicGo(ChosenM);

}

void MUSIC_menu()
{
	playSound(snd1, 0);
	SuperClear();
	paintBG(1);
	int i;
	beginPaint();
	setTextSize(charSize);
	setTextColor(WHITE);
	setTextBkColor(EMPTY);
	for (i = 0; i < 3; i++)
	{
		paintText(50, 50 + i * charSize + i * buttonDtn, uMenuB[i]);
	}
	endPaint();
	registerKeyboardEvent(NULL);
	registerTimerEvent(TimeEvent);
	registerMouseEvent(MouseEvent2);
}

void MouseEvent2(int x, int y, int button, int event)
{
	if (button == LEFT_BUTTON && event == BUTTON_DOWN) {
		if (x >= 50 && x <= 50 + 3 * REALcharSize)
		{
			if (y >= 50 && y <= 50 + 5 * (REALcharSize + buttonDtn) + REALcharSize)
			{
				if (y <= 50 + 1 * REALcharSize + 0 * buttonDtn)
				{
					SELECT_menu();
				}
				else
				{
					if (y <= 50 + 2 * REALcharSize + 1 * buttonDtn)
					{
						RECORD_menu();
					}
					else
					{
						if (y <= 50 + 3 * REALcharSize + 2 * buttonDtn)
						{
							START_menu();
						}
					}
				}
			}
		}
	}
}

void MusicGo(char* SONG)
{
	fpR = NULL;
	fpG = NULL;
	FileN[0] = '\0';
	int i;
	strcpy_s(FileN, SONG);
	for (i = 0; i < 50 && FileN[i] != '\0'; i++)
	{
		if (FileN[i] == '\r' || FileN[i] == '\n')
			FileN[i] = 'o';
	}
	strcat_s(FileN, 50, ".txt");
	registerMouseEvent(MouseEventG);
	registerTimerEvent(TimeEvent);
	registerKeyboardEvent(KeyboardEventG);
	startTimer(3, 10);
	startTimer(2, 10);
	MusicPlay(ChosenM);
}

void MouseEventG(int x, int y, int button, int event)
{
	if (button == LEFT_BUTTON && event == BUTTON_DOWN)
	{	
		if (y >= 300)
		{
			cancelTimer(2);
			cancelTimer(3);
			mciSendString("stop OnMusic", NULL, 0, NULL);
			mciSendString("close OnMusic", NULL, 0, NULL);
			PLAY_menu();
		}
	}
}

void KeyboardEventG(int key, int event)
{
	if (key == VK_SPACE && event == KEY_DOWN)
	{
		HitWatch(1);
	}
}

void SELECT_menu()
{
	MusicList();
	MusicLoad();
}

void MusicPlay(char* SONG)
{
	char string[50] = "open ";
	strcat_s(string, 50, SONG);		//这里可以用sprintf替换
	for (int i = 0; i < 50 && string[i] != '\0'; i++)
	{
		if (string[i] == '\r' || string[i] == '\n')
			string[i] = ' ';
	}
	strcat_s(string, 50, "alias OnMusic");
	mciSendString(string, NULL, 0, NULL);
	//打开音乐
	mciSendString("play OnMusic repeat", NULL, 0, NULL);
	//播放音乐
}

void MusicControl(char* SONG)
{
	MusicPlay(SONG);
	registerMouseEvent(MouseEventS);
}

void MouseEventS(int x, int y, int button, int event)
{
	if (button == LEFT_BUTTON && event == BUTTON_DOWN)
	{
		mciSendString("stop OnMusic", NULL, 0, NULL);
		mciSendString("close OnMusic", NULL, 0, NULL);
	}
}

void MusicList()
{
	rangel = 0;
	int j;
	for (j = 0; j < sl_song; j++)
	{
		songlist[j][0] = '\0';
	}
	FILE* fp = NULL;
	errno_t err;
	err = fopen_s(&fp, "songlist.txt", "w+");	//清空文件
	fclose(fp);
	fp = NULL;
	system("dir /b *.mp3 >> songlist.txt");
	err = fopen_s(&fp, "songlist.txt", "rb");
	while (fgets(songlist[rangel], sl_name, fp))
	{
		rangel++;
	}
	fclose(fp);
	PaintMusicList();
}

void PaintMusicList()
{
	beginPaint();
	setTextSize(MTextS);
	setTextColor(WHITE);
	setTextBkColor(EMPTY);
	for (int j = 0; j < rangel; j++)
	{
		paintText(300, 50 + MTextS * j, songlist[j]);
	}
	endPaint();
}

void PaintSelectList(int s)
{
	PaintMusicList();
	beginPaint();
	setTextSize(MTextS);
	setTextColor(GREEN);
	setTextBkColor(EMPTY);
	paintText(300, 50 + MTextS * s, songlist[s]);
	endPaint();
}

void MusicLoad()
{
	selects = 0;
	PaintSelectList(selects);
	registerKeyboardEvent(KeyboardEventS);
}

void KeyboardEventS(int key, int event)
{
	if (event == KEY_DOWN)
	{
		switch (key)
		{
		case	VK_UP:
		{
			if (selects <= 0)
			{
				PaintSelectList(selects);
			}
			else
			{
				selects--;
				PaintSelectList(selects);
			}
			break;
		}
		case    VK_DOWN:
		{
			if (selects >= rangel - 1)
			{
				PaintSelectList(selects);
			}
			else
			{
				selects++;
				PaintSelectList(selects);
			}
			break;
		}
		case	VK_RETURN:
		{
			strcpy_s(ChosenM, songlist[selects]);
			MusicControl(ChosenM);
			break;
		}
		case	VK_ESCAPE:
		{
			MusicInfo(ChosenM);
			MUSIC_menu();
			break;
		}
		default:
			break;
		}
	}
	else
	{
		return;
	}
}

void RECORD_menu(void)
{
	SuperClear();
	paintBG(1);
	MusicRecord(ChosenM);

}
void MusicInfo(char* SONG)	//获得曲子长度mlen（毫秒）
{
	RecordT = 0;
	mlen = 0;
	char string[50] = "status ";
	char len[12];
	int temp_int;
	char temp_char;
	int a;
	int i;
	strcat_s(string, 50, SONG);
	for ( i = 0; i < 50 && string[i] != '\0'; i++)
	{
		if (string[i] == '\r' || string[i] == '\n')
			string[i] = ' ';
	}
	strcat_s(string, 50, "length");
	mciSendString(string, len, strlen(len), 0);
	for ( i = 0; i < strlen(len); i++)
	{
		temp_char = len[i];
		temp_int = temp_char - '0';
		a = (int)pow(10, strlen(len) - i-1);
		mlen += temp_int * a;
	}
}

void MusicRecord(char* SONG)
{
	fpR = NULL;
	FileN[0]='\0';
	int i;
	strcpy_s(FileN, SONG);
	for (i = 0; i < 50 && FileN[i] != '\0'; i++)
	{
		if (FileN[i] == '\r' || FileN[i] == '\n')
			FileN[i] = 'o';
	}
	strcat_s(FileN, 50, ".txt");
	errno_t dd;
	dd = fopen_s(&fpR, FileN, "w+");

	//测试一接口处，因为部分调整无法完全对接，可手动微调
	//创建以曲名+“oo.txt”命名的txt文件，作为曲谱的记录文件
	//接下来以一定的时间间隔记录键盘的按键（空格或无）
	//经测试一，上条难以完成，改用记录按键发生的时间来表示节奏

	registerMouseEvent(MouseEventR);
	registerTimerEvent(TimeEvent);
	registerKeyboardEvent(KeyboardEventR);
	startTimer(1, 10);
	startTimer(2, 10);
	fclose(fpR);//
	MusicPlay(ChosenM);
}
void MouseEventR(int x, int y, int button, int event)
{
	if (button==LEFT_BUTTON && event==BUTTON_DOWN)
	{
		if (y >= 300)
		{
			cancelTimer(1);
			cancelTimer(2);
			mciSendString("stop OnMusic", NULL, 0, NULL);
			mciSendString("close OnMusic", NULL, 0, NULL);
			MUSIC_menu();
		}
	}
}

void TimeEvent(int ID)
{
	switch (ID)
	{
	case	0:
		//预留通道
		break;
	case	1:
		//计时用
		RecordT+=RECORD_RAPID;
		if (RecordT >= mlen)
		{
			cancelTimer(1);
			cancelTimer(2);
			mciSendString("stop OnMusic", NULL, 0, NULL);
			mciSendString("close OnMusic", NULL, 0, NULL);
			MUSIC_menu();
		}
		break;
	case	2:
		///move paint
		for (int i = 0; i < MaxNum; i++)
		{
			PaintCube(CubeNRH[i]->cubeTYPE, i);
		}
		MoveCube();
		PaintLine(0);
		break;
	case	3:
		RecordT += RECORD_RAPID;
		if (RecordT >= ReadT[0] - 567)//
		{
			errno_t dd;
			fpG = NULL;
			int ww=0;
			dd = fopen_s(&fpG, FileN, "r");
			while (RecordT >= ReadT[0] - 567)
			{
				if (fscanf_s(fpG, "%d", &ReadT[0]) == EOF)
				{
					ww = 1;
					break;
				}
			}
			fclose(fpG);
			for (int i = 0; (i < MaxNum) && (ww!=1); i++)
			{
				if (CubeNRH[i]->y <= -17)
				{
					CubeNRH[i]->cubeTYPE = 1;
				}
			}
		}
		if (RecordT >= mlen)
		{
			cancelTimer(3);
			cancelTimer(2);
			mciSendString("stop OnMusic", NULL, 0, NULL);
			mciSendString("close OnMusic", NULL, 0, NULL);
			fclose(fpR);
			PLAY_menu();
		}
		break;

	default:
		break;
	}
}

void KeyboardEventR(int key, int event)
{
	if (key==VK_SPACE && event==KEY_DOWN)
	{
		errno_t dd;
		dd = fopen_s(&fpR, FileN, "a");
		fprintf_s(fpR,"%d ", RecordT);
		fclose(fpR);
		HitWatch(0);
	}
}

void HitWatch(int control)
{
	switch (control)
	{
	case 0:
		///record
		for (int i = 0; i < MaxNum; i++)
		{
			if (CubeNRH[i]->y<Line_y && CubeNRH[i]->y + CubeHei>Line_y)
			{
				CubeNRH[i]->cubeTYPE = 1;
				PaintCube(CubeNRH[i]->cubeTYPE, i);
				PaintLine(1);
			}
		}
		break;
	case 1:
		///go
		for (int i = 0; i < MaxNum; i++)
		{
			if (CubeNRH[i]->y<Line_y && CubeNRH[i]->y + CubeHei>Line_y)
			{
				if (CubeNRH[i]->cubeTYPE == 1)
				{
					CubeNRH[i]->cubeTYPE = 2;
					PaintCube(CubeNRH[i]->cubeTYPE, i);
					score++;
					PaintScore();
				}
			}
		}
		break;
	default:
		break;
	}
}

void PaintScore(void)
{
	char string[50];
	beginPaint();
	setTextBkColor(GREEN);
	setTextColor(WHITE);
	sprintf_s(string, "%d", score);
	paintText(0, 0, string);
	endPaint();
}
	//以下是测试一的内容//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//隔一段时间检测一遍键盘输入？需要用到timeevent
	//timeevent隔一段时间（）进行一次（）事件；
	//发现键鼠的事件检测如果不用其他键鼠事件代替会一直存在
	//能否关闭特定检测？
	//能否用空事件填充检测？    可以填充，但是该空事件会一直发生，占用正在进行的程序段段，使程序停留在register...上
	//能否用timeevent控制检测的运行时间和类别？    可能：timeevent只负责隔一段时间“开始”事件，不负责关闭
	//timer是始终占据代码段，还是会在间隔时间时让位？让位给其他register还是所有程序段？
	////
	//starttimer不会读取到就立即执行，而会暂挂并执行后面的语句，直到其他代码段执行完后才会开始
	//这里的其他代码不包括键鼠检测     因为他们没有理论上的结束？
	//但这里的其他代码包括cancelTimer()
	//且这里其他代码段执行完毕后程序仍处于mouseevent2的控制范围
	//且register控制的进程可以同时执行    观察到timeevent和键鼠检测同时运行
	////
	//想法：可以通过事件检测转移执行代码段的位置，类似goto
	//任务：找方法把键鼠检测杀了    找方法把register控制的都杀了
	//考虑：键鼠检测如何监听键鼠信息？	starttimer如何暂挂、如何不停运行？
	//决定：弃用acllib，自己写一个（时间不够了，搁置
	//灵感：用鼠标检测控制timeevent的死活
	/*
	fclose(fpR);
	registerTimerEvent(TimeEvent);
	initConsole();
	startTimer(0, 1000);
	startTimer(1, 2000);
	if (RecordT == 10)
	{
		cancelTimer(0);
	}
	while (1)
	{
		if (getchar() == 'a') {

			cancelTimer(1);
			break;
		}
		if (getchar()=='b')
		{
			break;
		}
	}
	startTimer(2, 1000);
	cancelTimer(2);
}

void TimeEvent(int ID)
{
	if (ID == 0)	
	{
		RecordT++;
		printf_s("通道0运行\n");
	}
	if (ID == 1)
	{
		printf_s("通道1运行\n");
	}
	if (ID == 2)
	{
		printf_s("通道2运行\n");
	}

}

void MouseEventS(int x, int y, int button, int event)
{
	if (button == LEFT_BUTTON && event == BUTTON_DOWN)
	{
		mciSendString("stop OnMusic", NULL, 0, NULL);
		mciSendString("close OnMusic", NULL, 0, NULL);
	}
}

void KeyboardEventR(int key, int event)
{

}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////