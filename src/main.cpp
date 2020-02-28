#include <stdio.h>
#ifdef win32
#include <Windows.h>
#include <conio.h>
#else

#include <termio.h>
#include <stdio.h>
#endif
#include <stdlib.h>
#include "include.h"
#include "functions.h"
#include "config.h"
#include <iostream> 

#define   NR(x)   (sizeof(x)/sizeof(x[0]+0))
#define  TITLE_MAIN  "ZSXL复合感知系统-主页面"
#define  TITLE_SUB  "ZSXL复合感知系统-子项目启动界面"
#define  TITLE_PARAM  "ZSXL复合感知系统-参数配置界面"
#define  AUTHOR "syj.V1.0"
#define  DATE   "维护日期:2020年02月26日"
//定义枚举Keyboard的键值数据 
#ifdef win32
enum
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27,
};
#else
enum
{
	UP = 87,
	UP_ = 119,
	DOWN = 83,
	DOWN_ = 115,
	LEFT = 65,
	LEFT_ = 97,
	RIGHT = 68,
	RIGHT_ = 100,
	ENTER = 13,
	ESC = 27,
};
#endif
//定义要显示的菜单 
char *subMenu[200] =
{
	"＊ 1  翻越高墙              ＊",
	"＊ 2  通过壕坑              ＊",
	"＊ 3  通过独木桥            ＊",
	"＊ 4  独木桥绕桩            ＊",
	"＊ 5  跨越矮墙              ＊",
	"＊ 6  钻过矮墙              ＊",
	"＊ 7  爬越绳索              ＊",
	"＊ 8  通过低桩网            ＊",
	"＊ 9  隐蔽通过长壕          ＊",
	"＊ 10 通过长壕并观察        ＊",
	"＊ 11 高楼升降              ＊",
	"＊ 12 窗户探索              ＊",
	"＊ 13 单声枪【利用地形地物】＊",
	"＊ 14 连续枪【利用地形地物】＊",
	"＊ 15 返回                  ＊",
};
char *mainMenu[200] =
{
	"＊ 1  启动项目              ＊",
	"＊ 2  参数配置              ＊",
	"＊ 3  退出                  ＊",
};
char *paramMenu[200] =
{
	"＊ 1  frame_downsampling_percent            ＊",
	"＊ 2  continues_frame_goal_cnt              ＊",
	"＊ 3  返回                                  ＊",
};

class Menu {
private:

	int paramLength;
	//char *paramList[200];
	float paramValueList[10];
	int menuStatus;//0-out;1-main;2-sub;3-param
	int ret;
	int mainIndex;
	int paramIndex;
	int subIndex;
	//HANDLE hOut;
	bool isOut;
public:
	Menu() {
		menuStatus = 1;
		mainIndex = 0;
		subIndex = 0;
		//pos = { 0,0 };
		isOut = false;
	};
	~Menu() {};
#ifdef win32
	//定义结构体
	CONSOLE_CURSOR_INFO cci;
	//定义默认的坐标位置  	
	COORD pos;
#endif
	//获取用户输入的接口 
	int  get_userinput(int *index, int size)
	{
		int ch;
	#ifdef win32
		ch = getch();
	#else
	    ch =scanKeyboard();
		cout<<ch<<endl;
	#endif
		switch (ch)
		{
			//上 
			//如果选择上，那么光标向上移动 
		case UP_:
		case UP: if (*index > 0)  *index -= 1;  break;
			//下 
			//如果选择下，那么光标向下移动 
		case DOWN_:
		case DOWN:if (*index < size - 1)  *index += 1;  break;
			//左 
		case LEFT_: return LEFT;
		case LEFT: return LEFT;
			//右 
		case RIGHT_:return ENTER;
		case RIGHT:return ENTER;
			//回车 
		case ENTER: return ENTER;
			//ESC
		case ESC: return ESC;
		}
		return 0;
	}

	void run()
	{
		const char * split = " ";
		char **paramList;
		paramList = (char **)malloc(sizeof(char *) * 200);
		for (int i = 0; i < 10; i++)
			paramList[i] = (char *)malloc(sizeof(char) * 200);

		Config cfg("..//config.txt");
		cfg.readConfig(paramLength, paramList, paramValueList);
		paramList[paramLength++] = "返回";

		float frame_downsampling_percent;
		int cnt;
#ifdef win32
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
#endif
		char *title_str = TITLE_MAIN;
		int menu_size = 0;
		char **menu = mainMenu;
		int *index = &mainIndex;
		while (!isOut)
		{
			//状态显示
			//0-out;1-main;2-sub;3-param
			switch (menuStatus)
			{
			case 0:
				isOut = true;
				break; //退出
			case 1:
				title_str = TITLE_MAIN;
				menu_size = 3;
				menu = mainMenu;
				index = &mainIndex;
				break;  	//主界面
			case 2:
				title_str = TITLE_SUB;
				menu_size = 15;
				menu = subMenu;
				index = &subIndex;
				break;  	//子系统界面
			case 3:
				title_str = TITLE_PARAM;
				menu_size = paramLength;
				menu = paramList;
				index = &paramIndex;
				break;  	//参数配置界面
			}
#ifdef win32
			SetConsoleTitleA(title_str);
			show(title_str, hOut, menu, menu_size, *index);
#else
			show(title_str, menu, menu_size, *index);
#endif
			ret = get_userinput(index, menu_size);

			//按键处理
			if (ret == ESC)
				break;
			else if (ret == ENTER)
			{
				switch (menuStatus)
				{
				case 1://主界面
					switch (mainIndex)
					{
					case 0: menuStatus = 2;      break;  	//启动项目
					case 1:	menuStatus = 3;	     break;  	//参数配置
					case 2: isOut = true;        break;    //返回
					}
					break; 
				case 2://子系统界面
					switch (subIndex)
					{
						case 0:  RUN_FYGQ();        break;  	//翻越高墙
						case 1:						 break;  	//通过壕坑
						case 2:                      break;  	//通过独木桥
						case 3:                      break;  	//独木桥绕桩
						case 4:                      break;  	//跨越矮墙
						case 5:                      break;  	//钻过矮墙paramList
						case 6:                      break;  	//爬越绳索
						case 7:                      break;  	//通过低桩网
						case 8:                      break;  	//隐蔽通过长壕
						case 9:                      break;  	//通过长壕并观察
						case 10:                     break;  	//高楼升降
						case 11:                     break;  	//窗户探索
						case 12:                     break;  	//单声枪【利用地形地物】
						case 13:                     break;  	//连续枪【利用地形地物】
						case 14:    menuStatus = 1;  break;     //返回
					}
					break;
				case 3://参数配置界面
					if (paramIndex == paramLength - 1)
					{
						menuStatus = 1;
						break;
					}
					char paramTmp[200];
					memcpy(paramTmp, paramList[paramIndex],200);
					strtok(paramTmp, split);
					float paramValue= cfg.get_param(paramTmp);

					float new_value=0;
					cout << "\n\n\n请输入新的参数:  ";
					scanf("%f", &new_value);
					cfg.set_param(paramTmp, new_value);
					cfg.openFile("r");
					cfg.readConfig(paramLength, paramList, paramValueList);
					paramList[paramLength++] = "返回";
					break;
				}

			}
			else if (ret == LEFT)
			{
				switch (menuStatus)
				{
				case 1://主界面
					isOut = true;
					break;
				case 2://子系统界面
				case 3://参数配置界面
					menuStatus = 1;
					break;
				}
			}

		}
		cfg.clearBlank();
		cfg.closeFile();
	};
#ifdef win32
	void show(char *title,HANDLE hOut, char **menu, int size, int index)
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
		printf("%s", title);
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
	#else
	int scanKeyboard()
	{
		int in;
		struct termios new_settings;
		struct termios stored_settings;
		tcgetattr(0,&stored_settings);
		new_settings = stored_settings;
		new_settings.c_lflag &= (~ICANON);
		new_settings.c_cc[VTIME] = 0;
		tcgetattr(0,&stored_settings);
		new_settings.c_cc[VMIN] = 1;
		tcsetattr(0,TCSANOW,&new_settings);
		
		in = getchar();
		
		tcsetattr(0,TCSANOW,&stored_settings);
		return in;
	}

	void show(char *title, char **menu, int size, int index)
	{
	int i;
		system("clear");
		printf("\033[0;10H\e[0;32m%s",title);
		printf("\033[2;10H\e[0;32m%s", AUTHOR);
		printf("\033[3;10H\e[0;32m%s", DATE);

		for (i = 0; i < size; i++)
		{
			//如果i==index表示在当前选项的位置，默认初始化显示是第一项，显示为红色，
			//当按下上下按键选择的时候，光标会移动，也就看到了列表选择的现象 
			if (i == index)
			{
				//红色 
				string tmp="\033[";
				tmp+=int2string(i+5);
				tmp+=";10H";
				tmp+=RED;
				tmp+="%s";
				printf(tmp.c_str(), menu[i]);
			}
			//否则显示为白色 
			else
			{
				//白色 
				string tmp="\033[";
				tmp+=int2string(i+5);
				tmp+=";10H";
				tmp+=WHITE;
				tmp+="%s";
				printf(tmp.c_str(), menu[i]);
			}
		}

	
	}
	#endif

};


int main(int argc,char** argv)
{
	Menu menu;
	menu.run();
	return 0;
}