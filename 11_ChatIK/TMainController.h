/*главный объект программы*/
#pragma once

#include "Const.h"
#include "Predef.h"

#include "Exceptions.h"

#include "TScreen.h"

#include "TClient.h"
#include "TServer.h"

#include "TEventQue.h"
#include "TEvent.h"

#include "TUI_Main.h"

using namespace std;

class TMainController
{
public:
	TMainController();
	~TMainController();

	static void MainILoop(TMainController*);//цикл получения символов квиатуры
	void MainLoop();//главный цикл
	
	void SendAppEvent(shared_ptr<IEvent> e);//отправить сообщение в главный поток
	void AddClient(string srvIP, string srvPort);//создать клиента

	TScreen* Screen() { return &screen; };//ссылка на интерфейс отрисовки
	TServer* Server() { return &srv; };//сервер

	int ConnectionCount() { return clients.size(); };//количество клиентов
	IClient* Client(int n = -1) { if (n >= 0)return clients[n]; else return clients.back(); };
	//получение клиента по его унику
	IClient* ClientByID(int ID) { for (int i = 0; i < clients.size(); i++) if (clients[i]->id() == ID)return clients[i]; return nullptr; };
	bool CreateClientConnection(string ip, string port);
	void Exit() { AppStatus = MC_AS_STOPED; };
private:
	int AppStatus;

	thread thKeyIn;	
	TEventQue KeyEvents;
	TEventQue AppEvents;

	TScreen screen;
	TUI_Main UI;

	TServer srv;
	vector<IClient*> clients;
};

