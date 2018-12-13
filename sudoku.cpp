#include<iostream>
#include<cstdio>
#include<cstring>
#include<fstream>
#include<algorithm>
using namespace std;
struct Space {
	int row, col, anum;				//行数、列数、方格数
};
class Sudo {
private:
	ifstream fin;					//从文件读取输入
	ofstream fout;					//输出到文件
	int num_space;					//解数独中空格的个数
	int array[10][10];				//存放解数独的题目
	Space space[60];				//存放空格
	bool ok;						//判断是否已经解决 
public:
	void Finality(int num);
	void Solution(char* file);
	void Movesudo(int, int, int, int*);
	void Tryspace(int);
};
void Sudo::Movesudo(int rule1, int rule2, int rule3, int* a)
{
	int steps[10];					//记录每行移动的距离
	steps[0] = 0;
	steps[1] = rule1 / 10;
	steps[2] = rule1 % 10;
	steps[3] = rule2 / 100;
	steps[4] = (rule2 % 100) / 10;
	steps[5] = rule2 % 10;
	steps[6] = rule3 / 100;
	steps[7] = (rule3 % 100) / 10;
	steps[8] = rule3 % 10;
	for (int i = 0; i < 9; ++i) {
		int step = steps[i];
		for (int j = 0; j < 8; ++j) {
			int number = (j + step) % 9;		//记录移动后的下标
			fout << a[number] << " ";
		}
		fout << a[(8 + step) % 9] << endl;
	}
	fout << endl;
}
void Sudo::Finality(int num)
{
	int a[] = { 2, 1, 3, 4, 5, 6, 7, 8, 9 };
	int rule[8][8] = { {36, 63}, {147, 174, 417, 471, 714, 741}, {258, 285, 528, 582, 825, 852} };
	//生成终局
	fout.open("sudoku.txt");
	int count = 0;					//记录终局个数
	do {
		//平移的2*6*6种方式
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 6; ++j) {
				for (int k = 0; k < 6; ++k) {
					Movesudo(rule[0][i], rule[1][j], rule[2][k], a);
					++count;
				}
			}
		}
		if (count > num) {
			break;
		}
	} while (next_permutation(a + 1, a + 8));			//使用STL全排列函数
	fout.close();
}
void Sudo::Tryspace(int now)
{
	//逐个试探空格中的数字
	if (ok) {
		return;
	}
	if (now > num_space) {
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 8; ++j) {
				fout << array[i][j] << " ";
			}
			fout << array[i][8] << endl;
		}
		return;
	}
	int row = space[now].row;				//当前空格的所在行
	int col = space[now].col;				//当前空格的所在列
	int anum = space[now].anum;				//当前空格的所在方格数
	for (int i = 1; i <= 9; ++i) {
		bool flag = true;					//检测是否符合数独规则
		array[row][col] = i;						//试探该空格的值
		for (int j = 0; j < 9 && flag; ++j) {
			//检测该行是否出现重复的数字
			if (array[row][j] == i && j != col) {
				flag = false;
				break;
			}
		}
		for (int j = 0; j < 9 && flag; ++j) {
			//检测该列是否出现重复的数字
			if (array[j][col] == i && j != row) {
				flag = false;
				break;
			}
		}
		for (int j = (anum / 3) * 3; j <= (anum / 3) * 3 + 2 && flag; ++j) {
			for (int k = (anum % 3) * 3; k <= (anum % 3) * 3 + 2 && flag; ++k) {
				//检测该方格里是否出现重复的数字
				if (array[j][k] == i && (j != row || k != col)) {
					flag = false;
					break;
				}
			}
		}
		if (flag) {
			//向下一个空格试探
			Tryspace(now + 1);
		}
	}
	array[row][col] = 0;
}
void Sudo::Solution(char* file)
{
	//求解数独
	fin.open(file);
	fout.open("sudoku2.txt");
	while (!fin.eof()) {
		//读取题目
		int count = 1;					//空格的编号
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				fin >> array[i][j];
				if (array[i][j] == 0) {
					space[count].row = i;
					space[count].col = j;
					space[count++].anum = (i / 3) * 3 + (j / 3);
				}
			}
		}
		num_space = count - 1;
		ok = false;
		Tryspace(1);
		fout << endl;
	}
	fin.close();
	fout.close();
}
int main(int argc, char* argv[])
{//test
	Sudo sudo;
	if (argc == 3 && argv[1][1] == 'c') {
		//生成终局
		bool flag = true;				//记录是否有非法输入
		int num = 0;
		for (int i = strlen(argv[2]) - 1, times = 1; i >= 0; --i) {
			if (argv[2][i] - '0' < 0 || argv[2][i] - '0' > 9) {
				flag = false;
				break;
			}
			num += (argv[2][i] - '0')*times;
			times *= 10;
		}
		if (flag) {
			sudo.Finality(num);
		}
		else {
			//输入异常
			printf("Error!\n");
		}
	}
	else if (argc == 3 && argv[1][1] == 's') {
		//求解数独
		sudo.Solution(argv[2]);
	}
	else {
		//输入异常
		printf("Error!");
	}
	return 0;
}