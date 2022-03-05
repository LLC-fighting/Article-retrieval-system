#include<iostream>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
#include<cstring>
#pragma warning(disable : 4996)								//sprintf()������ʹ�ûᾯ�棬�ɽ��������
using namespace std;

int picture_bk;												//���Ʊ����л����ĸ�
bool change_bk;												//���Ʊ����л�
wchar_t tpattern[20];										//�������ַ���
char pattern[20];											//�������ַ���
bool isInput;												//�����Ƿ�����
int showPage;												//�����ʾҳ��
int dataNum;												//���ÿһҳ������
bool isShowReslut;											//�Ƿ���ʾ���
int nfind; 													//Ҫ���ҵ����

IMAGE img_background1;										//��ӭ����
IMAGE img_background2;										//����ѡ�񱳾�
IMAGE img_background3;										//��������
IMAGE img_background4;										//��������
IMAGE img_background5;										//���ڱ���
IMAGE img_background6;										//��ʾ�������
IMAGE img_background7;										//��ѯ��ϸ��Ϣ����
IMAGE img_background8;										//��ϸ��Ϣ��ʾ����

struct Text													//����һ�е���Ϣ
{
	string content;
	string bookName;
	int page=0;
	int chapter=0;
};

vector<Text>save_book;										//����
vector<int>text_index;										//��ѯ�����±�

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
	startup();													//��ʼ��
	readBook();													//����txt
	while (1)
	{
		show();
		control();
	}
}

void startup()													//���ݳ�ʼ��
{
	picture_bk = 1;
	change_bk = 1;
	isInput = 1;
	showPage = 1;
	dataNum = 13;
	initgraph(820, 615);										//���廭����С
	loadimage(&img_background1, _T(".\\picture\\background2.jpg"));//����ͼƬ
	loadimage(&img_background2, _T(".\\picture\\background3.jpg"));
	loadimage(&img_background3, _T(".\\picture\\background4.jpg"));
	loadimage(&img_background4, _T(".\\picture\\background5.jpg"));
	loadimage(&img_background5, _T(".\\picture\\background6.jpg"));
	loadimage(&img_background6, _T(".\\picture\\background7.jpg"));
	loadimage(&img_background7, _T(".\\picture\\background4.jpg"));
	loadimage(&img_background8, _T(".\\picture\\background4.jpg"));


}

void readBook()													//��ȡ�����������
{
	getText("J.K. Rowling - HP 3 - Harry Potter and the Prisoner of Azkaban");
}																//�ظ����������ú���ʵ��

void getText(string book_name)									//��ȡһ���������
{
	string pname = book_name + ".txt";							//������չ��
	ifstream fs(pname.c_str());									//��txt�ļ�
	if (!fs)													//��ʧ�ܣ��˳�
	{
		cout << "�ı���ȡ����!!!" << endl;
		exit(1);
	}
	Text temp_text;												//��ʱ�洢text
	temp_text.bookName = book_name;								//��������
	temp_text.chapter = 0;										//��ʼΪ0
	temp_text.page = 1;											//��Ϊҳ�������ʾ����ʼΪ1
	string line;
	while (getline(fs, line))									//���ж���
	{
		int len = line.length();
		if (len <= 3 && line[0] >= '1' && line[0] <= '9')		//ҳ��
		{
			temp_text.page++;
			continue;
		}
		else if (len < 20)										//�ж��Ƿ����½ڱ��
			if (line.substr(0, 7) == "chapter" || line.substr(0, 7) == "CHAPTER")
			{
				temp_text.chapter++;
				continue;
			}
		if (len < 3)
			continue;
		temp_text.content = line;
		save_book.push_back(temp_text);							//����ҳ����½ڱ�ţ���������
	}
}

void search()													//�����ַ���
{
	int tot = save_book.size();									//�ļ�������
	int cnt = 0;
	string::size_type nnpos = string::npos;						//δ���ҵ��ķ���ֵ
	while (cnt < tot)
	{
		string::size_type position = save_book[cnt].content.find(pattern);
		if (position != nnpos)									//���ҵ������������
			text_index.push_back(cnt);
		cnt++;
	}
}

void showResult()
{
	int cnt = dataNum * (showPage - 1);							//��ҳ����ʼ�𰸱��
	int tot = dataNum * showPage;								//ĩβ���
	if (tot > text_index.size())								//���ܸ�ҳ����������Խ��
		tot = text_index.size();
	if (text_index.size()== 0)									//����Ϊ0��˵��û�в��ҵ�
	{
		settextstyle(80, 40, _T("����"));						//���������ʽ
		outtextxy(240, 270, "Not found!");						//ָ��λ�����
		return;
	}
	else if (tot <= 0||tot<cnt||cnt<0)							//������һҳ֮ǰ�����һҳ֮��
	{
		settextstyle(40, 20, _T("����"));
		outtextxy(100, 240, "Please return to the previous page");
		return;
	}
	int distance = 30;											//ÿһ������ļ��
	for (int i = cnt; i < tot; i++)
	{
		int pos = text_index[i];
		TCHAR s[30];
		_stprintf(s, _T("%d"),i+1);								//�����ת��ΪTCHAR��
		settextstyle(20, 10, _T("����"));
		outtextxy(10, 90+distance*(i-cnt), s);					//ָ��λ�����
		outtextxy(70, 90+distance * (i - cnt), pattern);
		_stprintf(s, _T("%d"),save_book[pos].page );
		outtextxy(160, 90+distance * (i - cnt), s);
		_stprintf(s, _T("%d"), save_book[pos].chapter);
		outtextxy(215, 90+distance * (i - cnt), s);
		char ss[150];
		int j;
		string str = save_book[i].bookName;
		for (j = 0; j < str.length(); j++)						//stingת��Ϊchar������
			ss[j] = str[j];
		ss[j] = '\0';
		settextstyle(16, 8, _T("����"));
		outtextxy(290, 90 + distance * (i - cnt), ss);
	}
}

void show()
{
	if (change_bk)												//�Ƿ���Ҫ��������
	{
		backgroundShow();										//��������
		change_bk = 0;											//���Ʊ�������
	}
}

void backgroundShow()											//�����л�
{
	if (picture_bk == 1)										//��ӭ����
	{
		putimage(0, 0, &img_background1);						//Ӧ��ͼƬ
		Sleep(2000);											//ͣ��2s
		picture_bk = 2;											//ֱ���л�������2
	}
	if (picture_bk == 2)										//ѡ���ܱ���
		putimage(0, 0, &img_background2);						
	else if (picture_bk == 3)									//��������
		putimage(0, 0, &img_background3);
	else if (picture_bk == 4)									//��������
		putimage(0, 0, &img_background4);
	else if (picture_bk == 5)									//���ڱ���
		putimage(0, 0, &img_background5);
	else if (picture_bk == 6)									//�������
		putimage(0, 0, &img_background6);
	else if (picture_bk == 7)									//��ѯ����
		putimage(0, 0, &img_background7);
	else if (picture_bk == 8)									//��ϸ����
		putimage(0, 0, &img_background8);
}

void control()													//����
{
	if (picture_bk == 2)										//ÿ��������Ӧ����
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

void bk2Control()												//����2
{
	MOUSEMSG m;													//�����Ϣ
	if (MouseHit())												//��궯��
	{
		m = GetMouseMsg();										//��ȡ�����Ϣ
		if (m.uMsg == WM_LBUTTONDOWN)							//�������
		{
			if (m.x >= 300 && m.x <= 520 && m.y <= 260 && m.y >= 210)
			{													//���ָ��λ��
				picture_bk = 3;									//����
				change_bk = 1;									//���¿��Ʊ���
			}
			else if (m.x >= 300 && m.x <= 520 && m.y <= 325 && m.y >= 280)
			{													
				picture_bk = 4;									//����
				change_bk = 1;
			}
			else if (m.x >= 360 && m.x <= 460 && m.y <= 400 && m.y >= 350)
			{
				picture_bk = 5;									//����
				change_bk = 1;
			}
			else if (m.x <= 820 && m.x >= 720 && m.y <= 615 && m.y >= 565)
				exit(1);										//�˳�
		}
	}
}

void bk3Control()												//����3
{																//�����
	InputBox(pattern,20, "������Ҫ���ҵ�����/����                      ����Ҫ���س�Ŷ��",NULL,NULL,300,150);
	text_index.clear();											//�����һ�ν���±�
	search();													//����
	picture_bk = 6;												//�����������ʾ����
	change_bk = 1;
	isShowReslut = 1;											//���Ʊ�������
	showPage = 1;												//��ʼ��ʾҳ��Ϊ1
}

void bk45Control()												//4,5��Ӧ������ͬ
{
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)						
		{
			if (m.x <= 820 && m.x >= 720 && m.y <= 615 && m.y >= 565)
			{													//����������ѡ��
				picture_bk = 2;
				change_bk = 1;									
			}
		}
	}
}

void bk6Control()												//����6
{
	if (isShowReslut)											//������
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
			{													//���ع���ѡ��
				picture_bk = 2;
				change_bk = 1;
			}
			else if (m.x>=410&&m.x<=810&&m.y>=0&&m.y<=50)		//��ϸ��ϸ��Ϣ
			{
				picture_bk = 7;
				change_bk = 1;
			}
			else if (m.x>=682&&m.x<=810&&m.y>=510&&m.y<=570)	//��һҳ
			{
				picture_bk = 6;
				change_bk = 1;
				isShowReslut = 1;
				showPage++;										//ҳ���1
			}
			else if (m.x>=0&&m.x<=140&&m.y>=510&&m.y<=570)		//��һҳ	
			{
				picture_bk = 6;
				change_bk = 1;
				isShowReslut = 1;
				showPage--;
			}
			else if (m.x >= 210 && m.x <= 610 && m.y >= 570 && m.y <= 615)
			{													//���²�ѯ
				picture_bk = 3;
				change_bk = 1;
			}
		}
	}
}

void bk7Control()												//����7
{
	char pfind[100];											//�����
	InputBox(pfind, 20, "������Ҫ���ҵ����                               ����Ҫ���س�Ŷ��", NULL, 0, 300, 150);
	nfind = 0;
	if (*pfind != '0')
	{
		for (int i = 0; pfind[i] != '\0'; i++)					//ת��Ϊ����
			nfind = nfind * 10 + (int)(pfind[i] - '0');								
	}
	nfind--;													//�±�Ϊ��ż�һ
	picture_bk = 8;												//��ʾ��ϸ��Ϣ
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
				picture_bk = 6;									//�����������ʾ
				change_bk = 1;
				isShowReslut = 1;
			}
		}
	}
	char s[5000];												//�洢������
	int i=0;
	int k = 0;
	if (nfind<0||nfind >=text_index.size())						//����Ĳ�ѯ���Խ��
	{
		settextstyle(20, 10, _T("����"));
		outtextxy(230, 230, "information you entered is incorrect");	
		picture_bk = 7;											
		change_bk = 1;
		Sleep(2500);											//��ʾ��Ϣͣ������
		return;													//Խ���˳��ú���
	}
	int j = text_index[nfind];
	if(j-1>=0)													//��ȡ�����ģ�ע��Խ��
		for (i = 0; i < save_book[j-1].content.length(); i++)
			s[i] = save_book[j-1].content[i];
	for (k=i; i-k < save_book[j].content.length();i++)
		s[i] = save_book[j].content[i-k];
	/*if(j+1<save_book.size())
		for (k = i; i - k < save_book[j+1].content.length(); i++)
			s[i] = save_book[j+1].content[i-k];*/
	s[i] = '\0';
	setbkmode(TRANSPARENT);										//������ʾ���屳��͸��
	RECT r = { 100,100 ,610,500 };								//��ʾ���С
	drawtext(s, &r, DT_WORDBREAK);								//��ʾ��ʽ
}
