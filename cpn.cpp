#include"cpn.h"


string colorset[] = { "int","char","float","double","string" };

int Arc::id_num = 0;
int Place::id_num = 0;
int Transition::id_num = 0;


Arc::Arc(string s, string t, string v, bool s_P)
{
	source = s;
	target = t;
	V = v;
	sourceP = s_P;
	id_num++;
}

Place::Place(string n, string v_n, int colorset_t, bool c_P)
{
	name = n;
	v_name = v_n;
	colorset_tag = colorset_t;
	controlP = c_P;
	id_num++;
}

void Place::set_Place_value(int n)
{
	num = n;
	token_num = 1;
}

void Place::set_Place_value(double dec)
{
	decimal = dec;
	token_num = 1;
}

void Place::set_Place_value(string s)
{
	str = s;
	token_num = 1;
}

Transition::Transition(string n, bool c_T)
{
	name = n;
	controlT = c_T;
	id_num++;
}

void Transition::set_C_Transition_value(string str)
{
	booleanExpression = str;
}

void Transition::set_V_Transition_value(string str)
{
	v_Expression = str;
}

C_Petri::C_Petri()
{

}

void C_Petri::Add_Arc(Arc a)
{
	arc.push_back(a);
	arcnum++;
}

void C_Petri::Add_Arc(string source, string target, string V, bool sourceP)
{
	int sourceid, targetid;
	Arc a(source, target, V, sourceP);
	for (vector<Arc>::iterator it = arc.begin(); it != arc.end(); it++)
	{
		if (it->source == source && it->target == target)
		{

			if (it->V == V)
			{
				cout << "重复添加边" << source << "-->" << target << endl;
				//arcnum--;
				return;
			}
			else
			{
				cout << "覆盖边" << source << "-->" << target << endl;
				arc.erase(it);
				arcnum--;
				break;
			}
		}
	}
	if (sourceP)
	{
		int i, j;
		for (i = 0; i < p_num; i++)
		{
			if (place[i].name == source)
			{
				sourceid = place[i].id_num;
				break;
			}
		}
		if (i == p_num)
		{
			cout << "can't find source:" << source << endl;
			exit(1);
		}
		for (j = 0; j < t_num; j++)
		{
			if (transition[j].name == target)
			{
				targetid = transition[j].id_num;
				break;
			}
		}
		if (j == t_num)
		{
			cout << "can't find target:" << target << endl;
			exit(1);
		}
	}
	else
	{
		int i, j;
		for (i = 0; i < p_num; i++)
		{
			if (place[i].name == target)
			{
				targetid = place[i].id_num;
				break;
			}
		}
		if (i == p_num)
		{
			cout << "can't find target!" << endl;
			exit(1);
		}
		for (j = 0; j < t_num; j++)
		{
			if (transition[j].name == source)
			{
				sourceid = transition[j].id_num;
				break;
			}
		}
		if (j == t_num)
		{
			cout << "can't find source!" << endl;
			exit(1);
		}
	}
	a.sourceNum = sourceid;
	a.targetNum = targetid;
	Add_Arc(a);
}

void C_Petri::Add_Place(Place p)
{
	place.push_back(p);
	p_num++;
}

void C_Petri::Add_Place(string name, string v_name, int colorset_tag, bool controlP, bool t, int n, double dec, string s)
{
	Place place(name, v_name, colorset_tag, controlP);
	if (!t)
	{
		//		place.token_num = 0;//表示未赋初值
		if (controlP == true)
			place.token_num = 0;
		else
			place.token_num = 1;
		Add_Place(place);
	}
	else
	{
		if (controlP == true)
			place.token_num = 0;
		else
			place.token_num = 1;
		place.token_num = 1;
		switch (colorset_tag)
		{
		case 0:
			place.set_Place_value(n);
			break;
		case 2:
			place.set_Place_value(dec);
			break;
		case 3:
			place.set_Place_value(dec);
			break;
		default:
			place.set_Place_value(s);
			break;
		}
		Add_Place(place);
	}
}

void C_Petri::Add_Transition(Transition t)
{
	transition.push_back(t);
	t_num++;
}

void C_Petri::Add_Transition(string name, bool controlT, string s)
{
	Transition t(name, controlT);
	if (controlT)
		t.set_C_Transition_value(s);
	else
		t.set_V_Transition_value(s);
	Add_Transition(t);


}

string C_Petri::find_place(string t_name, string V)
{
	for (int i = 0; i < arcnum; i++)
	{
		if ((arc[i].sourceP == true) && (arc[i].target == t_name) && arc[i].V == V)
		{
			return arc[i].source;
		}
	}
	cout << "can't find place" << endl;
	return "";
}

bool C_Petri::Add_Place_enter(string name, string s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].enter.push_back(s);
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_enter(string name, vector<string> s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].enter.insert(place[i].enter.end(), s.begin(), s.end());
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_exit(string name, string s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].exit.push_back(s);
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_exit(string name, vector<string> s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].exit.insert(place[i].exit.end(), s.begin(), s.end());
			return true;
		}
	}
	return false;
}

vector<string> C_Petri::get_enter(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].enter;
		}
	}
	cout << "error in get_enter()" << endl;
	exit(1);
}

vector<string> C_Petri::get_exit(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].exit;
		}
	}
	cout << "error in get_exit()" << endl;
	exit(1);
}

bool C_Petri::is_enable(string t_name)
{
	vector<string> all_P;
	all_P = find_all_place(t_name);
	bool flag = true;
	for (unsigned int i = 0; i < all_P.size(); i++)
	{
		for (int j = 0; j < p_num; j++)
		{
			if (place[j].name == all_P[i])
			{
				if (place[j].token_num < 1)
				{
					flag = false;
					break;
				}
			}
		}
	}

	string guard_Expression;
	for (int i = 0; i < t_num; i++)
	{
		if (transition[i].name == t_name)
		{
			guard_Expression = transition[i].booleanExpression;
			break;
		}
	}
	if (guard_Expression != "")
	{
		bool flag1 = guard(guard_Expression);
		if (flag1 == true && flag == true)
			return true;
		else
			return false;
	}
	else
	{
		if (flag == false)
			return false;
		else
			return true;
	}

}

vector<string> C_Petri::enable_T()
{
	vector<string> T;
	for (int i = 0; i < t_num; i++)
	{
		if (is_enable(transition[i].name) == true)
		{
			T.push_back(transition[i].name);
		}
	}

	return T;
}

void SplitString_1(const string& s, vector<string>& v, const string& c)
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

void splitExpression_1(string &s, vector<string>& v)//传入字符串和结果集合，将字符串根据>,<等分割并放入结果集
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

int priority(char c)
{
	if (c == '#')
		return -1;
	else
	{
		switch (c)
		{
		case'(':
			return 0;
			break;
		case '+':
		case '-':
			return 1;
			break;
		case '*':
		case '/':
			return 2;
			break;
		case ')':
			return 3;
		}
	}
	cout << "priority error!" << endl;
	return -2;
}

bool is_operator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '#')
	{
		return true;
	}
	return false;
}

int get_value(string s,vector<Place> place)//通过变量名寻找变量库所value的值,暂时只有int
{
	if (s[0] >= '0'&&s[0] <= '9')
	{
		return atoi(s.c_str());
	}
	for (unsigned int i = 0; i < place.size(); i++)
	{
		if (s == place[i].v_name)
		{
			return place[i].num;//暂时不进行判断，直接返回库所内int内容
		}
	}
	cout << "get_value error!" << endl;
	exit(1);
}

int cal(int top_1, int top_2, char c)
{
	if (c == '+')
		return top_1 + top_2;
	else if (c == '-')
		return top_1 - top_2;
	else if (c == '*')
		return top_1 * top_2;
	else if (c == '/')
		return top_1 / top_2;
	else
		return -1;
}

int change_and_cal(string s,vector<Place> place)//将中缀式s转换成后缀式并且将变量进行代换
{
	string temp_s;


	stack<int> Stack;
	stack<char> Stack_c;
	Stack_c.push('#');
	s.append(1, '#');
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ')
			continue;
		if (is_operator(s[i]) == true)
		{
			if (i >= 1 && is_operator(s[i - 1]) == false)
			{
				int value = get_value(temp_s,place);
				temp_s.clear();
				Stack.push(value);
			}
			if (s[i] == '(')
			{
				Stack_c.push(s[i]);
			}
			else if (s[i] == ')')
			{
				while (Stack_c.top() != '(')
				{
					int top_1 = Stack.top();
					Stack.pop();
					int top_2 = Stack.top();
					Stack.pop();
					char c = Stack_c.top();
					Stack_c.pop();
					int res = cal(top_2, top_1, c);
					Stack.push(res);
				}
				Stack_c.pop();
			}
			else
			{
				while (priority(s[i]) <= priority(Stack_c.top()))
				{
					if (s[i] == '#' && Stack_c.top() == '#')
					{
						int result = Stack.top();
						Stack.pop();
						return result;
					}
					int top_1 = Stack.top();
					Stack.pop();
					int top_2 = Stack.top();
					Stack.pop();
					char c = Stack_c.top();
					Stack_c.pop();
					int res = cal(top_2, top_1, c);
					Stack.push(res);



				}
				Stack_c.push(s[i]);
			}
		}
		else
			temp_s.append(1, s[i]);
	}
	cout << "change error!" << endl;
	return -1;
}

bool C_Petri::guard(string booleanExpression)//暂时默认只有一个比较符号的简单表达式
{
	vector<string> v;
	int flag;
	if (booleanExpression.find(">=")!=string::npos)
	{
		flag = 0;//>=
		SplitString_1(booleanExpression, v, ">=");
	}
	else if (booleanExpression.find("<=") != string::npos)
	{
		flag = 1;//<=
		SplitString_1(booleanExpression, v, "<=");
	}
	else if (booleanExpression.find("<") != string::npos)
	{
		flag = 2;//<
		SplitString_1(booleanExpression, v, "<");
	}
	else if (booleanExpression.find(">") != string::npos)
	{
		flag = 3;//>
		SplitString_1(booleanExpression, v, ">");
	}
	else if (booleanExpression.find("==") != string::npos)
	{
		flag = 4;//==
		SplitString_1(booleanExpression, v, "==");
	}
	else if (booleanExpression.find("!=") != string::npos)
	{
		flag = 5;//!=
		SplitString_1(booleanExpression, v, "!=");
	}
	else
		cout << "not exist operator!" << endl;
	if (v.size() == 2)
	{
		int left = change_and_cal(v[0],place);
		int right = change_and_cal(v[1],place);
		if (flag == 0)
		{
			if (left >= right)
				return true;
			else
				return false;
		}
		else if (flag == 1)
		{
			if (left <= right)
				return true;
			else
				return false;
		}
		else if (flag == 2)
		{
			if (left < right)
				return true;
			else
				return false;
		}
		else if (flag == 3)
		{
			if (left > right)
				return true;
			else
				return false;
		}
		else if (flag == 4)
		{
			if (left == right)
				return true;
			else
				return false;
		}
		else if (flag == 5)
		{
			if (left != right)
				return true;
			else
				return false;
		}
		else
		{
			cout << "guard error!" << endl;
			exit(1);
		}
	}
	else if (v.size() == 1)
	{
		int res = change_and_cal(v[0],place);
		if (res != 0)
			return true;
		else
			return false;
	}
	else
	{
		cout << "guard error! no expression!" << endl;
		exit(1);
	}

	return true;
}

vector<string> C_Petri::find_all_place(string t_name)
{
	vector<string> v;

	for (int i = 0; i < arcnum; i++)
	{
		if (arc[i].sourceP == true)
			if (arc[i].target == t_name)
				v.push_back(arc[i].source);
	}
	return v;
}

