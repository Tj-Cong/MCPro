#pragma once

#include "cpn.h"

/*
*����״̬
*/
class M
{
public:
	bool control_P;  //���ƿ���
	int token_num;   //ĳһ״̬�µ�token��
	int color;       //��ɫ�������
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

//�ڽӱ����һ���ڵ�
class nextNode
{
public:
	int num; //��һ���ڵ�ı��
	string T; //�����ı�Ǩ��

};

//�ɴ�ͼ�ڵ� 
class RGNode
{
public:
	//string name; //�ýڵ������
	int num; // �ýڵ�ı��
	vector<M> m; //��״̬ÿһ��������M��ʶ
	vector<nextNode> next; //��̽ڵ�ļ���
	bool end;//�˵���
};

//�ɴ�ͼ
class RG
{
public:
	int node_num; //�ڵ����
	C_Petri petri;
	vector<RGNode> rgnode; //�ɴ�ͼ���ڽӱ���ʽ

public:
	RG(C_Petri petri);
	int create_node(C_Petri petri);
	void add_next(int node_num,stack<int> &newNode);

};
