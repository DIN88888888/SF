#include "DataClases.h"

IUserDB::IUserDB(string LoadDataPath):UFP(LoadDataPath) {
	m_Access.lock();
	if (UFP.length() > 0)
		loadUsers(UFP);
	m_Access.unlock();
}
IUserDB::~IUserDB() {
	m_Access.lock();
	if (UFP.length() > 0)
		saveUsers();
	for (auto u : users) {
		delete u.second;
	}
	m_Access.unlock();
}
bool IUserDB::LogIn(string name, string pass) {
	m_Access.lock();
	auto el = users.find(name);
	if (el != users.end()){
		if (el->second->pass == pass && !el->second->logged) {
			el->second->logged = true;
			m_Access.unlock();
			return true;
		}
	}
	else
	{
		users.emplace(name, new TUser(name, pass, true));
		m_Access.unlock();
		return true;
	}
	m_Access.unlock();
	return false;
}
void IUserDB::LogOFF(string name) {
	m_Access.lock();
	if (users.find(name) != users.end()) {
		users[name]->logged = false;		
	}
	m_Access.unlock();
}
void IUserDB::loadUsers(string path) {
	if (users.size() > 0) return;

	ifstream datafile(path);
	string ln;
	if (datafile.is_open()) {
		while (getline(datafile, ln)) {
			int pos;
			for (pos = 0; pos < ln.length(); pos++) if (ln[pos] == ' ') break;

			users.emplace(ln.substr(0,pos-1), new TUser(ln.substr(0, pos - 1), ln.substr(pos + 1), false));
		}
		datafile.close();
	}
}
void IUserDB::saveUsers() {
	ofstream f(UFP,ios::trunc);
	if (f.is_open()) {
		for(auto u : users){ 
			f << u.second->name << " " << u.second->pass;
		}
		f.close();
	}
}

string TMessage::GetMessageText() {
	string s = (author + "            ").substr(0,12) ;
	if (to != "ALL")
		s = s + "(whisper)";
	s += ": " + msg;
	return s;
}