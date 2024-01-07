#include "TTree.h"
TTree::TTree() {
	for (int i = 0; i < CH_Max; i++)
		ch[i] = nullptr;
	isWord = false;
}
TTree::~TTree() {
	for (int i = 0; i < CH_Max; i++)
		if (ch[i])
			delete ch[i];
}
TTree* TTree::strShift(const string& s, int& cp) {
	TTree* node = this;
	cp = 0;
	while (cp < s.length() && node->ch[s[cp]]) {
		node = node->ch[s[cp]];
		cp++;
	}
	return node;
}

void TTree::Add(const string& s) {
	int cp = 0;
	TTree* node = strShift(s, cp);
	while (cp < s.length()) {
		node->ch[s[cp]] = new TTree();
		node = node->ch[s[cp]];
		cp++;
	}
	node->isWord = true;	
}

bool TTree::IsValid(const string& s) {
	if (s.length() == 0) return true;
	int cp = 0;
	TTree* node = strShift(s, cp);
	if (cp < s.length()) return false;
	return true;
}

string TTree::GetNearestEnding(const string &s) {
	if (s.length() == 0) return "";
	int cp = 0;
	TTree* node = strShift(s, cp);
	if (cp < s.length()) return "";
	string res;
	node->FindNextNearestWord(res);
	return res;
}
string TTree::GetNearestWord(const string& s) {
	if (s.length() == 0) return "";
	int cp = 0;
	TTree* node = strShift(s, cp);
	if (cp < s.length()) return "";
	string res;
	node->FindNearestWord(res);
	return s+res;
}

void TTree::FindNearestWord(string &res) {//не знаю что считать ближайшей... пусть будет по алфавиту, не взирая на длину
	if (isWord) {
		return ;
	}
	FindNextNearestWord(res);
}
void TTree::FindNextNearestWord(string& res) {
	for (int i = 0; i < CH_Max; i++) {
		if (ch[i]) {
			res += (char)i;
			ch[i]->FindNearestWord(res);
			return;
		}
	}
}