#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<fstream>
#include<algorithm>
using namespace std;
struct Space {
	int row, col, anum;				//行数、列数、方格数
};
class Sudo {
private:
	int num_space;					//解数独中空格的个数
	int array[10][10];				//存放解数独的题目
	Space space[60];				//存放空格
	bool ok;						//判断是否已经解决 
public:
	void Finality(int num);
//	void Solution(char* file);
	void Movesudo(char*, char*, char*, char*, FILE*);
//	void Tryspace(int);
};
void Sudo::Movesudo(char *rule0, char *rule1, char *rule2, char *a, FILE *fout)
{
	//将每个字符型存储的移位规则转换成整形的移位规则 
	int steps[10];					//记录每行移动的距离
	int num = 0;
	for (int i = 0; i < 3; ++i) {
		steps[num++] = rule0[i] - '0';
	}
	for (int i = 0; i < 3; ++i) {
		steps[num++] = rule1[i] - '0';
	}
	for (int i = 0; i < 3; ++i) {
		steps[num++] = rule2[i] - '0';
	}
	//开始移位并存储在文件中
	
	for (int i = 0; i < 9; ++i) {
		int step = steps[i];
		fputc(a[(8+step)%9], fout); 
		for (int j = 1; j < 17; ++j) {
			fputc(' ', fout);
			fputc(a[((16-j)/2 + step) % 9], fout);
			++j;
		}
		fputc('\n', fout);
	}
	fputc('\n', fout);
	
}
void Sudo::Finality(int num)
{
	char a[15] = { '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	char rule0[10][5] = {"036", "063"};
	char rule1[10][5] = {"258", "285", "528", "582", "825", "852"};
	char rule2[10][5] = {"147", "174", "417", "471", "714", "741"};
	//生成终局
	FILE *fout = fopen("sudoku.txt", "w");
	int count = 0;					//记录终局个数
	bool flag = true;
	do {
		a[8] = '3';					//插入学号 :（2+0）% 9 + 1 = 3 
		//平移的2*6*6种方式
		for (int i = 0; i < 2 && flag; ++i) {
			for (int j = 0; j < 6 && flag; ++j) {
				for (int k = 0; k < 6 && flag; ++k) {
					Movesudo(rule0[i], rule1[j], rule2[k], a, fout);
					++count;
					if (count >= num) {
						flag = false;
						break;
					}
				}
			}
		}
	} while (next_permutation(a + 1, a + 8));			//使用STL全排列函数
	fputc('\0', fout);
	fclose(fout);
}
//void Sudo::Tryspace(int now)
//{
//	//逐个试探空格中的数字
//	if (ok) {
//		return;
//	}
//	if (now > num_space) {
//		for (int i = 0; i < 9; ++i) {
//			for (int j = 0; j < 8; ++j) {
//				fout << array[i][j] << " ";
//			}
//			fout << array[i][8] << endl;
//		}
//		return;
//	}
//	int row = space[now].row;				//当前空格的所在行
//	int col = space[now].col;				//当前空格的所在列
//	int anum = space[now].anum;				//当前空格的所在方格数
//	for (int i = 1; i <= 9; ++i) {
//		bool flag = true;					//检测是否符合数独规则
//		array[row][col] = i;						//试探该空格的值
//		for (int j = 0; j < 9 && flag; ++j) {
//			//检测该行是否出现重复的数字
//			if (array[row][j] == i && j != col) {
//				flag = false;
//				break;
//			}
//		}
//		for (int j = 0; j < 9 && flag; ++j) {
//			//检测该列是否出现重复的数字
//			if (array[j][col] == i && j != row) {
//				flag = false;
//				break;
//			}
//		}
//		for (int j = (anum / 3) * 3; j <= (anum / 3) * 3 + 2 && flag; ++j) {
//			for (int k = (anum % 3) * 3; k <= (anum % 3) * 3 + 2 && flag; ++k) {
//				//检测该方格里是否出现重复的数字
//				if (array[j][k] == i && (j != row || k != col)) {
//					flag = false;
//					break;
//				}
//			}
//		}
//		if (flag) {
//			//向下一个空格试探
//			Tryspace(now + 1);
//		}
//	}
//	array[row][col] = 0;
//}
//void Sudo::Solution(char* file)
//{
//	//求解数独
//	fin.open(file);
//	fout.open("sudoku2.txt");
//	while (!fin.eof()) {
//		//读取题目
//		int count = 1;					//空格的编号
//		for (int i = 0; i < 9; ++i) {
//			for (int j = 0; j < 9; ++j) {
//				fin >> array[i][j];
//				if (array[i][j] == 0) {
//					space[count].row = i;
//					space[count].col = j;
//					space[count++].anum = (i / 3) * 3 + (j / 3);
//				}
//			}
//		}
//		num_space = count - 1;
//		ok = false;
//		Tryspace(1);
//		fout << endl;
//	}
//	fin.close();
//	fout.close();
//}
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
			printf("over!\n");
		}
		else {
			//输入异常
			printf("Error!\n");
		}
	}
//	else if (argc == 3 && argv[1][1] == 's') {
//		//求解数独
//		sudo.Solution(argv[2]);
//	}
	else {
		//输入异常
		printf("Error!");
	}
	return 0;
}