/*������� ������ ���������*/
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

	static void MainILoop(TMainController*);//���� ��������� �������� ��������
	void MainLoop();//������� ����
	
	void SendAppEvent(shared_ptr<IEvent> e);//��������� ��������� � ������� �����
	void AddClient(string srvIP, string srvPort);//������� �������

	TScreen* Screen() { return &screen; };//������ �� ��������� ���������
	TServer* Server() { return &srv; };//������

	int ConnectionCount() { return clients.size(); };//���������� ��������
	IClient* Client(int n = -1) { if (n >= 0)return clients[n]; else return clients.back(); };
	//��������� ������� �� ��� �����
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

