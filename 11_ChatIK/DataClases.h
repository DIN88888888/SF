#pragma once
#include "Const.h"
#include "Predef.h"

#include <iostream>
#include <fstream>

using namespace std;

//строка в базе - пользователь
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

//база пользователей
class IUserDB
{
public:
	IUserDB(string LoadDataPath = DEFAULT_USER_FN);
	~IUserDB();
	bool LogIn(string name, string pass);
	void LogOFF(string name);
private:
	string UFP;//путь куда сохранить пароли
	void loadUsers(string path);
	void saveUsers();
	map<string, TUser*> users;
	mutex m_Access;
};

//строка сообщения в базе
struct TMessage
{
	TMessage(string Author, string To, string Msg)
		:t(gettime())
		,author(Author)
		,to(To)
		,msg(Msg)
	{};
	time_t t;//метка времени
	string author;//автор
	string to;//кому
	string msg;	//текст
	string GetMessageText();//получить текст для отображения
};
// база сообщений
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

//данные сервера
class TServerDataClass:public IUserDB, public IMessages
{
public:
	TServerDataClass() :IUserDB(), IMessages() {};
};

//данные клиента
class TClientDataClass : public IMessages
{
public:
	TClientDataClass() : IMessages() {};
};

