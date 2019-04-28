#include"RG.h"


RG::RG(C_Petri petri1)
{
	petri.p_num = petri1.p_num;
	petri.t_num = petri1.t_num;
	petri.arcnum = petri1.arcnum;
	for (int i = 0; i < petri1.p_num; i++)
	{
		petri.place.push_back(petri1.place[i]);
	}
	for (int i = 0; i < petri1.t_num; i++)
	{
		petri.transition.push_back(petri1.transition[i]);
	}
	for (int i = 0; i < petri1.arcnum; i++)
	{
		petri.arc.push_back(petri1.arc[i]);
	}

	create_node(petri);
}

bool equal_M(vector<M> m1, vector<M> m2)//要求m1.m2的size一样
{
	for (unsigned int i = 0; i < m1.size(); i++)
	{
		if (m1[i] == m2[i])
		{}
		else
			return false;
	}
	return true;
}

void load(vector<M> m, C_Petri &petri)
{
	for (int i = 0; i < petri.p_num; i++)
	{
		petri.place[i].controlP = m[i].control_P;
		petri.place[i].token_num = m[i].token_num;
		petri.place[i].colorset_tag = m[i].color;
		petri.place[i].num = m[i].n;
		petri.place[i].decimal = m[i].dec;
		petri.place[i].str = m[i].s;
	}

}

void occur_T(C_Petri &petri, string T)
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].sourceP == true)
		{
			if (petri.arc[i].target == T)
			{
				string source = petri.arc[i].source;
				for (int j = 0; j < petri.p_num; j++)
				{
					if (petri.place[j].name == source)
					{
						if (petri.place[j].controlP == true)
						{
							petri.place[j].token_num--;
							if (petri.place[j].token_num < 0)
							{
								cout << "token_num < 0 !" << endl;
								exit(1);
							}

						}
					}
				}
			}
		}
		else
		{
			if (petri.arc[i].source == T)
			{
				string target = petri.arc[i].target;
				for (int j = 0; j < petri.p_num; j++)
				{
					if (petri.place[j].name == target)
					{
						if (petri.place[j].controlP == true)
						{
							petri.place[j].token_num++;
						}
						else
						{
							int value = change_and_cal(petri.arc[i].V,petri.place);
							petri.place[j].num = value;
						}
					}
				}
			}
		}
	}
}

int RG::create_node(C_Petri petri)
{
	vector<M> temp_M;
	M temp_m;
	for (int i = 0; i < petri.p_num; i++)
	{
		temp_m.control_P = petri.place[i].controlP;
		temp_m.token_num = petri.place[i].token_num;
		temp_m.color = petri.place[i].colorset_tag;
		temp_m.n = petri.place[i].num;
		temp_m.dec = petri.place[i].decimal;
		temp_m.s = petri.place[i].str;
		temp_M.push_back(temp_m);
	}
	if (rgnode.size() > 0)
	{
		for (unsigned int i = 0; i < rgnode.size(); i++)
		{
			if (equal_M(rgnode[i].m, temp_M))
			{
				return rgnode[i].num;
			}
		}
		RGNode node;
		node.num = node_num++;
		node.m = temp_M;
		rgnode.push_back(node);
		return node.num;
	}
	else
	{
		RGNode node;
		node.num = node_num++;
		node.m = temp_M;
		rgnode.push_back(node);
		return node.num;
	}

}

void RG::add_next(int node_id,stack<int> &newnode)
{
	RGNode rgnode1 = rgnode[node_id];
	load(rgnode1.m, petri);
	vector<string> T = petri.enable_T();
	if (T.size() == 0)
	{
		rgnode[node_id].end = true;
		return;
	}
	for (unsigned int i = 0; i < T.size(); i++)
	{
		int old_num = node_num;
		nextNode next;
		next.T = T[i];
		load(rgnode1.m, petri);
		occur_T(petri, T[i]);
		int num = create_node(petri);
		next.num = num;
		rgnode[node_id].next.push_back(next);
		if (node_num > old_num)
			newnode.push(next.num);
	}
}

