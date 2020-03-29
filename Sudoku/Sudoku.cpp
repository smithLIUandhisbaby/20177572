#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//4996是为了提醒fopen、fclose、fscanf函数不安全的
#pragma warning(disable:4996)
using namespace std;
int write(int x, int y);
int inside(int x, int y);
int outside(int x, int y);
void check();
//记录棋盘上的标记点 
bool sign[10][10][10];
//记录标记点的可能性剩余个数 
int sign_count[10][10];
//记录棋盘 
int checkerboard[10][10];
//记录类型
int type;
int small_sign[10];
//用于记录是否有变化
bool change;
int chess_count;


//重置棋盘和标记 
void reset()
{
	for (int i = 1; i < type + 1; i++)
	{
		for (int j = 1; j < type + 1; j++)
		{
			//假设每个位置都有type种可能 
			sign_count[i][j] = type;
			//每个位置都是空 
			checkerboard[i][j] = 0;
			//每个位置未曾填写 
			sign[i][j][0] = false;
			//假设每个位置的type种可能都是可实现的
			for (int k = 1; k < type + 1; k++)
			{
				sign[i][j][k] = true;
			}
		}
	}
	return;
}

//填入确定值 
int write(int x, int y)
{
	//这个位置标记为存在数字 
	change = true;
	chess_count--;
	sign[x][y][0] = true;
	sign_count[x][y] = 0;
	//填入数字 
	for (int i = 1; i < type + 1; i++)
	{
		if (sign[x][y][i])
		{
			checkerboard[x][y] = i;
			break;
		}
	}
	/*//输出改变的位置和新棋盘
	cout<<x<<' '<<y<<'\n';
	for ( int i=1 ; i<type+1 ; i++ )
	{
		for ( int j=1 ; j<type+1 ; j++ )
		{
			cout<<checkerboard[i][j]<<' ';
		}
		cout<<'\n';
	}
	//输出每个位置的可能性
	cout<<'\n'<<"可能性：\n";
	for ( int i=1 ; i<type+1 ; i++ )
	{
		for ( int j=1 ; j<type+1 ; j++ )
		{
			cout<<sign_count[i][j]<<' ';
		}
		cout<<'\n';
	}
	cout<<'\n';*/
	outside(x, y);
	return 0;
}

//查出所有空缺的可能性（位置上还没有数字） 
//此时是扣除所在位置的可能性 
int inside(int x, int y)
{
	//排除横纵向可能性
	int remove;
	for (int i = 1; i < type + 1; i++)
	{
		//如果检测位置存在数 
		if (sign[x][i][0])
		{
			remove = checkerboard[x][i];
			//则这个空位不能出现相同数字
			//防止sign_count被误减去，前面先判断是不是已经变否了，未变否才变否 
			if (sign[x][y][remove])
			{
				sign[x][y][remove] = false;
				//可能性-1
				sign_count[x][y]--;
			}
			if (sign_count[x][y] == 1 && !sign[x][y][0])
			{
				write(x, y);
				return 0;
			}
		}
	}
	for (int i = 1; i < type + 1; i++)
	{
		if (sign[i][y][0])
		{
			remove = checkerboard[i][y];
			if (sign[x][y][remove])
			{
				sign[x][y][remove] = false;
				sign_count[x][y]--;
			}
			if (sign_count[x][y] == 1 && !sign[x][y][0])
			{
				write(x, y);
				return 0;
			}
		}
	}
	//宫格判断 
	if (type == 4 || type == 6 || type == 8 || type == 9)
	{
		int beginx, beginy;
		int xplus, yplus;
		switch (type)
		{
		case 4:
			xplus = 2;
			yplus = 2;
			break;
		case 6:
			xplus = 2;
			yplus = 3;
			break;
		case 8:
			xplus = 4;
			yplus = 2;
			break;
		case 9:
			xplus = 3;
			yplus = 3;
			break;
		}
		beginx = ((x - 1) / xplus)*xplus + 1;
		beginy = ((y - 1) / yplus)*yplus + 1;
		for (int i = beginx; i < beginx + xplus; i++)
		{
			for (int j = beginy; j < beginy + yplus; j++)
			{
				if (sign[i][j][0])
				{
					if (sign[x][y][(checkerboard[i][j])])
					{
						sign[x][y][(checkerboard[i][j])] = false;
						sign_count[x][y]--;
					}
					if (sign_count[x][y] == 1 && !sign[x][y][0])
					{
						write(x, y);
						return 0;
					}
				}
			}
		}
	}
	//经过上面的判断，如果该位置只剩下一种可能性，那么执行write()
	return 0;
}


//去除所填位置的横纵九宫格所有同数可能性（位置上刚填入数字） 
//此时是扣除所填位置的横纵九宫格的其他位置可能性 
int outside(int x, int y)
{
	//remove是当前位置填入的数字 
	int remove = checkerboard[x][y];
	for (int i = 1; i < type + 1; i++)
	{
		if (!sign[x][i][0] && sign[x][i][remove])
		{
			sign[x][i][remove] = false;
			sign_count[x][i]--;
			if (sign_count[x][i] == 1 && !sign[x][i][0])
			{
				write(x, i);
			}
		}
	}
	for (int i = 1; i < type + 1; i++)
	{
		if (!sign[i][y][0] && sign[i][y][remove])
		{
			sign[i][y][remove] = false;
			sign_count[i][y]--;
			if (sign_count[i][y] == 1 && !sign[i][y][0])
			{
				write(i, y);
			}
		}
	}
	//宫格判断 
	if (type == 4 || type == 6 || type == 8 || type == 9)
	{
		int beginx, beginy;
		int xplus, yplus;
		switch (type)
		{
		case 4:
			xplus = 2;
			yplus = 2;
			break;
		case 6:
			xplus = 2;
			yplus = 3;
			break;
		case 8:
			xplus = 4;
			yplus = 2;
			break;
		case 9:
			xplus = 3;
			yplus = 3;
			break;
		}
		beginx = ((x - 1) / xplus)*xplus + 1;
		beginy = ((y - 1) / yplus)*yplus + 1;
		for (int i = beginx; i < beginx + xplus; i++)
		{
			for (int j = beginy; j < beginy + yplus; j++)
			{
				if (!sign[i][j][0] && sign[i][j][remove])
				{
					sign[i][j][remove] = false;
					sign_count[i][j]--;
					if (sign_count[i][j] == 1 && !sign[i][j][0])
					{
						write(i, j);
					}
				}
			}
		}
	}
	return 0;
}

void check()
{
	//检查每一横
	for (int i = 1; i < type + 1; i++)
	{
		for (int j = 1; j < type + 1; j++)
		{
			small_sign[j] = 0;
		}
		for (int j = 1; j < type + 1; j++)
		{
			if (!sign[i][j][0])
			{
				for (int k = 1; k < type + 1; k++)
				{
					if (sign[i][j][k])
					{
						small_sign[k]++;
					}
				}
			}
		}
		for (int k = 1; k < type + 1; k++)
		{
			if (small_sign[k] == 1)
			{
				for (int j = 1; j < type + 1; j++)
				{
					if (sign[i][j][k] && !sign[i][j][0])
					{
						//这个位置标记为存在数字 
						chess_count--;
						change = true;
						sign[i][j][0] = true;
						sign_count[i][j] = 0;
						checkerboard[i][j] = k;
						outside(i, j);
					}
				}
			}
		}
	}
	//检查每一纵
	for (int j = 1; j < type + 1; j++)
	{
		for (int i = 1; i < type + 1; i++)
		{
			small_sign[i] = 0;
		}
		for (int i = 1; i < type + 1; i++)
		{
			if (!sign[i][j][0])
			{
				for (int k = 1; k < type + 1; k++)
				{
					if (sign[i][j][k])
					{
						small_sign[k]++;
					}
				}
			}
		}
		for (int k = 1; k < type + 1; k++)
		{
			if (small_sign[k] == 1)
			{
				for (int i = 1; i < type + 1; i++)
				{
					if (sign[i][j][k] && !sign[i][j][0])
					{
						//这个位置标记为存在数字
						chess_count--;
						change = true;
						sign[i][j][0] = true;
						sign_count[i][j] = 0;
						checkerboard[i][j] = k;
						outside(i, j);
					}
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int n;
	FILE* fp1;
	FILE* fp2;
	char* InputName = NULL;
	char* OutputName = NULL;
	string a = "-m", b = "-n", c= "-i", d="-o";
	for (int i = 1; i < argc; i++ )
	{
		if (argv[i] == a)
		{
			type = argv[++i][0] - '0';
		}
		else if (argv[i] == b)
		{
			n = argv[++i][0] - '0';
		}
		else if (argv[i] == c)
		{
			InputName = argv[++i];
		}
		else if (argv[i] == d)
		{
			OutputName = argv[++i];
		}
	}
	//type = atoi(argv[2]);
	//n = atoi(argv[4]);
	//char* InputName = argv[6];
	//char* OutputName = argv[8];
	//以只读方式打开文件
	fp1 = fopen(InputName, "r");
	if (fp1 == NULL) //
		return -1;
	//fscanf(fp1, "%d%d", &type,&n);
	//打开output.txt，并立即关闭，意义为清空文本内容
	fp2 = fopen(OutputName, "w");
	if (fp2 == NULL) //
		return -1;
	fclose(fp2);
	while (n > 0)
	{
		//重置棋盘 
		reset();
		//输入棋盘 
		for (int i = 1; i < type + 1; i++)
		{
			for (int j = 1; j < type + 1; j++)
			{
				//cin >> checkerboard[i][j];
				fscanf(fp1, "%d", &checkerboard[i][j]);
				if (checkerboard[i][j] != 0)
				{
					sign[i][j][0] = true;
					sign_count[i][j] = 0;
					chess_count--;
				}
			}
		}
		//棋盘上以填格子的数量，当它等于零的时候棋盘被填满
		chess_count = type * type;
		change = true;
		while (chess_count != 0 && change)
		{
			//先默认棋盘不发生变化
			change = false;
			//找出空缺位置
			for (int k = 0; k < 2; k++)
			{
				for (int i = 1; i < type + 1; i++)
				{
					for (int j = 1; j < type + 1; j++)
					{
						if (!sign[i][j][0])
						{
							inside(i, j);
						}
					}
				}
			}
			check();
		}
		//以只写方式打开文件
		fp2 = fopen(OutputName, "a");
		if (fp2 == NULL)
			return -1;
		
		bool sign_complete = true;
		for (int i = 1; i < type + 1; i++)
		{
			for (int j = 1; j < type + 1; j++)
			{
				if (checkerboard[i][j] == 0)
				{
					sign_complete = false;
				}
				//cout << checkerboard[i][j];
				fprintf(fp2, "%d", checkerboard[i][j]);
				if (j != type)
				{
					fprintf(fp2, " ");
					//cout << ' ';
				}
			}
			if (n != 1 && i == type && sign_complete)
			{
				//cout << "\n\n";  
				fprintf(fp2, "\n\n");
			}
			else if (n != 1 && i == type && !sign_complete)
			{
				//cout << "\n无法再确定地填入任何一格\n因此棋盘中有空位\n\n";
				fprintf(fp2, "\n无法再确定地填入任何一格\n因此棋盘中有空位\n\n");
			}
			else if (n == 1 && i == type && sign_complete) {}
			else if (n == 1 && i == type && !sign_complete)
			{
				//cout << "\n无法再确定地填入任何一格\n因此棋盘中有空位";
				fprintf(fp2, "\n无法再确定地填入任何一格\n因此棋盘中有空位");
			}
			else
			{
				//cout << "\n";
				fprintf(fp2, "\n");
			}
		}
		//cout << '\n';//
		//fprintf(fp2, "\n");
		n--;
		fclose(fp2);
	}
	fclose(fp1);
}