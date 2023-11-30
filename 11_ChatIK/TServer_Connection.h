/*клиент на стороне сервера*/
#pragma once
#include "Const.h"
#include "Predef.h"
#include "TEvent.h"
#include "TEventQue.h"
#include "TNetIOParser.h"
using namespace std;

class TServer_Connection
{
public:
	TServer_Connection(SOCKET socket, TServer* server);
	~TServer_Connection();	
	void Stop();

	void SendMsg(shared_ptr<IEvent> e) { sendBuf.insert(e); };

	SOCKET Socket() { return _sock; }
	string AuthorizedUserName() { return authorizedUserName; };
	auto Active() { return run; }
private:
	static void listenJob(TServer_Connection * conn);
	thread thjob;
	bool run;
	SOCKET _sock;
	TServer* srv;
	TNetIOParser pars;
	TEventQue sendBuf;
	int connID;//не понадобилось, но пусть будет...
	static int IDgen;
	string authorizedUserName;//имя пользователя, если прошла авторизация
};

