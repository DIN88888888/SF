#pragma once

#include "Const.h"
#include "Predef.h"

#include "TServer_Connection.h"
#include "DataClases.h"

using namespace std;

class TServer
{
public:
	TServer(TMainController *mc);
	~TServer();

	void Start(int port,string name);	
	void Stop();
	
	SOCKET Socket() { return server_socket; };
	TMainController* mc() { return _mc; };
	int Status() { return _srvStatus; };

	TServer_Connection* CreateClientConnection(SOCKET socket);//создание соединения с клиентом

	void SendMessageToClient(string userName, string message);//отправка сообщения по имени пользователя.
	void SendMessageToClient(TServer_Connection* client, string message);//отправка сообщения
	int Port() { return _port; };
	string Name() { return _name; };
	void InsertNewMessage(string author, string to, string msg);//добавление текстовых сообщений в базу сервера и отправка их клиентам
	TServerDataClass data;//данные сервера(пользователи и сообщения)
private:
	static void ListenJob(TServer * srv);//поток соединения с клиентами
	thread thlisen;
	TMainController* _mc;
	int _srvStatus;	//статус сервера
	SOCKET server_socket;
	int _port;
	string _name;//имя чата
	

	vector<TServer_Connection*> clients;//присоединённые клиенты

};

