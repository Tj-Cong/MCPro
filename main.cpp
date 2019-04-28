#include<iostream>
#include"xml2ltl.h"
#include"SBA.h"
using namespace std;

int main()
{
	char filename[50] = "ReachabilityCardinality.xml";
	ifstream infile(filename, ios::in);
	if (!infile)
		return 0;
	bool Reachability = false;
	convertRc(filename, Reachability);

	string S, propertyid; //propertyid stores names of LTL formulae
	char form[20000];     //store LTL formulae

	ifstream read("ReachabilityCardinality.txt", ios::in);
	if (!read) {
		cout << "error!";
		getchar();
		exit(-1);
	}

	while (getline(read, propertyid, ':')) {
	
		cout << propertyid << ':';
		getline(read, S);
		strcpy_s(form, S.c_str());
		//cout << form << endl;
		cout << endl;
		//lexer
		Lexer *lex = new Lexer(form, S.length());
		//syntax analysis
		Syntax_Tree *ST;
		ST = new Syntax_Tree;
		formula_stack Ustack;
		ST->reverse_polish(*lex);
		ST->build_tree();
		cout << "The syntax tree of unsimplified formula£º" << endl;
		ST->print_syntax_tree(ST->root, 0);
		//LTL formula rewrite
		ST->simplify_LTL(ST->root->left);
		/*cout << endl;
		cout << "The syntax tree of simplified formula£º" << endl;
		ST.print_syntax_tree(ST.root, 0);*/
		//syntax tree convert
		ST->negconvert(ST->root->left, Ustack);
		delete lex;
		/*cout << endl;
		cout << "The converted formula£º" << endl;
		cout << ST.root->left->formula << endl;
		cout << endl;*/
		//Êä³öU×ÓÊ½
		/*cout << "The subformulas of LTL whose main operator is \'U\'£º" << endl;
		vector<STNode>::iterator Uiter;
		for (Uiter = Ustack.loc.begin(); Uiter != Ustack.loc.end(); Uiter++)
		{
			cout << (*Uiter)->formula << endl;
		}
		cout << endl;*/
		//¹¹ÔìTGBA
		TGBA *Tgba;
		Tgba = new TGBA;
		Tgba->CreatTGBA(Ustack, ST->root->left);
		Tgba->SimplifyStates();
		delete ST;
		//cout << endl;
		//¹¹ÔìTBA
		TBA *tba;
		tba = new TBA;
		tba->CreatTBA(*Tgba, Ustack);
		delete Tgba;
		string filename = propertyid + ".txt";
		tba->PrintBuchi(filename);
		/*cout << "Please check the file" + filename + ". In this file you can see the Buchi automata related to the LTL formula!";
		cout << endl;*/
		//¹¹ÔìSBA
		SBA *sba;
		sba = new SBA;
		sba->CreatSBA(*tba);
		sba->Simplify();
		sba->Compress();
		delete tba;
		
	}
	return 0;
}