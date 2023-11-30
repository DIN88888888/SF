/*клиент*/
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

	int Status() { return _cliStatus; };//статус клиента
	TMainController* mc() { return _mc; }	//ссылка на приложение
	void Disconnect();// отключиться
	void SendMsg(shared_ptr<IEvent> e) { sendBuf.insert(e); };//отправить сообщение на сервер

	string _port;//порт
	string _srvIP;//айпи сервера
	int id() { return ID; };//уникальный номер клиента сессии
	string SrvName;//имя чата
	string Name;//подтверждённое имя клиента
	bool authorised;//статус авторизации
	TClientDataClass data;//сообщения
private:
	int _cliStatus;//статус цикла обмена данными
	TMainController* _mc;
	SOCKET client_socket;//сокет

	static void ResiveJob(IClient *cli);//метод связи с сервером
	thread thjob;// поток связи
	TNetIOParser pars;// парсер сообщений
	TEventQue sendBuf;// буфер отправки
	int ID;//уник
	static int IdGen;//уник генератор
};

