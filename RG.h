#pragma once

#include "cpn.h"

/*
*库所状态
*/
class M
{
public:
	bool control_P;  //控制库所
	int token_num;   //某一状态下的token数
	int color;       //颜色类型序号
	int n;           
	double dec;        
	string s;

public:
	bool operator==(const M m1)
	{
		if (control_P == m1.control_P&&token_num == m1.token_num&&color == m1.color&&n == m1.n&&dec == m1.dec&&s == m1.s)
			return true;
		else
			return false;
	}
};

//邻接表的下一个节点
class nextNode
{
public:
	int num; //下一个节点的编号
	string T; //发生的变迁名

};

//可达图节点 
class RGNode
{
public:
	//string name; //该节点的名称
	int num; // 该节点的编号
	vector<M> m; //该状态每一个库所的M标识
	vector<nextNode> next; //后继节点的集合
	bool end;//端点标记
};

//可达图
class RG
{
public:
	int node_num; //节点个数
	C_Petri petri;
	vector<RGNode> rgnode; //可达图，邻接表形式

public:
	RG(C_Petri petri);
	int create_node(C_Petri petri);
	void add_next(int node_num,stack<int> &newNode);

};
