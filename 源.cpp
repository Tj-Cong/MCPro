#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>  
#include <string.h>
#include<string>
#include <iomanip>
#include<fstream>
#include<stack>
#include"tree.h"
#include"table.h"
#include"cpn.h"
#include"RG.h"
using namespace std;

table tb1[tb_sum];//符号表，最多纪录20(tb_sum)个变量
int tb_num = 0;
int nextnum = 100;//标号(从100开始)
int labelnum = 1;
int labelnum1 = 1;
gtree *tree;
int offset1 = 0;
char prog[MAX_LENGTH], token[20], ch;//缓冲区最大大小100，标识符最长不超过20
int store[MAX_WORD_NUM][2];
char store_char[MAX_WORD_NUM][20];
int syn, syn_1, p, m, n, sum, store_num, read_num;
string temp = "", temp1 = "";//temp和temp1用于存放表达式和赋值语句的place
C_Petri petri;
int gen_P_num = 0, gen_T_num = 0;//生成库所和变迁名的下标
int gen_if_num = 0, gen_while_num = 0;
char *rwtab[6] = { (char*)("int"),(char*)("void"),(char*)("if"),(char*)("else"),(char*)("while"),(char*)("return") };
char *caltab[10] = { (char*)("<="),(char*)("<"),(char*)(">="),(char*)(">"),(char*)("+"),(char*)("-"),(char*)("!="),(char*)("=="),(char*)("*"),(char*)("/" )};
char *type[15] = { (char*)("关键字"),(char*)("标识符"),(char*)("数值"),(char*)("赋值号"),(char*)("算符"),(char*)("界符"),(char*)("分隔符"),(char*)("注释号"),(char*)("左括号"),(char*)("右括号"),
(char*)("左大括号"),(char*)("右大括号"),(char*)("字母"),(char*)("数字"),(char*)("结束符") };
string lastT, preT;//lastT代表上一句的变迁，preT代表上一个位置的变迁
void scaner(void);
void analysis_yufa();
void trans_print(int num, int n, bool flag);
void emit(string op, string arg1, string arg2, string result, int num, int flag)
{
	if (flag == 0)
		cout << num << ":(" << op << "  ," << arg1 << "  ," << arg2 << "  ," << result << ')' << endl;
	else if (flag == 1)
		cout << num << ": " << "  if " << arg1 << "  " << op << "  " << arg2 << "  goto " << result << endl;
	else if (flag == 2)
		cout << num << ": " << "goto " << result << endl;
	else
	{
	};
	nextnum++;

	ofstream out;
	out.open("out.txt", ios::app);
	if (flag == 0)
		out << num << ":(" << op << "  ," << arg1 << "  ," << arg2 << "  ," << result << ')' << endl;
	else if (flag == 1)
		out << num << ": " << "  if " << arg1 << "  " << op << "  " << arg2 << "  goto " << result << endl;
	else if (flag == 2)
		out << num << ": " << "goto " << result << endl;
	else
	{
	};
	out.close();
}
string gen_if()
{
	return "if" + to_string(gen_if_num++);
}
string gen_while()
{
	return "while" + to_string(gen_while_num++);
}
void TraverseTree1(gtree *p,string func)//先第一遍遍历语法树，对place值进行更新
{
	if (p == NULL) return;
	//trans_print(p->type, 1, true);

	if (p->type == suanfu)//算符place=算符本身
	{
		switch (p->type_1)
		{
		case 1:
			p->place = "<=";
			break;
		case 2:
			p->place = "<";
			break;
		case 3:
			p->place = ">=";
			break;
		case 4:
			p->place = ">";
			break;
		case 5:
			p->place = "+";
			break;
		case 6:
			p->place = "-";
			break;
		case 7:
			p->place = "!=";
			break;
		case 8:
			p->place = "==";
			break;
		case 9:
			p->place = "*";
			break;
		case 10:
			p->place = "/";
			break;
		}
	}
	else if (p->type == fuzhihao)//赋值号place=本身
		p->place = "=";
	else if (p->type == zuokuohao)
		p->place = "(";
	else if (p->type == youkuohao)
		p->place = ")";
	else if (p->type == hanshushengming)//函数声明定义命名空间
		func = p->place;
	else if (p->type == neibubianliangshengming)//内部变量声明place需要带有函数前缀
		p->place = func + '.' + p->place;
	else if (p->type == shengming)
	{
		if (p->child->type == guanjianzi && p->child->type_1 == 2)//void
		{
			p->child->next->next->place = p->child->next->place;
		}
		else if (p->child->type == guanjianzi && p->child->type_1 == 1)//int
		{
			p->child->next->next->child->place = p->child->next->place;
		}
	}

	TraverseTree1(p->child, func);
	TraverseTree1(p->next, func);

}
string TraverseTree2(gtree *p)//对表达式进行操作
{
	string child, next, temp;
	if (p == NULL)return "";
	if (p->type == suanfu || p->type == id || p->type == yinzi || p->type == zuokuohao || p->type == youkuohao)
		temp = p->place;
	child = TraverseTree2(p->child);
	next = TraverseTree2(p->next);

	if (p->type == biaodashi)
		p->place = child ;
	return child + temp + next;
}
void TraverseTree3(gtree *p)
{
	if (p == NULL)return;
	if (p->type == fuzhiyuju)
	{
		p->place = p->child->place + '=' + p->child->next->next->place;
	}
	else if (p->type == ifyuju)
	{
		p->place = gen_if();
	}
	else if (p->type == whileyuju)
	{
		p->place = gen_while();
	}
	TraverseTree3(p->child);
	TraverseTree3(p->next);
}
void Traverse(gtree *p)
{
	if (p == NULL) return;
	if (p->type == fuzhiyuju)
		cout << p->place << endl;
	Traverse(p->child);
	Traverse(p->next);
}
string gen_P()
{
	string temp = "";
	return temp + 'P' + to_string(gen_P_num++);
	
}
string gen_T()
{
	string temp = "";
	return temp + 'T' + to_string(gen_T_num++);
}

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
void splitExpression(string &s, vector<string>& v)//传入字符串和结果集合，将字符串根据>,<等分割并放入结果集
{
	string s1;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == '(' || s[i] == ')')
			continue;
		if (s[i] == '<' || s[i] == '>')
		{
			if (s[i + 1] == '=')
				i++;
			v.push_back(s1);
			s1.clear();
		}
		else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
		{
			v.push_back(s1);
			s1.clear();
		}
		else if (s[i] == '!' || s[i] == '=')
		{
			if (s[i + 1] == '=')
				i++;
			else
				cout << "出现异常!=或==" << endl;
			v.push_back(s1);
			s1.clear();
		}
		else
			s1 = s1 + s[i];
	}
	v.push_back(s1);
}

bool find_P(C_Petri petri, string P1)
{
	for (int i = petri.p_num - 1; i >= 0; i--)
	{
		
		string s = petri.place[i].v_name;
		vector<string> v;
		SplitString(s, v, ".");
		int pos = v.size() - 1;
		if (pos>=0 && v[pos] == P1)
			return true;
	}
	return false;
}
string find_P_name(C_Petri petri,string v_name)//通过变量名v_name找库所名name
{
	for (int i = petri.p_num-1; i >=0; i--)
	{
		string s = petri.place[i].v_name;
		vector<string> v;
		SplitString(s, v, ".");
		
		int pos = v.size() - 1;
		if (pos>=0 && v[pos] == v_name)
			return petri.place[i].name;
	}
	cout << "can't find P" << endl;
	return "";
}
string find_T_name(C_Petri petri, string v_name)//通过表达式名v_name找变迁名name
{
	for (int i = 0; i < petri.t_num; i++)
	{
		string s = petri.transition[i].v_Expression;
		string s1= petri.transition[i].booleanExpression;
		if (s == v_name || s1 == v_name)
			return petri.transition[i].name;
	}
	cout << "can't find T" << endl;
	return "";
}
void find_P_connect_T(C_Petri petri, string v_name, vector<string> &v)
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].sourceP == true)
		{
			if (petri.arc[i].source == v_name)
			{
				v.push_back(petri.arc[i].target);
			}
		}
	}
}

int string_replace(string &s1, const string &s2, const string &s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
	return 0;
}

string opposite(string s)
{
	if (s.find(">=") != string::npos)
		string_replace(s, ">=", "<");
	else if (s.find("<=") != string::npos)
		string_replace(s, "<=", ">");
	else if (s.find("<") != string::npos)
		string_replace(s, "<", ">=");
	else if (s.find(">") != string::npos)
		string_replace(s, ">", "<=");
	else if (s.find("==") != string::npos)
		string_replace(s, "==", "!=");
	else if (s.find("!=") != string::npos)
		string_replace(s, "!=", "==");
	return s;

}
string find_first_yuju(gtree *p)
{
	if (p == NULL) return "";
	if (p->type == yuju)
		return p->child->place;
	string child = find_first_yuju(p->child);
	string next = find_first_yuju(p->next);
	if (child != "")
		return child;
	if (next != "")
		return next;
	return "";
}
string find_last_yuju(gtree *p)
{
	if (p == NULL) return "";
	if (p->type == yuju)
		return p->child->place;
	string child = find_last_yuju(p->child);
	string next = find_last_yuju(p->next);
	if (next != "")
		return next;
	if (child != "")
		return child;
	return "";
}
gtree* find_yujuchuan(gtree *&p)
{
	if (p == NULL) return NULL;
	if (p->type == yujuchuan)
	{
		return p;
	}
	gtree* child = find_yujuchuan(p->child);
	gtree* next = find_yujuchuan(p->next);
	if (child)
		return child;
	if (next)
		return next;
	return NULL;
}

void create_connect(string T, string express)//给定变迁和表达式建立表达式中所有库所与变迁的联系
{
	string P2;
	vector<string> v;
	string V;
	bool sourceP;
	splitExpression(express, v);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (v[i][0] == '_' || (v[i][0] >= 'a'&&v[i][0] <= 'z'))
		{
			P2 = find_P_name(petri, v[i]);
			V = v[i];
			sourceP = true;
			petri.Add_Arc(P2, T, V, sourceP);
			sourceP = false;
//			V = "";
			petri.Add_Arc(T, P2, V, sourceP);
		}
	}
}
void inside_block(gtree *tree1,string T)//语句块内建模
{
	gtree *node = find_yujuchuan(tree1);
	gtree *tr = node->child;
	vector<string> now;
	vector<string> last;
	while (tr)
	{
		now.clear();
		if (tr->child->type != returnyuju && tr->child->type != biaodashi)//暂时不考虑过程调用
		{
			bool control_P, t, tag;
			int n1 = 0;
			double d = 0.0;
			string V = "", V_name = "";
			string s = "";
			string _P;
			bool sourceP = false;
			
			string temp_P;

			_P = find_P_name(petri, tr->child->place);

			petri.Add_Arc(T, _P, V, sourceP);
			if (tr->child->type == fuzhiyuju)
				now.push_back(find_T_name(petri, tr->child->place));
			else if (tr->child->type == ifyuju)
			{
				vector<string> v;
				temp_P = find_P_name(petri, tr->child->place);
				now = petri.get_enter(temp_P);
			}
			else if (tr->child->type == whileyuju)
			{
				vector<string> v;
				temp_P = find_P_name(petri, tr->child->place);
				now = petri.get_enter(temp_P);
			}

			if (last.size() != 0)
			{
				string newP = gen_P();
				V_name = "executedP";
				tag = 0;
				control_P = true;
				t = false;
				petri.Add_Place(newP, V_name, tag, control_P, t, n1, d, s);
				V = "executed";
				sourceP = false;
				for (unsigned int i = 0; i < last.size(); i++)
					petri.Add_Arc(last[i], newP, V, sourceP);
				sourceP = true;
				for (unsigned int i = 0; i < now.size(); i++)
					petri.Add_Arc(newP, now[i], V, sourceP);
				if (tr->child->type == whileyuju)
				{
					string while_name = tr->child->place;
					for (int i = 0; i < petri.p_num; i++)
					{
						if (petri.place[i].v_name == while_name)
						{
							//string enter = petri.place[i].enter[0];
							for (unsigned int j = 0; j < petri.place[i].exit.size()-1; j++)
							{
								V = "";
								sourceP = false;
								petri.Add_Arc(petri.place[i].exit[j], newP, V, sourceP);
							}
							break;
						}
					}
				}

			}
			if (tr->child->type == fuzhiyuju)
				last = now;
			else if (tr->child->type == ifyuju)
			{
				last = petri.get_exit(temp_P);
			}
			else if (tr->child->type == whileyuju)//whil语句的真实出口只有exit[-1]
			{
				last.clear();
				vector<string> temp_last = petri.get_exit(temp_P);
				last.push_back(temp_last[temp_last.size()-1]);
			}
		}
		tr = tr->next;
	}
}


int tag = 0;
bool control_P = true;
bool control_T = true;
bool t = false;
bool sourceP = true;
int n1 = 0;
double d = 0.0;
string V_name = "";
string s = "";
string P1 = "";
string T = "";
string V = "";
void declaration(gtree *p)
{
	if (p == NULL) return;
	tag = 0;
	control_P = true;
	control_T = true;
	t = false;
	sourceP = true;
	n1 = 0;
	d = 0.0;
	V_name = "";
	s = "";
	P1 = "";
	T = "";
	V = "";
	if (p->type == neibubianliangshengming)
	{
		control_P = false;
		V_name = p->place;
		//cout << p->place << endl;
		//getchar();
		if (p->child->type == guanjianzi && p->child->type_1 == 1)//此处先添加int的,其余再补上
		{
			tag = 0;//0代表int
			if (p->child->next->next)//有初始化
			{
				t = true;
				//暂时先不处理用变量初始化的情况
				//
				//
				n1 = atoi(p->child->next->next->next->place.c_str());
			}
			else
				t = false;
		}
		else
			cout << "can't find type!" << endl;

		P1 = gen_P();
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s);


	}
	else if (p->type == shengming)
	{
		control_P = true;
		control_T = false;
		tag = 0;
		t = false;
		sourceP = true;
		if (p->child->next->next->type == hanshushengming || (p->child->next->next->child&&p->child->next->next->child->type == hanshushengming))
		{
			P1 = gen_P();
			T = gen_T();
			V_name = p->child->next->place;
			petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s);
			s = p->child->next->place + " begin";
			petri.Add_Transition(T, control_T, s);//begin前有空格
			petri.Add_Arc(P1, T, V, sourceP);
		}
		else if ((p->child->next->next->child) && (p->child->next->next->child->type == bianliangshengming))
		{
			control_P = false;
			if (p->child->type == guanjianzi && p->child->type_1 == 1)//此处先添加int的,其余再补上
			{
				tag = 0;//0代表int
				if (p->child->next->next->child->child->type != jiefu)//有初始化
				{
					t = true;
					//暂时先不处理用变量初始化的情况
					//
					//
					n1 = atoi(p->child->next->next->child->child->next->place.c_str());
				}
				else
					t = false;
			}
			else
				cout << "can't find type!" << endl;
			P1 = gen_P();
			V_name = p->child->next->place;
			petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s);
		}
		else
		{
			cout << "shengming error!" << endl;
		}
	}

	declaration(p->child);
	declaration(p->next);
	if (p->type == fuzhiyuju)
	{

		if (find_P(petri, p->child->place))
		{
			P1 = gen_P();
			control_P = true;
			t = false;
			V_name = p->place;
			petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s);
			
			
			T = gen_T();
			control_T = false;
			s = p->place;
			petri.Add_Transition(T, control_T, s);

			sourceP = true;
			petri.Add_Arc(P1, T, V, sourceP);


			create_connect(T, p->child->place);
			create_connect(T, p->child->next->next->place);
			string P2 = find_P_name(petri, p->child->place);
			//cout << "P2=" << (P2=="P0") << endl;
			
			V = p->child->next->next->place;
			sourceP = false;
			petri.Add_Arc(T, P2, V, sourceP);
			
			//向语句中加入enter和exit
			petri.Add_Place_enter(P1, T);
			petri.Add_Place_exit(P1, T);
		}
		else
		{
			cout << "找不到" << p->child->place << endl;
			return;
		}

	}
	else if (p->type == ifyuju)
	{

		//默认程序正确
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = p->place;
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s);
		//string P2 = gen_P();
		//petri.Add_Place(P2, V_name, tag, control_P, t, n1, d, s);
		//string P3 = gen_P();
		//petri.Add_Place(P3, V_name, tag, control_P, t, n1, d, s);

		T = gen_T();
		control_T = true;
		s = p->child->next->next->place;
		petri.Add_Transition(T, control_T, s);
		string T1 = gen_T();
		create_connect(T, s);
		s = opposite(s);
		petri.Add_Transition(T1, control_T, s);
		create_connect(T1, s);
		string T2 = "";
		string T3 = "";
		//string p2 = find_first_yuju(p->child->next->next->next->next);//找if里语句块的第一个表达式
		//string P2 = find_P_name(petri,p2);
		string t2 = find_last_yuju(p->child->next->next->next->next->child);

		T2 = find_P_name(petri, t2);//这里T2其实是P
		vector<string> temp_v1 = petri.get_exit(T2);
		vector<string> temp_v2;


		inside_block(p->child->next->next->next->next, T);

		string t3;
		string P3;
		if (p->child->next->next->next->next->next)
		{
			string p3 = find_first_yuju(p->child->next->next->next->next->next->next);//找else里语句块的第一个表达式
			P3 = find_P_name(petri, p3);
			t3 = find_last_yuju(p->child->next->next->next->next->next->next->child);
			T3 = find_P_name(petri, t3);//这里T3其实是P
			temp_v2 = petri.get_exit(T3);
			inside_block(p->child->next->next->next->next->next->next, T1);
		}
		sourceP = true;
		petri.Add_Arc(P1, T, V, sourceP);
		petri.Add_Arc(P1, T1, V, sourceP);

		//向语句中加入enter和exit
		petri.Add_Place_enter(P1, T);
		petri.Add_Place_enter(P1, T1);
		petri.Add_Place_exit(P1, temp_v1);
		if (p->child->next->next->next->next->next)
			petri.Add_Place_exit(P1, temp_v2);
		else
			petri.Add_Place_exit(P1, T1);
		
	}
	else if (p->type == whileyuju)
	{
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = p->place;
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s);
		T = gen_T();
		control_T = true;
		string s = p->child->next->next->place;
		petri.Add_Transition(T, control_T, s);
		string T1 = gen_T();
		create_connect(T, s);
		s = opposite(s);
		petri.Add_Transition(T1, control_T, s);
		create_connect(T1, s);
		string T2 = "";
		string T3 = "";
		//string p2 = find_first_yuju(p->child->next->next->next->next);//找if里语句块的第一个表达式
		//string P2 = find_P_name(petri,p2);
		string t2 = find_last_yuju(p->child->next->next->next->next);

		T2 = find_P_name(petri, t2);//这里T2其实是P
		vector<string> temp_v1 = petri.get_exit(T2);
		for (unsigned int i = 0; i < temp_v1.size(); i++)
		{
			V = "";
			sourceP = false;
			petri.Add_Arc(temp_v1[i], P1, V, sourceP);
			
		}

		inside_block(p->child->next->next->next->next, T);

		sourceP = true;
		petri.Add_Arc(P1, T, V, sourceP);
		petri.Add_Arc(P1, T1, V, sourceP);

		//向语句中加入enter和exit
		petri.Add_Place_enter(P1, T);
		petri.Add_Place_enter(P1, T1);
		petri.Add_Place_exit(P1, temp_v1);//这里temp_v1其实不是真实出口，只有T1是while语句的出口
		petri.Add_Place_exit(P1, T1);
	}
	else if (p->type == hanshushengming)
	{
		string func = p->place + " begin";
		T = find_T_name(petri,func);
		inside_block(p, T);
	}
	
}
void TraverseTree(gtree *p)
{
	string s1;
	if (p == NULL) return;
	//cout << p->type << endl;
	if (p->type == fuzhiyuju)
	{
		s1 = lookup(p->child->place, tb1);
		if (s1 == " ")
		{
			cout << "未定义元素" << p->child->place << endl;
			exit(0);
		}
	}
	else if (p->type == yinzi)
	{
		if (p->child->type == id)
		{
			s1 = lookup(p->child->child->place, tb1);
			if (s1 == " ")
			{
				cout << "未定义元素" << p->child->child->place << endl;
				exit(0);
			}
		}
	}
	else if (p->type == whileyuju)
	{
		//cout << "while1" << endl;

		p->end = p->newlable1(labelnum1++, 1);
		p->begin = p->newlable1(labelnum1++, 1);
		p->child->next->next->_true = p->newlable1(labelnum1++, 1);
		p->child->next->next->_false = p->end;
		p->child->next->next->next->next->end = p->begin;
		string s;
		s = p->begin + ":";
		p->child->next->next->before = s;
		p->child->next->next->_before = 1;
		s = p->child->next->next->_true + ":";
		p->child->next->next->next->next->before = s;
		p->child->next->next->next->next->_before = 1;
	}
	else if (p->type == ifyuju)
	{
		if (p->child->next->next->next->next->next == NULL)//没有else
		{
			p->end = p->newlable1(labelnum1++, 1);
			p->child->next->next->_true = p->newlable1(labelnum1++, 1);
			p->child->next->next->_false = p->end;
			string s;
			s = p->child->next->next->_true + ":";
			p->child->next->next->next->next->before = s;
			p->child->next->next->next->next->_before = 1;
		}
		else
		{
			p->end = p->newlable1(labelnum1++, 1);
			p->child->next->next->_true = p->newlable1(labelnum1++, 1);
			p->child->next->next->_false = p->newlable1(labelnum1++, 1);
			p->child->next->next->next->next->end = p->end;
			p->child->next->next->next->next->next->next->end = p->end;
			string s;
			s = p->child->next->next->_true + ":";
			p->child->next->next->next->next->before = s;
			p->child->next->next->next->next->_before = 1;

			p->child->next->next->next->next->next->end = p->end;//else语句专用

			s = p->child->next->next->_false + ":";
			p->child->next->next->next->next->next->next->before = s;
			p->child->next->next->next->next->next->next->_before = 1;
		}
	}
	else if (p->type == guanjianzi&&p->type_1 == 4)
	{
		emit(" ", " ", " ", p->end, nextnum, 2);
	}
	else if (p->type == jiafabiaodashi)
	{
		if (p->child->next)
			p->newlable(labelnum++);
		else
			p->place = p->child->child->place;
	}
	else if (p->type == xiang)
	{
		if (p->child->next)
			p->newlable(labelnum++);
		else
			p->place = p->child->place;
	}
	if (p->_before)
	{
		ofstream out;
		out.open("out.txt", ios::app);
		out << p->before << endl;
		out.close();
		cout << p->before << endl;
	}

	TraverseTree(p->child);
	//TraverseTree(p->next);


	if (p->type == fuzhiyuju)
	{
		//cout << "fuzhi" << endl;
		emit(":=", p->child->next->next->child->place, "-", s1, nextnum, 0);
	}
	else if (p->type == whileyuju)
	{
		//cout << "while" << endl;
		emit(" ", " ", " ", p->begin, nextnum, 2);
		ofstream out;
		out.open("out.txt", ios::app);
		out << p->end << endl;
		out.close();
		cout << p->end << ":" << endl;
	}
	else if (p->type == ifyuju)
	{
		ofstream out;
		out.open("out.txt", ios::app);
		out << p->end << endl;
		out.close();
		cout << p->end << ":" << endl;
	}
	else if (p->type == biaodashi)
	{
		if (p->child->next != NULL)
		{
			s1 = caltab[p->child->next->type_1 - 1];
			emit(s1, p->child->child->place, p->child->next->next->child->place, p->_true, nextnum, 1);
			emit("j", "-", "-", p->_false, nextnum, 2);
		}
	}
	else if (p->type == jiafabiaodashi)
	{
		if (p->child->next != NULL&&p->child->next->type == suanfu && (p->child->next->type_1 == 5))//+
		{
			emit("+", p->child->place, p->child->next->next->place, p->place, nextnum, 0);

		}
		else if (p->child->next != NULL&&p->child->next->type == suanfu && (p->child->next->type_1 == 6))//-
		{
			emit("-", p->child->place, p->child->next->next->place, p->place, nextnum, 0);
		}
		else//单一
		{
			//emit(":=", p->child->place, "-", p->place, nextnum, 0);
		}
		gtree *tempp = NULL;
		if (p->child->next)
			if (p->child->next->next->next)
				tempp = p->child->next->next->next;
		while (tempp)
		{
			if (tempp->type == suanfu&&tempp->type_1 == 5)//+
			{
				string temps = p->place;
				p->newlable(labelnum++);
				emit("+", temps, tempp->next->place, p->place, nextnum, 0);
			}
			else if (tempp->type == suanfu&&tempp->type_1 == 6)//-
			{
				string temps = p->place;
				p->newlable(labelnum++);
				emit("-", temps, tempp->next->place, p->place, nextnum, 0);
			}

			tempp = tempp->next->next;
		}
	}
	else if (p->type == xiang)
	{
		if (p->child->next != NULL&&p->child->next->type == suanfu && (p->child->next->type_1 == 9))//*
		{
			emit("*", p->child->place, p->child->next->next->place, p->place, nextnum, 0);
		}
		else if (p->child->next != NULL&&p->child->next->type == suanfu && (p->child->next->type_1 == 10))// /
		{
			emit("/", p->child->place, p->child->next->next->place, p->place, nextnum, 0);
		}
		else
		{
			//emit(":=", p->child->place, "-", p->place, nextnum, 0);
		}
		gtree *tempp = NULL;
		if (p->child->next)
			if (p->child->next->next->next)
				tempp = p->child->next->next->next;
		while (tempp)
		{
			if (tempp->type == suanfu&&tempp->type_1 == 9)//*
			{
				string temps = p->place;
				p->newlable(labelnum++);
				emit("*", temps, tempp->next->place, p->place, nextnum, 0);
			}
			else if (tempp->type == suanfu&&tempp->type_1 == 10)// /
			{
				string temps = p->place;
				p->newlable(labelnum++);
				emit("/", temps, tempp->next->place, p->place, nextnum, 0);
			}

			tempp = tempp->next->next;
		}
	}
	if (p->_after)
	{
		ofstream out;
		out.open("out.txt", ios::app);
		out << p->after << endl;
		out.close();
		cout << p->after << endl;
	}
	TraverseTree(p->next);
}

bool isletter(char ch)
{
	if ((ch >= 'A'&&ch <= 'Z') || (ch >= 'a'&&ch <= 'z'))
		return 1;
	else
		return 0;
}

bool isnumber(char ch)
{
	if (ch >= '0'&&ch <= '9')
		return 1;
	else
		return 0;
}

void print_petri(C_Petri petri)
{
	cout << "-----------------------" << endl;
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].sourceP == true)
		{
			int j;
			for (j = 0; j < petri.p_num; j++)
			{
				if (petri.arc[i].source == petri.place[j].name)
				{
					cout << "P:" << petri.place[j].v_name << "    ------>    ";
					break;
				}
			}
			if (j == petri.p_num)
			{
				cout << "can't found" << petri.arc[i].source;
			}
			for (j = 0; j < petri.t_num; j++)
			{
				if (petri.arc[i].target == petri.transition[j].name)
				{
					if (petri.transition[j].booleanExpression != "")
						cout << "T:" << petri.transition[j].booleanExpression;
					else
						cout << "T:" << petri.transition[j].v_Expression;

					break;
				}
			}
			if (j == petri.t_num)
			{
				cout << "can't found" << petri.arc[i].target;
			}
			cout << "		value:" << petri.arc[i].V;
			cout << endl;
		}
		else
		{
			int j;
			for (j = 0; j < petri.t_num; j++)
			{
				if (petri.arc[i].source == petri.transition[j].name)
				{
					if (petri.transition[j].booleanExpression != "")
						cout << "T:" << petri.transition[j].booleanExpression << "    ------>    ";
					else
						cout << "T:" << petri.transition[j].v_Expression << "    ------>    ";

					break;
				}
			}
			if (j == petri.t_num)
			{
				cout << "can't found" << petri.arc[i].source;
			}
			for (j = 0; j < petri.p_num; j++)
			{
				if (petri.arc[i].target == petri.place[j].name)
				{
					cout << "P:" << petri.place[j].v_name;
					break;
				}
			}
			if (j == petri.p_num)
			{
				cout << "can't found" << petri.arc[i].target;
			}
			cout << "		value:" << petri.arc[i].V;
			cout << endl;
		}
	}
}

void create_RG(RG &rg)
{
	stack<int> newNode;
	newNode.push(0);
	while (!newNode.empty())
	{
		int node_id = newNode.top();
		newNode.pop();
		rg.add_next(node_id,newNode);
	}
}

void print_RG(RG rg)
{
	for (int i = 0; i < rg.node_num; i++)
	{
		cout << i << endl;
		for (unsigned int j = 0; j < rg.rgnode[i].m.size(); j++)
		{
			
			cout << "(" << rg.petri.place[j].v_name << "," << rg.rgnode[i].m[j].token_num << "," << rg.rgnode[i].m[j].n <<")    ";
		}
		cout << endl;
		cout << "后继节点:";
		for (unsigned int j = 0; j < rg.rgnode[i].next.size(); j++)
		{
			cout << rg.rgnode[i].next[j].num << "    经过变迁:" << rg.rgnode[i].next[j].T << "    ";
		}
		cout << endl;
	}
}

int main()
{
	tree = new gtree;
	tree->init();
	ifstream fin;
	fin.open("input.txt", ios::in);
	if (!fin.is_open())
	{
		cout << "open failed!" << endl;
	}
	fin.read(prog,MAX_LENGTH);
	fin.close();
	//p = 0;
	//printf("\n please input a string(end with '#'):\n");
	//
	//do {
	//	scanf("%c", &ch);
	//	prog[p++] = ch;
	//} while (ch != '#');
	

	p = 0;
	do {

		scaner();
		store[store_num][0] = syn;
		store[store_num][1] = syn_1;
		strcpy(store_char[store_num++], token);
		switch (syn)
		{
		case suanfu:

			cout << "<	" << setw(10) << token << "	" << type[syn - 1] << "	" << syn_1 << "	>" << endl;
			break;

		case -1:
			//printf("you have input a wrong string\n");
			//getch();  
			//return 0;
			syn = 0;
			break;

		default:
			cout << "<	" << setw(10) << token << "	" << type[syn - 1] << "	>" << endl;
			break;
		}
	} while (syn != 0);

	analysis_yufa();

	ofstream out;
	out.open("out.txt", ios::out);
	TraverseTree1(tree,"");
	TraverseTree2(tree);
	TraverseTree3(tree);
	//Traverse(tree);
	declaration(tree);
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].v_name == "main")
		{
			petri.place[i].token_num = 1;
			break;
		}
	}
	print_petri(petri);

	vector<string> v;
	RG rg(petri);
	create_RG(rg);
	print_RG(rg);
//	Traverse(tree);
	out.close();
	//cout << tb1[0].name << endl;
	//cout << tb1[1].name << endl;
	//getch();  
}

void scaner()
{
	sum = 0;

	for (m = 0; m < 10; m++)
		token[m] = NULL;

	ch = prog[p++];
	m = 0;

	while ((ch == ' ') || (ch == '\n'))
		ch = prog[p++];

	if (isletter(ch))
	{
		int n = 0;
		while (isletter(ch) || isnumber(ch))
		{
			token[m++] = ch;
			ch = prog[p++];
			n++;
		}

		p--;

		if (n == 1)
			syn = zimu;
		else
			syn = biaoshifu;

		for (n = 0; n < 6; n++)
			if (strcmp(token, rwtab[n]) == 0)
			{
				syn = guanjianzi;
				syn_1 = n + 1;
				break;
			}
	}
	else if (isnumber(ch))
	{
		//cout << "num" << endl;
		int n = 0;
		while (isnumber(ch))
		{
			//sum = sum * 10 + ch - '0';
			token[m++] = ch;
			ch = prog[p++];

			n++;
		}
		p--;
		if (n == 1)
			syn = shuzi;
		else
			syn = shuzhi;
	}
	else
	{
		switch (ch)
		{
		case '<':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 1;
				token[m++] = ch;
			}
			else
			{
				syn_1 = 2;
				p--;
			}
			syn = suanfu;
			break;

		case '>':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 3;
				token[m++] = ch;
			}
			else
			{
				syn_1 = 4;
				p--;
			}
			syn = suanfu;
			break;

		case '+':
			token[m++] = ch;
			//ch = prog[p++];
			//if (ch == '+')
			//{
			//	syn_1 = 5;
			//	token[m++] = ch;
			//}
			//else
			//{
			//	syn_1 = 6;
			//	p--;
			//}
			syn_1 = 5;
			syn = suanfu;
			break;

		case '-':
			token[m++] = ch;
			//ch = prog[p++];
			//if (ch == '-')
			//{
			//	syn_1 = 7;
			//	token[m++] = ch;
			//}
			//else
			//{
			//	syn_1 = 8;
			//	p--;
			//}
			syn_1 = 6;
			syn = suanfu;
			break;

		case '!':
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 7;
				token[m++] = ch;
				syn = suanfu;
			}
			//else
			//{
			//	syn = 31;
			//	p--;
			//}

			break;

		case '=':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 8;
				token[m++] = ch;
				syn = suanfu;
			}
			else
			{
				syn = fuzhihao;
				p--;
			}
			break;

		case '*':
			syn = suanfu;
			syn_1 = 9;
			token[m++] = ch;
			break;

		case '/':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '/')
			{
				token[m++] = ch;
				syn = zhushihao;
			}
			else if (ch == '*')
			{
				token[m++] = ch;
				while (prog[p] != '*')
				{
					token[m++] = prog[p];
					p++;
				}
				token[m++] = prog[p];
				if (prog[++p] == '/')
				{
					token[m++] = prog[p];
					syn = zhushihao;
					p++;
				}
			}
			else
			{
				syn = suanfu;
				syn_1 = 10;
				p--;
				//token[m++] = ch;
			}
			break;

		case '(':
			syn = zuokuohao;
			token[m++] = ch;
			break;

		case ')':
			syn = youkuohao;
			token[m++] = ch;
			break;

		case '{':
			syn = zuodakuohao;
			token[m++] = ch;
			break;

		case '}':
			syn = youdakuohao;
			token[m++] = ch;
			break;

		case ';':
			syn = jiefu;
			token[m++] = ch;
			break;

		case ',':
			syn = fengefu;
			token[m++] = ch;
			break;

		case '#':
			syn = jieshufu;
			token[m++] = ch;
			break;


		default:
			syn = -1;
			break;
		}
	}
	token[m++] = '\0';
}

bool judge_yuju()
{
	if (store[read_num][0] == biaoshifu || store[read_num][0] == zimu || (store[read_num][0] == guanjianzi && (store[read_num][1] == 3 || store[read_num][1] == 5 || store[read_num][1] == 6)))
		return 1;
	else
		return 0;
}

bool is_int()
{
	if (store[read_num][0] == guanjianzi&&store[read_num][1] == 1)
		return 1;
	else
		return 0;
}

bool is_void()
{
	if (store[read_num][0] == guanjianzi&&store[read_num][1] == 2)
		return 1;
	else
		return 0;
}

bool is_if()
{
	if (store[read_num][0] == guanjianzi&&store[read_num][1] == 3)
		return 1;
	else
		return 0;
}

bool is_else()
{
	if (store[read_num][0] == guanjianzi&&store[read_num][1] == 4)
		return 1;
	else
		return 0;
}

bool is_while()
{
	if (store[read_num][0] == guanjianzi&&store[read_num][1] == 5)
		return 1;
	else
		return 0;
}

bool is_return()
{
	if (store[read_num][0] == guanjianzi&&store[read_num][1] == 6)
		return 1;
	else
		return 0;
}

bool is_relop()
{
	if (store[read_num][0] == suanfu && ((store[read_num][1] >= 1 && store[read_num][1] <= 4) || (store[read_num][1] == 7 || store[read_num][1] == 8)))
		return 1;
	else
		return 0;
}

void trans_print(int num, int n, bool flag)
{
	//int tag;
	if (flag == false)
	{
		for (int i = 0; i <= n ; i++)
			cout << '	';
		cout << store_char[read_num - 1] << endl;
	}
	else
	{
		for (int i = 0; i <= n - 1; i++)
			cout << '	';
		//cout << num << endl;
		switch (num)
		{
		case 16:
			cout << "<程序>" << endl;
			break;
		case 17:
			cout << "<类型>" << endl;
			break;
		case 18:
			cout << "<id>" << endl;
			break;
		case 19:
			cout << "<语句块>" << endl;
			break;
		case 20:
			cout << "<内部声明>" << endl;
			break;
		case 21:
			cout << "<语句串>" << endl;
			break;
		case 22:
			cout << "<内部变量声明>" << endl;
			break;
		case 23:
			cout << "<语句>" << endl;
			break;
		case 24:
			cout << "<if语句>" << endl;
			break;
		case 25:
			cout << "<while语句>" << endl;
			break;
		case 26:
			cout << "<return语句>" << endl;
			break;
		case 27:
			cout << "<赋值语句>" << endl;
			break;
		case 28:
			cout << "<表达式>" << endl;
			break;
		case 29:
			cout << "<加法表达式>" << endl;
			break;
		case 30:
			cout << "<relop>" << endl;
			break;
		case 31:
			cout << "<项>" << endl;
			break;
		case 32:
			cout << "<因子>" << endl;
			break;
		case 33:
			cout << "<FTYPE>" << endl;
			break;
		case 34:
			cout << "<call>" << endl;
			break;
		case 35:
			cout << "<实参列表>" << endl;
			break;
		case 36:
			cout << "<实参>" << endl;
			break;
		case 37:
			cout << "<形参>" << endl;
			break;
		case 38:
			cout << "<参数列表>" << endl;
			break;
		case 39:
			cout << "<参数>" << endl;
			break;
		case 40:
			cout << "<声明串>" << endl;
			break;
		case 41:
			cout << "<声明>" << endl;
			break;
		case 42:
			cout << "<声明类型>" << endl;
			break;
		case 43:
			cout << "<函数声明>" << endl;
			break;
		case 44:
			cout << "<变量声明>" << endl;
			break;
		}
	}
}

int P(int num, int n, gtree *tree1)
{
	trans_print(num, n, true);
	if (num == program)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "program" << endl;
		if (P(shengmingchuan, n + 1, tree1) == ERROR)
			return ERROR;
		tree1 = tree1->next;
		
	}
	else if (num == shengmingchuan)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(shengming, n + 1, tree1) == ERROR)
			return ERROR;
		//tree1 = tree1->next;
		while (store[read_num][0] == guanjianzi && (store[read_num][1] == 1 || store[read_num][1] == 2))
		{
			tree1->cnode(shengming, 0, tree1);
			tree1 = tree1->next;
			if (P(shengming, n + 1, tree1) == ERROR)
				return ERROR;

		}
	}
	else if (num == shengming)
	{
		if (store[read_num][0] == guanjianzi && store[read_num][1] == 1)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;

			read_num++;
			tree1 = tree1->next;
			if (P(id, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (P(shengmingleixing, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else if (store[read_num][0] == guanjianzi && store[read_num][1] == 2)
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;

			read_num++;
			tree1 = tree1->next;
			if (P(id, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (P(hanshushengming, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else
		{
			cout << "不支持除了int void之外的类型！" << endl;
			return ERROR;
		}
	}
	else if (num == shengmingleixing)
	{
		if (store[read_num][0] == zuokuohao)
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(hanshushengming, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else if (store[read_num][0] == jiefu || store[read_num][0] == fuzhihao)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(bianliangshengming, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else
		{
			cout << "shengmingleixing_error!" << endl;
			return ERROR;
		}
	}
	else if (num == bianliangshengming)
	{
		if (store[read_num][0] == jiefu)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
		}
		else if (store[read_num][0] == fuzhihao)
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			read_num++;
			tree1 = tree1->next;
			if (P(biaodashi, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (store[read_num][0] == jiefu)
			{
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n,false);

			}
			else
			{
				cout << "变量声明缺少';'" << endl;
				return ERROR;
			}
		}
		else
		{
			cout << "变量声明有误!" << endl;
			return ERROR;
		}
	}
	else if (num == hanshushengming)
	{
		tree1->place = store_char[read_num - 1];
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == zuokuohao)
		{
			
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (P(xingcan, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (store[read_num][0] == youkuohao)
			{
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n,false);
			}
			else
			{
				cout << "缺少右括号!" << endl;
				return ERROR;
			}
			if (P(yujukuai, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else
		{
			cout << "缺少左括号！" << endl;
			return ERROR;
		}
	}
	else if (num == xingcan)
	{

		//cout << "xingcan" << endl;
		if (store[read_num][0] == youkuohao)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;

		}
		else
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(canshuliebiao, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
	}
	else if (num == canshuliebiao)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "canshuliebiao" << endl;
		if (P(canshu, n + 1, tree1) == ERROR)
			return ERROR;
		//tree1 = tree1->next;
		while (1)
		{
			if (store[read_num][0] == fengefu)
			{
				read_num++;
				//cout << 1 << endl;
				tree1->cnode(fengefu, 0, tree1);
				tree1 = tree1->next;
				tree1->cnode(canshu, 0, tree1);
				tree1 = tree1->next;

				trans_print(num, n,false);
				if (P(canshu, n + 1, tree1) == ERROR)
					return ERROR;
				tree1 = tree1->next;
			}
			else
				break;
		}
	}
	else if (num == canshu)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "canshu" << endl;
		//cout << store[read_num][0] << ' ' << store[read_num][1] << endl;
		if (store[read_num][0] == guanjianzi&&store[read_num][1] == 1)//int代号为1
		{
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
			tb1[tb_num].enter(store_char[read_num], 1, kong, offset1);//1代表int
			tb_num++;
			offset1 += sizeof(int);
			if (P(id, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else
		{
			cout << "缺少int!" << endl;
			return ERROR;
		}
	}
	else if (num == leixing)
	{

		//cout << "leixing" << endl;
		if (store[read_num][0] == guanjianzi && (store[read_num][1] == 1 || store[read_num][1] == 2))//int or void
		{

			sum = store[read_num][1];
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
		}
		else
		{
			cout << "缺少int或void!" << endl;
			return ERROR;
		}
	}
	else if (num == id)
	{
		//cout << "id" << endl;
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1->place = store_char[read_num];
		tree1 = tree1->child;
		if (store[read_num][0] == biaoshifu || store[read_num][0] == zimu)
		{
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);

		}
		else
		{
			cout << "缺少标识符!" << endl;
			return ERROR;
		}
	}
	else if (num == yujukuai)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "yujukuai" << endl;
		if (store[read_num][0] == zuodakuohao)
		{
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (P(neibushengming, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (P(yujuchuan, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (store[read_num][0] == youdakuohao)
			{
				//cout << store[read_num][0] << ' ' << store[read_num+1][0] << endl;
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n,false);
			}
			else
			{
				cout << "缺少右大括号!" << endl;
				return ERROR;
			}
		}
		else
		{
			cout << "缺少左大括号!" << endl;
			return ERROR;
		}

	}
	else if (num == neibushengming)
	{
		//cout << "neibushengming" << endl;
		if (judge_yuju())
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			return 0;
		}
		else
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(neibubianliangshengming, n + 1, tree1) == ERROR)
				return ERROR;
			//cout << 1 << endl;
			tree1 = tree1->next;
			if (store[read_num][0] == jiefu)
			{
				read_num++;
				//tree1 = tree1->next;
				trans_print(num, n,false);
			}
			else
			{
				cout << "缺少界符!" << endl;
				return ERROR;
			}
			while (store[read_num][0] == guanjianzi&&store[read_num][1] == 1)
			{

				tree1->cnode(neibubianliangshengming, 0, tree1);

				tree1 = tree1->next;
				if (P(neibubianliangshengming, n + 1, tree1) == ERROR)
					return ERROR;
				//tree1 = tree1->next;
				if (store[read_num][0] == jiefu)
				{
					read_num++;
					tree1->cnode(jiefu, 0, tree1);
					tree1 = tree1->next;
					trans_print(num, n,false);
				}
				else
				{
					cout << "缺少界符!" << endl;
					return ERROR;
				}
			}
		}
	}
	else if (num == neibubianliangshengming)
	{
		tree1->place = store_char[read_num + 1];
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "neibubianliangshengming" << endl;

		if (store[read_num][0] == guanjianzi && store[read_num][1] == 1)
		{

			read_num++;
			tree1 = tree1->next;
			trans_print(num, n, false);
			tb1[tb_num].enter(store_char[read_num], 1, kong, offset1);//1代表int
			tb_num++;
			offset1 += sizeof(int);
			if (P(id, n + 1, tree1) == ERROR)
				return ERROR;
			//tree1 = tree1->next;
			if (store[read_num][0] == fuzhihao)
			{
				read_num++;
				tree1->cnode(fuzhihao, 0, tree1);
				tree1 = tree1->next;
				trans_print(num, n, false);
				tree1->cnode(biaodashi, 0, tree1);
				tree1 = tree1->next;
				if (P(biaodashi, n + 1, tree1) == ERROR)
					return ERROR;
			}
		}
		else
		{
			cout << "内部变量声明缺少int!" << endl;
			return ERROR;
		}
	}
	else if (num == yujuchuan)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "yujuchuan" << endl;
		if (P(yuju, n + 1, tree1) == ERROR)
			return ERROR;
		//tree1 = tree1->next;
		while (judge_yuju())
		{
			tree1->cnode(yuju, 0, tree1);
			tree1 = tree1->next;
			if (P(yuju, n + 1, tree1) == ERROR)
				return ERROR;

		}
		//tree1 = tree1->next;
		//cout << 1 << endl;
	}
	else if (num == yuju)
	{
		//cout << "yuju" << endl;
		//cout << store[read_num][0] << endl;
		//getchar();
		if (is_if())
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(ifyuju, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else if (is_while())
		{
			sum = 2;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(whileyuju, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else if (is_return())
		{
			sum = 3;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(returnyuju, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;

		}
		else if ((store[read_num][0] == biaoshifu || store[read_num][0] == zimu) && store[read_num + 1][0] == fuzhihao)
		{
			sum = 4;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(fuzhiyuju, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;

		}
		else if ((store[read_num][0] == biaoshifu || store[read_num][0] == zimu) && store[read_num + 1][0] == zuokuohao)
		{
			sum = 5;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(biaodashi, n + 1, tree1) == ERROR)
				return ERROR;
			//特殊处理表达式;的结构
			//tree1 = tree1->next;
			if (store[read_num][0] == jiefu)
			{
				read_num++;
				tree1->cnode(jiefu, 0, tree1);
				trans_print(num, n,false);
			}
			else
			{
				cout << "缺少界符！" << endl;
				return ERROR;
			}
		}
		else
		{
			cout << "缺少句子（文法中语句串不能为空）!" << endl;
			return ERROR;
		}
		//cout << 1 << endl;
	}
	else if (num == fuzhiyuju)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "fuzhiyuju" << endl;
		if (P(id, n + 1, tree1) == ERROR)
			return ERROR;
		tree1 = tree1->next;
		if (store[read_num][0] == fuzhihao)
		{
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
		}
		else
		{
			cout << "缺少赋值号!" << endl;
			return ERROR;
		}
		if (P(biaodashi, n + 1, tree1) == ERROR)
			return ERROR;
		tree1 = tree1->next;
		if (store[read_num][0] == jiefu)
		{

			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);

		}
		else
		{
			cout << "缺少界符!" << endl;
			return ERROR;
		}
	}
	else if (num == returnyuju)
	{

		//cout << "returnyuju" << endl;
		//省略isreturn因为语句部分检查过了
		
		read_num++;
		trans_print(num, n, false);

		if (store[read_num][0] != jiefu)
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1 = tree1->next;//因为return是重复部分已经跳过了
			if (P(biaodashi, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (store[read_num][0] == jiefu)
			{

				
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n, false);

			}
			else
			{
				cout << "缺少界符！" << endl;
				return ERROR;
			}
		}
		else
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n, false);
		}

	}
	else if (num == whileyuju)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "whileyuju" << endl;
		if (is_while())
		{
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (store[read_num][0] == zuokuohao)
			{
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n,false);
				if (P(biaodashi, n + 1, tree1) == ERROR)
					return ERROR;
				tree1 = tree1->next;
				if (store[read_num][0] == youkuohao)
				{
					read_num++;
					tree1 = tree1->next;
					trans_print(num, n, false);
				}
				else
				{
					cout << "缺少右括号!" << endl;
					return ERROR;
				}
				if (P(yujukuai, n + 1, tree1) == ERROR)
					return ERROR;
				tree1 = tree1->next;
			}
			else
			{
				cout << "缺少左括号!" << endl;
				return ERROR;
			}
		}
		else
		{
			cout << "缺少while!" << endl;
			return ERROR;
		}
	}
	else if (num == ifyuju)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "ifyuju" << endl;
		if (is_if())
		{
			read_num++;
			trans_print(num, n, false);
			//cout << 1 << endl;
			tree1 = tree1->next;

			if (store[read_num][0] == zuokuohao)
			{
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n,false);

				if (P(biaodashi, n + 1, tree1) == ERROR)
					return ERROR;
				tree1 = tree1->next;
				if (store[read_num][0] == youkuohao)
				{
					read_num++;
					tree1 = tree1->next;
					trans_print(num, n,false);
					if (P(yujukuai, n + 1, tree1) == ERROR)
						return ERROR;
					//tree1 = tree1->next;
					if (store[read_num][0] == guanjianzi&&store[read_num][1] == 4)//else 4
					{

						read_num++;
						tree1->cnode(guanjianzi, 4, tree1);
						tree1 = tree1->next;
						tree1->cnode(yujukuai, 0, tree1);
						tree1 = tree1->next;
						trans_print(num, n,false);
						if (P(yujukuai, n + 1, tree1) == ERROR)
							return ERROR;
						tree1 = tree1->next;
					}
				}
				else
				{
					cout << "缺少右括号!" << endl;
					return ERROR;
				}
			}
			else
			{
				cout << "缺少左括号!" << endl;
				return ERROR;
			}
		}
		else
		{
			cout << "缺少if!" << endl;
			return ERROR;
		}
	}
	else if (num == biaodashi)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "biaodashi" << endl;
		if (P(jiafabiaodashi, n + 1, tree1) == ERROR)
			return ERROR;
		//tree1 = tree1->next;
		while (is_relop())
		{
			read_num++;
			tree1->cnode(suanfu, store[read_num][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(jiafabiaodashi, 0, tree1);
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (P(jiafabiaodashi, n + 1, tree1) == ERROR)
				return ERROR;
			//tree1 = tree1->next;
		}
	}
	else if (num == jiafabiaodashi)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		//cout << "jiafabiaodashi" << endl;
		if (P(xiang, n + 1, tree1) == ERROR)
			return ERROR;
		//tree1 = tree1->next;
		
		while (store[read_num][0] == suanfu && (store[read_num][1] == 5 || store[read_num][1] == 6))
		{
			read_num++;
			tree1->cnode(suanfu, store[read_num][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(xiang, 0, tree1);
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (P(xiang, n + 1, tree1) == ERROR)
				return ERROR;
			//tree1 = tree1->next;
		}
	}
	else if (num == xiang)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(yinzi, n + 1, tree1) == ERROR)
			return ERROR;
		//tree1 = tree1->next;
		while (store[read_num][0] == suanfu && (store[read_num][1] == 9 || store[read_num][1] == 10))
		{
			read_num++;
			tree1->cnode(suanfu, store[read_num][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(yinzi, 0, tree1);
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (P(yinzi, n + 1, tree1) == ERROR)
				return ERROR;
			//tree1 = tree1->next;
		}
	}
	else if (num == yinzi)
	{
		//cout << "xiang" << endl;
		if (store[read_num][0] == shuzhi || store[read_num][0] == shuzi)
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1->place = store_char[read_num];
			tree1 = tree1->child;
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
		}
		else if (store[read_num][0] == biaoshifu || store[read_num][0] == zimu)
		{
			sum = 3;
			tree1->expand(num, tree1, sum);
		//	tree1->place = store_char[read_num];//如果是标识符则终究会归结到id，所以无需存储
			tree1 = tree1->child;
			if (P(id, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (P(FTYPE, n + 1, tree1) == ERROR)
				return ERROR;
			//tree1 = tree1->next;
		}
		else if (store[read_num][0] == zuokuohao)
		{
			sum = 2;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (P(biaodashi, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;

			if (store[read_num][0] == youkuohao)
			{
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n,false);
			}
			else
			{
				cout << "缺少右括号!" << endl;
				return ERROR;
			}
		}
		else
		{
			cout << "缺少左括号!" << endl;
			return ERROR;
		}
	}
	else if (num == FTYPE)
	{
		if (store[read_num][0] == zuokuohao)
		{
			
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(call, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
		else
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			
			return 0;
		}
	}
	else if (num == call)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == zuokuohao)
		{
			read_num++;
			tree1 = tree1->next;
			trans_print(num, n,false);

			if (P(shican, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
			if (store[read_num][0] == youkuohao)
			{
				read_num++;
				tree1 = tree1->next;
				trans_print(num, n,false);
			}
			else
			{
				cout << "缺少右括号!" << endl;
				return ERROR;
			}
		}
		else
		{
			cout << "缺少左括号!" << endl;
			return ERROR;
		}
	}
	else if (num == shican)
	{
		if (store[read_num][0] == youkuohao)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;

		}
		else
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(shicanliebiao, n + 1, tree1) == ERROR)
				return ERROR;
			tree1 = tree1->next;
		}
	}
	else if (num == shicanliebiao)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(biaodashi, n + 1, tree1) == ERROR)
			return ERROR;
		while (store[read_num][0] == fengefu)
		{
			read_num++;
			tree1->cnode(fengefu, store[read_num][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(biaodashi, 0, tree1);
			tree1 = tree1->next;
			trans_print(num, n,false);
			if (P(biaodashi, n + 1, tree1) == ERROR)
				return ERROR;
		}
	}
	else
	{
		cout << "input error!" << endl;
		return ERROR;
	}
	if (n == 0)
	{
		if (store[read_num][0] == jieshufu)
			return 0;
		else
		{
			cout << "end error!" << endl;
			return ERROR;

		}
	}
}

void analysis_yufa()
{
	if (P(program, 0, tree) == ERROR)
		cout << "ERROR" << endl;
	else
		cout << "successful" << endl;
}