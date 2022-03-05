#include<iostream>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
#include<cstring>
#pragma warning(disable : 4996)								//sprintf()函数的使用会警告，可将警告忽略
using namespace std;

int picture_bk;												//控制背景切换到哪个
bool change_bk;												//控制背景切换
wchar_t tpattern[20];										//待查找字符串
char pattern[20];											//待查找字符串
bool isInput;												//控制是否输入
int showPage;												//结果显示页码
int dataNum;												//结果每一页的数量
bool isShowReslut;											//是否显示结果
int nfind; 													//要查找的序号

IMAGE img_background1;										//欢迎背景
IMAGE img_background2;										//功能选择背景
IMAGE img_background3;										//检索背景
IMAGE img_background4;										//帮助背景
IMAGE img_background5;										//关于背景
IMAGE img_background6;										//显示结果背景
IMAGE img_background7;										//查询详细信息背景
IMAGE img_background8;										//详细信息显示背景

struct Text													//文章一行的信息
{
	string content;
	string bookName;
	int page=0;
	int chapter=0;
};

vector<Text>save_book;										//存书
vector<int>text_index;										//查询到的下标

void startup();
void readBook();
void getText(string book_name);
void search();
void showResult();
void show();
void backgroundShow();
void control();
void bk2Control();
void bk3Control();
void bk45Control();
void bk6Control();
void bk7Control();
void bk8Control();


int main()
{
	startup();													//初始化
	readBook();													//读入txt
	while (1)
	{
		show();
		control();
	}
}

void startup()													//数据初始化
{
	picture_bk = 1;
	change_bk = 1;
	isInput = 1;
	showPage = 1;
	dataNum = 13;
	initgraph(820, 615);										//定义画布大小
	loadimage(&img_background1, _T(".\\picture\\background2.jpg"));//载入图片
	loadimage(&img_background2, _T(".\\picture\\background3.jpg"));
	loadimage(&img_background3, _T(".\\picture\\background4.jpg"));
	loadimage(&img_background4, _T(".\\picture\\background5.jpg"));
	loadimage(&img_background5, _T(".\\picture\\background6.jpg"));
	loadimage(&img_background6, _T(".\\picture\\background7.jpg"));
	loadimage(&img_background7, _T(".\\picture\\background4.jpg"));
	loadimage(&img_background8, _T(".\\picture\\background4.jpg"));


}

void readBook()													//获取所有书的内容
{
	getText("J.K. Rowling - HP 3 - Harry Potter and the Prisoner of Azkaban");
}																//重复操作，调用函数实现

void getText(string book_name)									//获取一本书的内容
{
	string pname = book_name + ".txt";							//加上扩展名
	ifstream fs(pname.c_str());									//打开txt文件
	if (!fs)													//打开失败，退出
	{
		cout << "文本读取错误!!!" << endl;
		exit(1);
	}
	Text temp_text;												//临时存储text
	temp_text.bookName = book_name;								//书名不变
	temp_text.chapter = 0;										//开始为0
	temp_text.page = 1;											//因为页码最后显示，初始为1
	string line;
	while (getline(fs, line))									//逐行读入
	{
		int len = line.length();
		if (len <= 3 && line[0] >= '1' && line[0] <= '9')		//页码
		{
			temp_text.page++;
			continue;
		}
		else if (len < 20)										//判断是否是章节编号
			if (line.substr(0, 7) == "chapter" || line.substr(0, 7) == "CHAPTER")
			{
				temp_text.chapter++;
				continue;
			}
		if (len < 3)
			continue;
		temp_text.content = line;
		save_book.push_back(temp_text);							//不是页码和章节编号，加入数组
	}
}

void search()													//查找字符串
{
	int tot = save_book.size();									//文件总行数
	int cnt = 0;
	string::size_type nnpos = string::npos;						//未查找到的返回值
	while (cnt < tot)
	{
		string::size_type position = save_book[cnt].content.find(pattern);
		if (position != nnpos)									//查找到，加入答案数组
			text_index.push_back(cnt);
		cnt++;
	}
}

void showResult()
{
	int cnt = dataNum * (showPage - 1);							//该页的起始答案编号
	int tot = dataNum * showPage;								//末尾编号
	if (tot > text_index.size())								//可能该页不满，避免越界
		tot = text_index.size();
	if (text_index.size()== 0)									//长度为0，说明没有查找到
	{
		settextstyle(80, 40, _T("黑体"));						//设置字体格式
		outtextxy(240, 270, "Not found!");						//指定位置输出
		return;
	}
	else if (tot <= 0||tot<cnt||cnt<0)							//翻到第一页之前或最后一页之后
	{
		settextstyle(40, 20, _T("黑体"));
		outtextxy(100, 240, "Please return to the previous page");
		return;
	}
	int distance = 30;											//每一行输出的间距
	for (int i = cnt; i < tot; i++)
	{
		int pos = text_index[i];
		TCHAR s[30];
		_stprintf(s, _T("%d"),i+1);								//将序号转换为TCHAR型
		settextstyle(20, 10, _T("黑体"));
		outtextxy(10, 90+distance*(i-cnt), s);					//指定位置输出
		outtextxy(70, 90+distance * (i - cnt), pattern);
		_stprintf(s, _T("%d"),save_book[pos].page );
		outtextxy(160, 90+distance * (i - cnt), s);
		_stprintf(s, _T("%d"), save_book[pos].chapter);
		outtextxy(215, 90+distance * (i - cnt), s);
		char ss[150];
		int j;
		string str = save_book[i].bookName;
		for (j = 0; j < str.length(); j++)						//sting转换为char型数组
			ss[j] = str[j];
		ss[j] = '\0';
		settextstyle(16, 8, _T("黑体"));
		outtextxy(290, 90 + distance * (i - cnt), ss);
	}
}

void show()
{
	if (change_bk)												//是否需要更换背景
	{
		backgroundShow();										//更换背景
		change_bk = 0;											//控制变量更新
	}
}

void backgroundShow()											//背景切换
{
	if (picture_bk == 1)										//欢迎背景
	{
		putimage(0, 0, &img_background1);						//应用图片
		Sleep(2000);											//停留2s
		picture_bk = 2;											//直接切换到背景2
	}
	if (picture_bk == 2)										//选择功能背景
		putimage(0, 0, &img_background2);						
	else if (picture_bk == 3)									//检索背景
		putimage(0, 0, &img_background3);
	else if (picture_bk == 4)									//帮助背景
		putimage(0, 0, &img_background4);
	else if (picture_bk == 5)									//关于背景
		putimage(0, 0, &img_background5);
	else if (picture_bk == 6)									//结果背景
		putimage(0, 0, &img_background6);
	else if (picture_bk == 7)									//查询背景
		putimage(0, 0, &img_background7);
	else if (picture_bk == 8)									//详细背景
		putimage(0, 0, &img_background8);
}

void control()													//操作
{
	if (picture_bk == 2)										//每个背景对应操作
		bk2Control();
	else if (picture_bk == 3)
		bk3Control();
	else if (picture_bk == 4)
		bk45Control();
	else if (picture_bk == 5)
		bk45Control();
	else if (picture_bk == 6)
		bk6Control();
	else if (picture_bk == 7)
		bk7Control();
	else if (picture_bk == 8)
		bk8Control();
}

void bk2Control()												//操作2
{
	MOUSEMSG m;													//鼠标信息
	if (MouseHit())												//鼠标动了
	{
		m = GetMouseMsg();										//获取鼠标信息
		if (m.uMsg == WM_LBUTTONDOWN)							//按下左键
		{
			if (m.x >= 300 && m.x <= 520 && m.y <= 260 && m.y >= 210)
			{													//点击指定位置
				picture_bk = 3;									//检索
				change_bk = 1;									//更新控制变量
			}
			else if (m.x >= 300 && m.x <= 520 && m.y <= 325 && m.y >= 280)
			{													
				picture_bk = 4;									//帮助
				change_bk = 1;
			}
			else if (m.x >= 360 && m.x <= 460 && m.y <= 400 && m.y >= 350)
			{
				picture_bk = 5;									//关于
				change_bk = 1;
			}
			else if (m.x <= 820 && m.x >= 720 && m.y <= 615 && m.y >= 565)
				exit(1);										//退出
		}
	}
}

void bk3Control()												//操作3
{																//输入框
	InputBox(pattern,20, "请输入要查找的人名/地名                      （不要多打回车哦）",NULL,NULL,300,150);
	text_index.clear();											//清空上一次结果下标
	search();													//查找
	picture_bk = 6;												//更换至结果显示背景
	change_bk = 1;
	isShowReslut = 1;											//控制变量更新
	showPage = 1;												//初始显示页码为1
}

void bk45Control()												//4,5对应操作相同
{
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)						
		{
			if (m.x <= 820 && m.x >= 720 && m.y <= 615 && m.y >= 565)
			{													//返回至功能选择
				picture_bk = 2;
				change_bk = 1;									
			}
		}
	}
}

void bk6Control()												//操作6
{
	if (isShowReslut)											//结果输出
	{
		showResult();
		isShowReslut = 0;
	}
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 820 && m.x >= 720 && m.y <= 615 && m.y >= 565)	
			{													//返回功能选择
				picture_bk = 2;
				change_bk = 1;
			}
			else if (m.x>=410&&m.x<=810&&m.y>=0&&m.y<=50)		//详细详细信息
			{
				picture_bk = 7;
				change_bk = 1;
			}
			else if (m.x>=682&&m.x<=810&&m.y>=510&&m.y<=570)	//下一页
			{
				picture_bk = 6;
				change_bk = 1;
				isShowReslut = 1;
				showPage++;										//页码加1
			}
			else if (m.x>=0&&m.x<=140&&m.y>=510&&m.y<=570)		//上一页	
			{
				picture_bk = 6;
				change_bk = 1;
				isShowReslut = 1;
				showPage--;
			}
			else if (m.x >= 210 && m.x <= 610 && m.y >= 570 && m.y <= 615)
			{													//重新查询
				picture_bk = 3;
				change_bk = 1;
			}
		}
	}
}

void bk7Control()												//操作7
{
	char pfind[100];											//输入框
	InputBox(pfind, 20, "请输入要查找的序号                               （不要多打回车哦）", NULL, 0, 300, 150);
	nfind = 0;
	if (*pfind != '0')
	{
		for (int i = 0; pfind[i] != '\0'; i++)					//转换为整型
			nfind = nfind * 10 + (int)(pfind[i] - '0');								
	}
	nfind--;													//下标为序号减一
	picture_bk = 8;												//显示详细信息
	change_bk = 1;
}

void bk8Control()
{
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x <= 820 && m.x >= 720 && m.y <= 615 && m.y >= 565)
			{
				picture_bk = 6;									//返回至结果显示
				change_bk = 1;
				isShowReslut = 1;
			}
		}
	}
	char s[5000];												//存储上下文
	int i=0;
	int k = 0;
	if (nfind<0||nfind >=text_index.size())						//输入的查询序号越界
	{
		settextstyle(20, 10, _T("黑体"));
		outtextxy(230, 230, "information you entered is incorrect");	
		picture_bk = 7;											
		change_bk = 1;
		Sleep(2500);											//提示信息停留两秒
		return;													//越界退出该函数
	}
	int j = text_index[nfind];
	if(j-1>=0)													//获取上下文，注意越界
		for (i = 0; i < save_book[j-1].content.length(); i++)
			s[i] = save_book[j-1].content[i];
	for (k=i; i-k < save_book[j].content.length();i++)
		s[i] = save_book[j].content[i-k];
	/*if(j+1<save_book.size())
		for (k = i; i - k < save_book[j+1].content.length(); i++)
			s[i] = save_book[j+1].content[i-k];*/
	s[i] = '\0';
	setbkmode(TRANSPARENT);										//设置显示字体背景透明
	RECT r = { 100,100 ,610,500 };								//显示框大小
	drawtext(s, &r, DT_WORDBREAK);								//显示格式
}
