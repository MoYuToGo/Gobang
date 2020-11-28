/******************************************************************************

1.0
初代

2.0
加入方向键交互
重构了判定部分的代码

2.1
修BUG

2.5
重构了地图更新有关的代码
按住方向键时地图不会闪烁了
增加了更新日志
增加了胜利方提示
修复了胜利时计数错误的问题(虽然没有什么影响w)
优化了键盘输入
将光标初始位置移到中间

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

int main(int argc, char const *argv[])
{
	//设置窗口标题和大小
	system("title 五子棋 2.5");
	system("mode con cols=38 lines=21");

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //窗口句柄
	int map[361] = {0};								 //地图
	int in_x = 8, in_y = 8, h = 8, k = 8;			 //光标位置
	int info = -1, t = 0;							 //游戏状态
	int key;										 //键盘输入
	int x, y, a = 0, b = 1, n = 0;					 //临时变量

	//隐藏控制台光标
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);

	while (1) //主体
	{
		while (1) //交互
		{
			//覆盖原先的格子
			SetConsoleCursorPosition(handle, {(short int)(h * 2), (short int)k}); //设置光标位置
			if (map[h + 19 * k] == 0)
			{
				printf("　");
			}
			else if (map[h + 19 * k] == 1)
			{
				printf("Ｘ");
			}
			else if (map[h + 19 * k] == -1)
			{
				printf("Ｏ");
			}

			//输出新的格子
			SetConsoleCursorPosition(handle, {(short int)(in_x * 2), (short int)in_y});
			SetConsoleTextAttribute(handle, 0x0080); //设置字体颜色和背景颜色
			if (map[in_x + 19 * in_y] == 0)
			{
				printf("　");
			}
			else if (map[in_x + 19 * in_y] == 1)
			{
				printf("Ｘ");
			}
			else if (map[in_x + 19 * in_y] == -1)
			{
				printf("Ｏ");
			}
			SetConsoleTextAttribute(handle, 0x000F);

			//提示
			SetConsoleCursorPosition(handle, {0, 19});
			if (t == 0)
			{
				if (info == -1)
				{
					printf("现在是 Ｏ 的回合\n");
					printf("使用方向键或WASD移动光标，回车落子");
				}
				else if (info == 1)
				{
					printf("现在是 Ｘ 的回合\n");
					printf("使用方向键或WASD移动光标，回车落子");
				}
			}
			else
			{
				printf("                                    \n");
				printf("                                    ");
				SetConsoleCursorPosition(handle, {0, 19});

				if (info == 1)
				{
					printf("游戏结束，Ｏ 胜利！\n");
				}
				else if (info == -1)
				{
					printf("游戏结束，Ｘ 胜利！\n");
				}

				system("color 02");
				Sleep(500);
				system("pause");
				return 0;
			}
			h = in_x;
			k = in_y;

			//获取键盘输入
			n = 0;
			key = _getch();
			if (key == 224)
			{
				key = _getch();
				n = 1;
			}

			if (key == 13 && n == 0 && map[in_x + 19 * in_y] == 0)
			{
				map[in_x + 19 * in_y] = info; //写地图
				break;						  //退出交互，进入判定
			}
			if (((key == 72 && n == 1) || ((key == 119 || key == 87) && n == 0)) && in_y > 0) //上
			{
				in_y--;
			}
			else if (((key == 75 && n == 1) || ((key == 97 || key == 65) && n == 0)) && in_x > 0) //左
			{
				in_x--;
			}
			else if (((key == 80 && n == 1) || ((key == 115 || key == 83) && n == 0)) && in_y < 18) //下
			{
				in_y++;
			}
			else if (((key == 77 && n == 1) || ((key == 100 || key == 68) && n == 0)) && in_x < 18) //右
			{
				in_x++;
			}
		}

		for (int i = 0; i < 4; i++) //判定
		{
			if (i == 1) //改变参数a,b,使其分别向下、右下、右、右上搜索
			{
				a = 1;
			}
			else if (i > 1)
			{
				b--;
			}

			n = 0; //重置计数器
			x = in_x;
			y = in_y;
			while (x - a >= 0 && y - b >= 0 && x - a <= 18 && y - b <= 18 && map[(x - a) + 19 * (y - b)] == info) //找端点
			{
				x -= a;
				y -= b;
			}
			while (x >= 0 && y >= 0 && x <= 18 && y <= 18 && map[x + 19 * y] == info) //计数
			{
				n++;
				x += a;
				y += b;
				if (n >= 5)
				{
					t = 1;
				}
			}
		}
		a = 0; //重置搜索方向
		b = 1;

		info = -info; //交换落子
	}
}