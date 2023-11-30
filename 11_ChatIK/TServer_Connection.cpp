#include "TServer_Connection.h"
#include "TServer.h"
#include "TMainController.h"

#define DEFAULT_BUFLEN 4096
int TServer_Connection::IDgen = 0;
TServer_Connection::TServer_Connection(SOCKET socket, TServer* server)
	: _sock(socket)
	, srv(server)
	, connID(IDgen)
	, pars(IDgen)
{
	IDgen++;
	run = true;
    thjob = thread(listenJob, this);
}
void TServer_Connection::listenJob(TServer_Connection* conn) {

    listen(conn->_sock, 1);
    TIMEVAL delay{1,0};
    fd_set readfds;

	int addrlen = sizeof(sockaddr_in);
	sockaddr_in address;

	while (conn->run && conn->srv->Status()==SRV_STATUS_STARTED) {
        // clear the socket fdset
		char client_message[DEFAULT_BUFLEN+1];

        FD_ZERO(&readfds);

        // add master socket to fdset
        FD_SET(conn->_sock, &readfds);
        // wait for an activity on any of the socket
        if (select(0, &readfds, NULL, NULL, &delay) == SOCKET_ERROR) {
            conn->run = false;
        }
		//получение данных из потока
		if (FD_ISSET(conn->_sock, &readfds))
		{
			// get details of the client
			getpeername(conn->_sock, (sockaddr*)&address, (int*)&addrlen);

			// check if it was for closing, and also read the incoming message
			// recv does not place a null terminator at the end of the string (whilst printf %s assumes there is one)			

			int client_message_length = recv(conn->_sock, client_message, DEFAULT_BUFLEN, 0);
			client_message[client_message_length] = '\0';
			conn->pars.AddData(client_message);
		}
		//если данные из потока можно обработать
		while (conn->pars.HaveDataToRead()) {
			bool used = false;
			shared_ptr<IEvent> e = conn->pars.GetNextEvent();
			//пришёл запрос на авторизацию
			if (e->type() == ET_SEND_LOGINPASS) {
				used = true;
				bool sucess = conn->srv->data.LogIn(
					dynamic_cast<TEvent_net_send_LoginPass*>(e.get())->Login(),
					dynamic_cast<TEvent_net_send_LoginPass*>(e.get())->Pass());//пользователь создаётся автоматически, если его небыло в базе

				string msg = TEvent_net_send_AuthStatus(sucess, dynamic_cast<TEvent_net_send_LoginPass*>(e.get())->Login(), 0).ToString();
				send(conn->_sock, msg.c_str(), msg.length(), 0);
				//таки признали - запускаем
				if (sucess) {					
					conn->authorizedUserName = dynamic_cast<TEvent_net_send_LoginPass*>(e.get())->Login();
					//send all chat data to new client
					vector<TMessage*> m = conn->srv->data.ReadMessages();
					for (auto em : m) {
						//отправка всех данных этому клиенту
						bool all = em->to == "ALL";
						bool whisp = em->to == conn->authorizedUserName;
						bool self = em->author== conn->authorizedUserName;
						if (all || whisp || self) {
							msg = TEvent_net_send_HistMesage(em->t,em->author,em->msg, !all, 0).ToString();
							send(conn->_sock, msg.c_str(), msg.length(), 0);
						}
					}
					//добавляем пометку что пользователь подключился
					conn->srv->InsertNewMessage("~SERVER", "ALL", dynamic_cast<TEvent_net_send_LoginPass*>(e.get())->Login() + " connected to chat.");
				}
			}
			//получение текста от клиента
			else if (e->type() == ET_SEND_MESSAGE) {
				used = true;
				if (conn->AuthorizedUserName().length() > 0) {
					conn->srv->InsertNewMessage(conn->authorizedUserName, dynamic_cast<TEvent_net_send_Mesage*>(e.get())->To(), dynamic_cast<TEvent_net_send_Mesage*>(e.get())->Msg());
				}
			}
			//отключение клиента
			else if (e->type() == ET_SEND_DISCONNECT) {
				used = true;
				conn->srv->data.LogOFF(conn->AuthorizedUserName());
				conn->run = false;				
				string msg = TEvent_net_send_Disconnect(conn->AuthorizedUserName(),0).ToString();
				send(conn->_sock, msg.c_str(), msg.length(), 0);
			}
			//если сообщение тут не обработано - отправим в основной поток, авось найдут ему применение.
			if(!used)
				conn->srv->mc()->SendAppEvent(e);
		}
		//отправка сообщений клиенту из буфера
		if (conn->sendBuf.HaveEvents())	{
			deque< shared_ptr<IEvent> > q;
			conn->sendBuf.extract(&q);
			for (auto e : q) {
				string msg = e.get()->ToString();
				if(msg.length()>0)
					send(conn->_sock, msg.c_str(), msg.length(), 0);
			}

		}
        this_thread::sleep_for(chrono::milliseconds(25));
    }
}
void TServer_Connection::Stop() {
	run = false;
	if(thjob.joinable())
		thjob.join();
}
TServer_Connection::~TServer_Connection() {
		Stop();
}