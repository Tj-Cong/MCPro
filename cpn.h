#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stack>
#define ERROR -1
using namespace std;
//有色Petri网的结构

//string colorset[] = { "int","char","float","double","string" };

class Arc
{
public:
	int static id_num;

	string source;//源的名称
	string target;//目标的名称
	string V;//记录弧上的变量
	int sourceNum = 0;
	int targetNum = 0;
	bool sourceP;//false代表源节点是变迁

//	int weight = 1;//记录弧上的权值

	Arc(string s, string t, string v, bool s_P);
};


class Place
{
public:
	static int id_num;

	string name;//库所的名称（用于弧的标记）
	string v_name;//用来存放变量库所的变量名
	string str;//colorset为string或者char时存储值
	int token_num;//token的个数
	int colorset_tag;//对应colorset的数值
	int num = 0;//colorset为整型时存储值
	double decimal;//colorset为小数时存储值
	bool controlP;//区分控制库所还是变量库所

	vector<string> enter;//标记开始变迁
	vector<string> exit;//标记结尾变迁

	Place(string n, string v_n, int colorset_t, bool c_P);
	void set_Place_value(int num);
	void set_Place_value(double decimal);
	void set_Place_value(string str);

};

class Transition
{
public:
	static int id_num;

	string name;//变迁的名称（用于弧的标记）
	string booleanExpression;//控制哨用字符串存储bool表达式
	string v_Expression;//变量哨存放变量表达式
//	int num = 0;
	bool controlT;//区分控制哨还是变量哨
	//bool TorF;//控制哨的值

	Transition(string n, bool c_T);
	void set_C_Transition_value(string str);
	void set_V_Transition_value(string str);
};

class C_Petri
{
public:
	int p_num;//库所个数
	int t_num;//变迁个数
	int arcnum;//弧个数


	vector<Place> place;//库所
	vector<Transition> transition;//变迁
	vector<Arc> arc;//弧


public:
	C_Petri();

	void Add_Place(Place p);
	void Add_Place(string name, string v_name, int colorset_tag, bool controlP, bool t, int n, double dec, string s);//t=true表示有初始值


	void Add_Transition(Transition t);
	void Add_Transition(string name, bool controlT, string s);


	void Add_Arc(Arc a);
	void Add_Arc(string source, string target, string V, bool sourceP);

	string find_place(string t_name, string V);//通过变迁和弧上的变量值找到对应库所,返回库所name

	bool Add_Place_enter(string name, string s);
	bool Add_Place_enter(string name, vector<string> s);
	bool Add_Place_exit(string name, string s);
	bool Add_Place_exit(string name, vector<string> s);
	bool is_enable(string t);//判断变迁t是否可发生
	bool guard(string booleanExpression); //guard函数的处理

	vector<string> get_enter(string name);
	vector<string> get_exit(string name);
	vector<string> enable_T(); //返回网内所有可发生的变迁
	vector<string> find_all_place(string t_name);//找变迁的所有前驱库所
};

int priority(char c);
bool is_operator(char c);
int get_value(string s, vector<Place> place);
int cal(int top_1, int top_2, char c);
int change_and_cal(string s, vector<Place> place);