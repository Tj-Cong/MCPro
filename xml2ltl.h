#pragma once
#include"tinyxml.h"
#include<fstream>
using namespace std;


void convertf(char *filename);
void convertc(char *filename);
void convertRc(char *filename, bool Reachability);
void GetLTLf(TiXmlElement *p, ofstream &outltl);
void GetLTLC(TiXmlElement *p, ofstream &outltl);
void GetRC(TiXmlElement *p, ofstream &outrc, bool Reachability);