#pragma once
using namespace std;
#include"table.h"
#define MAX_LENGTH 2000
#define MAX_WORD_NUM 200
#define tb_sum 20
#define kong -1
#define ERROR -1
#define guanjianzi 1
#define biaoshifu 2
#define shuzhi 3
#define fuzhihao 4
#define suanfu 5
#define jiefu 6
#define fengefu 7
#define zhushihao 8
#define zuokuohao 9
#define youkuohao 10
#define zuodakuohao 11
#define youdakuohao 12
#define zimu 13
#define shuzi 14
#define jieshufu 15

#define program 16
#define leixing 17
#define id 18
#define yujukuai 19
#define neibushengming 20
#define yujuchuan 21
#define neibubianliangshengming 22
#define yuju 23
#define ifyuju 24
#define whileyuju 25
#define returnyuju 26
#define fuzhiyuju 27
#define biaodashi 28
#define jiafabiaodashi 29
#define relop 30
#define xiang 31
#define yinzi 32
#define FTYPE 33
#define call 34
#define shicanliebiao 35
#define shican 36
#define xingcan 37
#define canshuliebiao 38
#define canshu 39
#define shengmingchuan 40
#define shengming 41
#define shengmingleixing 42
#define hanshushengming 43
#define bianliangshengming 44

string lookup(string s, table *tb1)//找到return 1,否则0
{
	for (int i = 0; i < tb_sum; i++)
	{
		if (s == tb1[i].name)
			return tb1[i].name;
	}
	return " ";
}



class gtree
{
public:
	//保留部分，添加属性信息
	string place;
	string _true;
	string _false;
	string begin;
	string end;
	string before;
	bool _before;
	string after;
	bool _after;
	//string code;
	//保留部分，添加属性信息
	gtree *next;//兄弟指针
	gtree *child;//孩子指针
	int type;//节点类型syn
	int type_1;//节点具体类型syn_1,针对算符和关键字
	void init();//初始化
	void expand(int num, gtree *t, int sum);
	void cnode(int syn, int syn_1, gtree *t);
	void newlable(int a);
	string newlable1(int a, int flag);
	//void gen(string a);
};


void gtree::init()
{
	this->type = program;
	this->type_1 = 0;
	this->child = NULL;
	this->next = NULL;
}

void gtree::cnode(int syn, int syn_1, gtree *t)
{
	t->next = new gtree;
	t->next->type = syn;
	t->next->type_1 = syn_1;
	t->next->child = NULL;
	t->next->next = NULL;
}

void gtree::newlable(int a)
{
	char b[3] = "0";
	if (a >= 10)
	{
		a = a - 10 + 'a' - '0';
	}
	b[0] += a;
	string c = b;
	place = "T" + c;
}

string gtree::newlable1(int a, int flag)
{
	char b[3] = "0";
	b[0] += a;
	string c = b;
	return("L" + c);
}

void gtree::expand(int num, gtree *t, int sum)//sum表示一些特殊的内容重复次数
{
	t->child = new gtree;
	gtree *t1 = t->child;
	t1->child = NULL;
	t1->next = NULL;

	if (num == program)
	{
		t1->type = shengmingchuan;
		t1->type_1 = 0;

	}
	else if (num == shengmingchuan)
	{
		t1->type = shengming;
		t1->type_1 = 0;
	}
	else if (num == shengming)
	{
		if (sum == 0)//sum=0表示int声明，sum=1表示void声明
		{
			t1->type = guanjianzi;
			t1->type_1 = 1;
			cnode(id, 0, t1);
			cnode(shengmingleixing, 0, t1->next);
		}
		else if(sum==1)
		{
			t1->type = guanjianzi;
			t1->type_1 = 2;
			cnode(id, 0, t1);
			cnode(hanshushengming, 0, t1->next);
		}
		else
		{
			cout << "tree_shengming_sum error!" << endl;
		}
	}
	else if (num == shengmingleixing)
	{
		if (sum == 0)//sum=0表示变量声明，sum=1表示函数声明
		{
			t1->type = bianliangshengming;
			t1->type_1 = 0;
		}
		else if (sum == 1)
		{
			t1->type = hanshushengming;
			t1->type_1 = 0;
		}
		else
		{
			cout << "tree_shengming_sum error!" << endl;
		}
	}
	else if (num == bianliangshengming)//sum=0代表无初值
	{
		if (sum == 0)
		{
			t1->type = jiefu;
			t1->type_1 = 0;
		}
		else
		{
			t1->type = fuzhihao;
			t1->type_1 = 0;
			cnode(biaodashi, 0, t1);
			cnode(jiefu, 0, t1->next);
		}
	}
	else if (num == hanshushengming)
	{
		t1->type = zuokuohao;
		t1->type_1 = 0;
		cnode(xingcan, 0, t1);
		cnode(youkuohao, 0, t1->next);
		cnode(yujukuai, 0, t1->next->next);
	}
	else if (num == xingcan)
	{
		if (sum == 0)//sum=0表示空，1表示参数列表
		{
			delete t1;
			t->child = NULL;
		}
		else if (sum == 1)
		{
			t1->type = canshuliebiao;
			t1->type_1 = 0;
		}
		else
		{
			cout << "error in tree_xingcan" << endl;
		}
	}
	else if (num == canshuliebiao)//sum传入重复{,参数}次数
	{
		t1->type = canshu;
		t1->type_1 = 0;
		/*for (int i = 0; i < sum; i++)
		{
		cnode(fengefu, 0, t1->next);
		cnode(canshu, 0, t1->next->next);
		}*/
	}
	else if (num == canshu)
	{
		t1->type = guanjianzi;
		t1->type_1 = 1;//int
		cnode(id, 0, t1);
	}
	else if (num == leixing)//sum=1表示int，sum=2表示void
	{
		t1->type = guanjianzi;
		if (sum == 1)
		{
			t1->type_1 = 1;//int
		}
		else if (sum == 2)
		{
			t1->type_1 = 2;//void
		}
		else
		{
			cout << "error in tree_leixing" << endl;
		}
	}
	else if (num == id)
	{
		t1->type = biaoshifu;
		t1->type_1 = 0;
	}
	else if (num == yujukuai)
	{
		t1->type = zuodakuohao;
		t1->type = 0;
		cnode(neibushengming, 0, t1);
		cnode(yujuchuan, 0, t1->next);
		cnode(youdakuohao, 0, t1->next->next);
	}
	else if (num == neibushengming)//sum=0表示空，sum=1表示内部变量声明
	{
		if (sum == 0)
		{
			delete t1;
			t->child = NULL;
		}
		else if (sum == 1)
		{
			t1->type = neibubianliangshengming;
			t1->type_1 = 0;
			cnode(jiefu, 0, t1);
		}
		else
		{
			cout << "error in tree_neibushengming" << endl;
		}
	}
	else if (num == neibubianliangshengming)//sum=0代表无初值
	{

		t1->type = guanjianzi;
		t1->type_1 = 1;//int
		cnode(id, 0, t1);

	}
	else if (num == yujuchuan)//loading...
	{
		t1->type = yuju;
		t1->type_1 = 0;
	}
	else if (num == yuju)//sum=1.2.3.4分别代表if while return 赋值语句
	{
		t1->type_1 = 0;
		if (sum == 1)
		{
			t1->type = ifyuju;
		}
		else if (sum == 2)
		{
			t1->type = whileyuju;
		}
		else if (sum == 3)
		{
			t1->type = returnyuju;
		}
		else if (sum == 4)
		{
			t1->type = fuzhiyuju;
		}
		else if (sum == 5)
		{
			t1->type = biaodashi;
		}
		else
		{
			cout << "error in tree_yuju" << endl;
		}
	}
	else if (num == fuzhiyuju)
	{
		t1->type = id;
		t1->type_1 = 0;
		cnode(fuzhihao, 0, t1);
		cnode(biaodashi, 0, t1->next);
		cnode(jiefu, 0, t1->next->next);
	}
	else if (num == returnyuju)
	{
		t1->type = guanjianzi;
		t1->type_1 = 6;//return
		if (sum == 1)//sum=0表示return;sum=1表示 return 表达式;
		{
			cnode(biaodashi, 0, t1);
			cnode(jiefu, 0, t1->next);
		}
		else
			cnode(jiefu, 0, t1);
	}
	else if (num == whileyuju)
	{
		t1->type = guanjianzi;
		t1->type_1 = 5;//while
		cnode(zuokuohao, 0, t1);
		cnode(biaodashi, 0, t1->next);
		cnode(youkuohao, 0, t1->next->next);
		cnode(yujukuai, 0, t1->next->next->next);
	}
	else if (num == ifyuju)//sum=1表示有else***
	{
		t1->type = guanjianzi;
		t1->type_1 = 3;//if
		cnode(zuokuohao, 0, t1);
		cnode(biaodashi, 0, t1->next);
		cnode(youkuohao, 0, t1->next->next);
		cnode(yujukuai, 0, t1->next->next->next);
		if (sum == 1)
		{
			cnode(guanjianzi, 4, t1->next->next->next->next);
			cnode(yujukuai, 0, t1->next->next->next->next->next);
		}
	}
	else if (num == biaodashi)
	{
		t1->type = jiafabiaodashi;
		t1->type_1 = 0;
		//
		//
	}
	else if (num == jiafabiaodashi)
	{
		t1->type = xiang;
		t1->type_1 = 0;
		//
		//
	}
	else if (num == xiang)
	{
		t1->type = yinzi;
		t1->type_1 = 0;
	}
	else if (num == yinzi)
	{
		if (sum == 1)//因子是数字
		{
			t1->type = shuzhi;
			t1->type_1 = 0;
		}
		else if (sum == 2)//因子是（表达式）
		{
			t1->type = zuokuohao;
			t1->type_1 = 0;
			cnode(biaodashi, 0, t1);
			cnode(youkuohao, 0, t1->next);
		}
		else if (sum == 3)//因子是ID FTYPE
		{
			t1->type = id;
			t1->type_1 = 0;
			cnode(FTYPE, 0, t1);
		}
	}
	else if (num == FTYPE)
	{
		if (sum == 0)//FYPE为空
		{
			delete t1;
			t->child = NULL;
		}
		else
		{
			t1->type = call;
			t1->type_1 = 0;
		}
	}
	else if (num == call)
	{
		t1->type = zuokuohao;
		t1->type_1 = 0;
		cnode(shican, 0, t1);
		cnode(youkuohao, 0, t1->next);
	}
	else if (num == shican)
	{
		if (sum == 0)//sum=0表示实参为空
		{
			delete t1;
			t->child = NULL;
		}
		else
		{
			t1->type = shicanliebiao;
			t1->type_1 = 0;
		}
	}
	else if (num == shicanliebiao)
	{
		t1->type = biaodashi;
		t1->type_1 = 0;
	}
	else
	{
		cout << "error in tree" << endl;
		exit(0);
	}
}
