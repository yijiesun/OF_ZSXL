#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include "include.h"
#define   NR(x)   (sizeof(x)/sizeof(x[0]+0))
#define  TITLE  "ZSXL复合感知系统"
#define  AUTHOR "版本:V1.0"
#define  DATE   "维护日期:2020年02月26日"
//定义枚举Keyboard的键值数据 
enum
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27,
};

//定义要显示的菜单 
char *menu[] =
{
	"＊1  翻越高墙              ＊",
	"＊2  通过壕坑              ＊",
	"＊3  通过独木桥            ＊",
	"＊4  独木桥绕桩            ＊",
	"＊5  跨越矮墙              ＊",
	"＊6  钻过矮墙              ＊",
	"＊7  爬越绳索              ＊",
	"＊8  通过低桩网            ＊",
	"＊9  隐蔽通过长壕          ＊",
	"＊10 通过长壕并观察        ＊",
	"＊11 高楼升降              ＊",
	"＊12 窗户探索              ＊",
	"＊13 单声枪【利用地形地物】＊",
	"＊14 连续枪【利用地形地物】＊",
	"＊15 退出                  ＊",
};
//定义结构体
CONSOLE_CURSOR_INFO cci;
//定义默认的坐标位置  	
COORD pos = { 0,0 };
//显示菜单 
void showmenu(HANDLE hOut, char **menu, int size, int index);
//获取用户输入 
int  get_userinput(int *index, int size);
int main()
{
	int i;
	int ret;
	int index = 0;
	HANDLE hOut;
	SetConsoleTitleA(TITLE);
	//获取当前的句柄---设置为标准输出句柄 
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//获取光标信息
	GetConsoleCursorInfo(hOut, &cci);
	//设置光标大小   
	cci.dwSize = 1;
	//设置光标不可见 FALSE   
	cci.bVisible = 0;
	//设置(应用)光标信息
	SetConsoleCursorInfo(hOut, &cci);
	while (1)
	{
		showmenu(hOut, menu, NR(menu), index);
		ret = get_userinput(&index, NR(menu));
		if (ret == ESC)
			break;
		if (ret == ENTER)
		{
			switch (index)
			{
			case 0:  RUN_FYGQ();         break;  	//翻越高墙
			case 1:						 break;  	//通过壕坑
			case 2:                      break;  	//通过独木桥
			case 3:                      break;  	//独木桥绕桩
			case 4:                      break;  	//跨越矮墙
			case 5:                      break;  	//钻过矮墙
			case 6:                      break;  	//爬越绳索
			case 7:                      break;  	//通过低桩网
			case 8:                      break;  	//隐蔽通过长壕
			case 9:                      break;  	//通过长壕并观察
			case 10:                     break;  	//高楼升降
			case 11:                     break;  	//窗户探索
			case 12:                     break;  	//单声枪【利用地形地物】
			case 13:                     break;  	//连续枪【利用地形地物】
			case 14:                     return 0; //退出
			}
		}
	}
	return 0;
}

void showmenu(HANDLE hOut, char **menu, int size, int index)
{
	int i;
	system("cls");
	//设置显示的文本的颜色 
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | 0x8);
	//初始化控制台显示的X,Y轴的坐标 
	pos.X = 30;
	pos.Y = 0;
	//设置显示到控制台终端的具体位置 
	SetConsoleCursorPosition(hOut, pos);
	//调用printf在控制台对应的位置上输出 
	printf("%s", TITLE);
	pos.X = 32;
	pos.Y = 1;
	SetConsoleCursorPosition(hOut, pos);
	printf("%s", AUTHOR);
	pos.X = 30;
	pos.Y = 2;
	SetConsoleCursorPosition(hOut, pos);
	printf("%s", DATE);
	for (i = 0; i < size; i++)
	{
		//如果i==index表示在当前选项的位置，默认初始化显示是第一项，显示为红色，
		//当按下上下按键选择的时候，光标会移动，也就看到了列表选择的现象 
		if (i == index)
		{
			//红色 
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | 0x8);
			pos.X = 30;
			pos.Y = 5 + i;
			//设置光标坐标
			SetConsoleCursorPosition(hOut, pos);
			printf("%s", menu[i]);
		}
		//否则显示为白色 
		else
		{
			//白色 
			SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			pos.X = 30;
			pos.Y = 5 + i;
			//设置光标坐标
			SetConsoleCursorPosition(hOut, pos);    //设置光标坐标
			printf("%s", menu[i]);
		}
	}
	//刷新标准输出缓冲区 
	fflush(stdout);
}

//获取用户输入的接口 
int  get_userinput(int *index, int size)
{
	int ch;
	ch = getch();
	switch (ch)
	{
		//上 
		//如果选择上，那么光标向上移动 
	case UP: if (*index > 0)  *index -= 1;  break;
		//下 
		//如果选择下，那么光标向下移动 
	case DOWN:if (*index < size - 1)  *index += 1;  break;
		//左 
	case LEFT:
	case 97:return 0;
		//右 
	case RIGHT:return 0;
		//回车 
	case ENTER: return ENTER;
		//ESC
	case ESC: return ESC;
	}
	return 0;
}