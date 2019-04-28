#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stack>
#define ERROR -1
using namespace std;
//��ɫPetri���Ľṹ

//string colorset[] = { "int","char","float","double","string" };

class Arc
{
public:
	int static id_num;

	string source;//Դ������
	string target;//Ŀ�������
	string V;//��¼���ϵı���
	int sourceNum = 0;
	int targetNum = 0;
	bool sourceP;//false����Դ�ڵ��Ǳ�Ǩ

//	int weight = 1;//��¼���ϵ�Ȩֵ

	Arc(string s, string t, string v, bool s_P);
};


class Place
{
public:
	static int id_num;

	string name;//���������ƣ����ڻ��ı�ǣ�
	string v_name;//������ű��������ı�����
	string str;//colorsetΪstring����charʱ�洢ֵ
	int token_num;//token�ĸ���
	int colorset_tag;//��Ӧcolorset����ֵ
	int num = 0;//colorsetΪ����ʱ�洢ֵ
	double decimal;//colorsetΪС��ʱ�洢ֵ
	bool controlP;//���ֿ��ƿ������Ǳ�������

	vector<string> enter;//��ǿ�ʼ��Ǩ
	vector<string> exit;//��ǽ�β��Ǩ

	Place(string n, string v_n, int colorset_t, bool c_P);
	void set_Place_value(int num);
	void set_Place_value(double decimal);
	void set_Place_value(string str);

};

class Transition
{
public:
	static int id_num;

	string name;//��Ǩ�����ƣ����ڻ��ı�ǣ�
	string booleanExpression;//���������ַ����洢bool���ʽ
	string v_Expression;//�����ڴ�ű������ʽ
//	int num = 0;
	bool controlT;//���ֿ����ڻ��Ǳ�����
	//bool TorF;//�����ڵ�ֵ

	Transition(string n, bool c_T);
	void set_C_Transition_value(string str);
	void set_V_Transition_value(string str);
};

class C_Petri
{
public:
	int p_num;//��������
	int t_num;//��Ǩ����
	int arcnum;//������


	vector<Place> place;//����
	vector<Transition> transition;//��Ǩ
	vector<Arc> arc;//��


public:
	C_Petri();

	void Add_Place(Place p);
	void Add_Place(string name, string v_name, int colorset_tag, bool controlP, bool t, int n, double dec, string s);//t=true��ʾ�г�ʼֵ


	void Add_Transition(Transition t);
	void Add_Transition(string name, bool controlT, string s);


	void Add_Arc(Arc a);
	void Add_Arc(string source, string target, string V, bool sourceP);

	string find_place(string t_name, string V);//ͨ����Ǩ�ͻ��ϵı���ֵ�ҵ���Ӧ����,���ؿ���name

	bool Add_Place_enter(string name, string s);
	bool Add_Place_enter(string name, vector<string> s);
	bool Add_Place_exit(string name, string s);
	bool Add_Place_exit(string name, vector<string> s);
	bool is_enable(string t);//�жϱ�Ǩt�Ƿ�ɷ���
	bool guard(string booleanExpression); //guard�����Ĵ���

	vector<string> get_enter(string name);
	vector<string> get_exit(string name);
	vector<string> enable_T(); //�����������пɷ����ı�Ǩ
	vector<string> find_all_place(string t_name);//�ұ�Ǩ������ǰ������
};

int priority(char c);
bool is_operator(char c);
int get_value(string s, vector<Place> place);
int cal(int top_1, int top_2, char c);
int change_and_cal(string s, vector<Place> place);