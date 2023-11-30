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

	TServer_Connection* CreateClientConnection(SOCKET socket);//�������� ���������� � ��������

	void SendMessageToClient(string userName, string message);//�������� ��������� �� ����� ������������.
	void SendMessageToClient(TServer_Connection* client, string message);//�������� ���������
	int Port() { return _port; };
	string Name() { return _name; };
	void InsertNewMessage(string author, string to, string msg);//���������� ��������� ��������� � ���� ������� � �������� �� ��������
	TServerDataClass data;//������ �������(������������ � ���������)
private:
	static void ListenJob(TServer * srv);//����� ���������� � ���������
	thread thlisen;
	TMainController* _mc;
	int _srvStatus;	//������ �������
	SOCKET server_socket;
	int _port;
	string _name;//��� ����
	

	vector<TServer_Connection*> clients;//������������� �������

};

