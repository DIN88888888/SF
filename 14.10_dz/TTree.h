#pragma once
#include <string>
const int CH_Max = 256;
using std::string;
class TTree
{
public:
	TTree();
	~TTree();
	void Add(const string &s);
	string GetNearestEnding(const string &s);
	string GetNearestWord(const string& s);
	bool IsValid(const string& s);
private:
	TTree* ch[CH_Max];//����-����, ���� ������ ������ ��� ����... �� ��� ������ ��� ����� ���� ������ �����?
	bool isWord;
	TTree* strShift(const string& s, int &cp);
	void FindNearestWord(string& res);
	void FindNextNearestWord(string& res);
};

