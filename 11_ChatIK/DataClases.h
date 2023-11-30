#pragma once
#include "Const.h"
#include "Predef.h"

#include <iostream>
#include <fstream>

using namespace std;

//������ � ���� - ������������
struct TUser
{
	TUser(string Name, string Pass, bool loggined)
		:name(Name)
		,pass(Pass)
		,logged(loggined)
	{}
	string name;
	string pass;
	bool logged;
};

//���� �������������
class IUserDB
{
public:
	IUserDB(string LoadDataPath = DEFAULT_USER_FN);
	~IUserDB();
	bool LogIn(string name, string pass);
	void LogOFF(string name);
private:
	string UFP;//���� ���� ��������� ������
	void loadUsers(string path);
	void saveUsers();
	map<string, TUser*> users;
	mutex m_Access;
};

//������ ��������� � ����
struct TMessage
{
	TMessage(string Author, string To, string Msg)
		:t(gettime())
		,author(Author)
		,to(To)
		,msg(Msg)
	{};
	time_t t;//����� �������
	string author;//�����
	string to;//����
	string msg;	//�����
	string GetMessageText();//�������� ����� ��� �����������
};
// ���� ���������
class IMessages
{
public:
	void InsertMessage(string author, string to, string msg)
	{
		m_Access.lock();
		messages.push_back(new TMessage(author, to, msg));
		m_Access.unlock();
	};
	void SortMessages()
	{
		m_Access.lock();
		sort(messages.begin(), messages.end(), [](TMessage* a, TMessage* b) {return a->t < b->t; });
		m_Access.unlock();
	};
	vector<TMessage*> ReadMessages() { return messages; };
	TMessage* Msg(int n) { return messages[n]; }
	int MsgCount() { return messages.size(); }
private:
	vector<TMessage*> messages;
	mutex m_Access;
};

//������ �������
class TServerDataClass:public IUserDB, public IMessages
{
public:
	TServerDataClass() :IUserDB(), IMessages() {};
};

//������ �������
class TClientDataClass : public IMessages
{
public:
	TClientDataClass() : IMessages() {};
};

