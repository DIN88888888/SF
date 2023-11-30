/*������*/
#pragma once
#include "Const.h"
#include "Predef.h"

#include "TEventQue.h"
#include "TEvent.h"
#include "TNetIOParser.h"
#include "DataClases.h"

using namespace std;

class IClient
{
public:
	IClient(TMainController* mc, string server_ip,string port);
	~IClient();

	int Status() { return _cliStatus; };//������ �������
	TMainController* mc() { return _mc; }	//������ �� ����������
	void Disconnect();// �����������
	void SendMsg(shared_ptr<IEvent> e) { sendBuf.insert(e); };//��������� ��������� �� ������

	string _port;//����
	string _srvIP;//���� �������
	int id() { return ID; };//���������� ����� ������� ������
	string SrvName;//��� ����
	string Name;//������������� ��� �������
	bool authorised;//������ �����������
	TClientDataClass data;//���������
private:
	int _cliStatus;//������ ����� ������ �������
	TMainController* _mc;
	SOCKET client_socket;//�����

	static void ResiveJob(IClient *cli);//����� ����� � ��������
	thread thjob;// ����� �����
	TNetIOParser pars;// ������ ���������
	TEventQue sendBuf;// ����� ��������
	int ID;//����
	static int IdGen;//���� ���������
};

