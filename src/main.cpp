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
#define  TITLE_MAIN  "ZSXL���ϸ�֪ϵͳ-��ҳ��"
#define  TITLE_SUB  "ZSXL���ϸ�֪ϵͳ-����Ŀ��������"
#define  TITLE_PARAM  "ZSXL���ϸ�֪ϵͳ-�������ý���"
#define  AUTHOR "syj.V1.0"
#define  DATE   "ά������:2020��02��26��"
//����ö��Keyboard�ļ�ֵ���� 
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
//����Ҫ��ʾ�Ĳ˵� 
char *subMenu[200] =
{
	"�� 1  ��Խ��ǽ              ��",
	"�� 2  ͨ������              ��",
	"�� 3  ͨ����ľ��            ��",
	"�� 4  ��ľ����׮            ��",
	"�� 5  ��Խ��ǽ              ��",
	"�� 6  �����ǽ              ��",
	"�� 7  ��Խ����              ��",
	"�� 8  ͨ����׮��            ��",
	"�� 9  ����ͨ������          ��",
	"�� 10 ͨ���������۲�        ��",
	"�� 11 ��¥����              ��",
	"�� 12 ����̽��              ��",
	"�� 13 ����ǹ�����õ��ε����",
	"�� 14 ����ǹ�����õ��ε����",
	"�� 15 ����                  ��",
};
char *mainMenu[200] =
{
	"�� 1  ������Ŀ              ��",
	"�� 2  ��������              ��",
	"�� 3  �˳�                  ��",
};
char *paramMenu[200] =
{
	"�� 1  frame_downsampling_percent            ��",
	"�� 2  continues_frame_goal_cnt              ��",
	"�� 3  ����                                  ��",
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
	//����ṹ��
	CONSOLE_CURSOR_INFO cci;
	//����Ĭ�ϵ�����λ��  	
	COORD pos;
#endif
	//��ȡ�û�����Ľӿ� 
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
			//�� 
			//���ѡ���ϣ���ô��������ƶ� 
		case UP_:
		case UP: if (*index > 0)  *index -= 1;  break;
			//�� 
			//���ѡ���£���ô��������ƶ� 
		case DOWN_:
		case DOWN:if (*index < size - 1)  *index += 1;  break;
			//�� 
		case LEFT_: return LEFT;
		case LEFT: return LEFT;
			//�� 
		case RIGHT_:return ENTER;
		case RIGHT:return ENTER;
			//�س� 
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
		paramList[paramLength++] = "����";

		float frame_downsampling_percent;
		int cnt;
#ifdef win32
		//��ȡ��ǰ�ľ��---����Ϊ��׼������ 
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		//��ȡ�����Ϣ
		GetConsoleCursorInfo(hOut, &cci);
		//���ù���С   
		cci.dwSize = 1;
		//���ù�겻�ɼ� FALSE   
		cci.bVisible = 0;
		//����(Ӧ��)�����Ϣ
		SetConsoleCursorInfo(hOut, &cci);
#endif
		char *title_str = TITLE_MAIN;
		int menu_size = 0;
		char **menu = mainMenu;
		int *index = &mainIndex;
		while (!isOut)
		{
			//״̬��ʾ
			//0-out;1-main;2-sub;3-param
			switch (menuStatus)
			{
			case 0:
				isOut = true;
				break; //�˳�
			case 1:
				title_str = TITLE_MAIN;
				menu_size = 3;
				menu = mainMenu;
				index = &mainIndex;
				break;  	//������
			case 2:
				title_str = TITLE_SUB;
				menu_size = 15;
				menu = subMenu;
				index = &subIndex;
				break;  	//��ϵͳ����
			case 3:
				title_str = TITLE_PARAM;
				menu_size = paramLength;
				menu = paramList;
				index = &paramIndex;
				break;  	//�������ý���
			}
#ifdef win32
			SetConsoleTitleA(title_str);
			show(title_str, hOut, menu, menu_size, *index);
#else
			show(title_str, menu, menu_size, *index);
#endif
			ret = get_userinput(index, menu_size);

			//��������
			if (ret == ESC)
				break;
			else if (ret == ENTER)
			{
				switch (menuStatus)
				{
				case 1://������
					switch (mainIndex)
					{
					case 0: menuStatus = 2;      break;  	//������Ŀ
					case 1:	menuStatus = 3;	     break;  	//��������
					case 2: isOut = true;        break;    //����
					}
					break; 
				case 2://��ϵͳ����
					switch (subIndex)
					{
						case 0:  RUN_FYGQ();        break;  	//��Խ��ǽ
						case 1:						 break;  	//ͨ������
						case 2:                      break;  	//ͨ����ľ��
						case 3:                      break;  	//��ľ����׮
						case 4:                      break;  	//��Խ��ǽ
						case 5:                      break;  	//�����ǽparamList
						case 6:                      break;  	//��Խ����
						case 7:                      break;  	//ͨ����׮��
						case 8:                      break;  	//����ͨ������
						case 9:                      break;  	//ͨ���������۲�
						case 10:                     break;  	//��¥����
						case 11:                     break;  	//����̽��
						case 12:                     break;  	//����ǹ�����õ��ε��
						case 13:                     break;  	//����ǹ�����õ��ε��
						case 14:    menuStatus = 1;  break;     //����
					}
					break;
				case 3://�������ý���
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
					cout << "\n\n\n�������µĲ���:  ";
					scanf("%f", &new_value);
					cfg.set_param(paramTmp, new_value);
					cfg.openFile("r");
					cfg.readConfig(paramLength, paramList, paramValueList);
					paramList[paramLength++] = "����";
					break;
				}

			}
			else if (ret == LEFT)
			{
				switch (menuStatus)
				{
				case 1://������
					isOut = true;
					break;
				case 2://��ϵͳ����
				case 3://�������ý���
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
		//������ʾ���ı�����ɫ 
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | 0x8);
		//��ʼ������̨��ʾ��X,Y������� 
		pos.X = 30;
		pos.Y = 0;
		//������ʾ������̨�ն˵ľ���λ�� 
		SetConsoleCursorPosition(hOut, pos);
		//����printf�ڿ���̨��Ӧ��λ������� 
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
			//���i==index��ʾ�ڵ�ǰѡ���λ�ã�Ĭ�ϳ�ʼ����ʾ�ǵ�һ���ʾΪ��ɫ��
			//���������°���ѡ���ʱ�򣬹����ƶ���Ҳ�Ϳ������б�ѡ������� 
			if (i == index)
			{
				//��ɫ 
				SetConsoleTextAttribute(hOut, FOREGROUND_RED | 0x8);
				pos.X = 30;
				pos.Y = 5 + i;
				//���ù������
				SetConsoleCursorPosition(hOut, pos);
				printf("%s", menu[i]);
			}
			//������ʾΪ��ɫ 
			else
			{
				//��ɫ 
				SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				pos.X = 30;
				pos.Y = 5 + i;
				//���ù������
				SetConsoleCursorPosition(hOut, pos);    //���ù������
				printf("%s", menu[i]);
			}
		}
		//ˢ�±�׼��������� 
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
			//���i==index��ʾ�ڵ�ǰѡ���λ�ã�Ĭ�ϳ�ʼ����ʾ�ǵ�һ���ʾΪ��ɫ��
			//���������°���ѡ���ʱ�򣬹����ƶ���Ҳ�Ϳ������б�ѡ������� 
			if (i == index)
			{
				//��ɫ 
				string tmp="\033[";
				tmp+=int2string(i+5);
				tmp+=";10H";
				tmp+=RED;
				tmp+="%s";
				printf(tmp.c_str(), menu[i]);
			}
			//������ʾΪ��ɫ 
			else
			{
				//��ɫ 
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