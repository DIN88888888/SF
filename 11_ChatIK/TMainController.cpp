#include "TMainController.h"
#include "TClient.h"
#include "TServer.h"
using namespace std;


TMainController::TMainController()
	:UI(this, &screen)
	,AppEvents()
	,srv(this)
{
	AppStatus = MC_AS_WORKING;
	
	thKeyIn = thread(MainILoop,this);
}
TMainController::~TMainController(){
	AppStatus = MC_AS_STOPED;
	for (auto cli : clients)
	{
		cli->Disconnect();
	}
	for (auto cli : clients)
	{
		delete cli;
	}
	screen.write_text(20, 10, 30, 1, "PRESS ANY KEY");
	screen.write_chiAttr(0,0,80,30,TCOLOR_WHITE);
	screen.draw();
	if(thKeyIn.joinable())
		thKeyIn.join();
	cout << "Main:  Key Event handler stoped"<< endl;
}

void TMainController::MainILoop(TMainController* mc){
	int k;
	while (mc->AppStatus == MC_AS_WORKING){
		k = _getch();
		//cout << "Main I Loop K=" <<k<< endl;
		if (k == 224) {
			k = _getch();
			mc->KeyEvents.insert(make_shared<TEvent_Key>(KT_ARROW, k));
		}
		else if (k==0) {
			k = _getch();
			mc->KeyEvents.insert(make_shared<TEvent_Key>(KT_SPECIAL, k));
		}
		else {
			mc->KeyEvents.insert(make_shared<TEvent_Key>(KT_KEY, k));
		}		
	}
}
void TMainController::MainLoop() {

	while (AppStatus == MC_AS_WORKING) {
		// проверка наличия сообщений клавиатуры
		if (KeyEvents.HaveEvents()) {
			deque< shared_ptr<IEvent> > q;
			KeyEvents.extract(&q);
			for(auto e : q){ 
				/*screen.debug_write(
					"key type = " +
					to_string((int)static_cast<TEvent_Key*>(e.get())->keyType()) +
					"  value = " +
					to_string((int)static_cast<TEvent_Key*>(e.get())->keyValue()));*/

				/*if ((int)static_cast<TEvent_Key*>(e.get())->keyType() == 0 && (int)static_cast<TEvent_Key*>(e.get())->keyValue() == 96) {
					AppStatus = MC_AS_STOPED;
				}*/
				bool b = false;
				UI.DispatchKey(static_cast<TEvent_Key*>(e.get()), b);//отправка кнопок в интерфейс
			}
		}
		//проверка сообщений приложения
		else if (AppEvents.HaveEvents()){
			deque< shared_ptr<IEvent> > q;
			AppEvents.extract(&q);
			for (auto e : q) {
				bool disp = false;
				IClient* cl = nullptr;
				switch (e.get()->type())
				{
				case ET_DEBUGTEXT:
					screen.debug_write(static_cast<TEvent_DebugText*>(e.get())->msg());
					disp = true;
					break;
				case ET_SEND_SRVNAME:
					//установка имени сервера для клиента
					cl = ClientByID(static_cast<TEvent_net_send_ServerName*>(e.get())->CLID());
					if (cl)	cl->SrvName = static_cast<TEvent_net_send_ServerName*>(e.get())->Name();
					break;
				case ET_SEND_LOGINPASS:
					// в теории оно сюда не попадёт...
					break;
				case ET_SEND_AUTHSTATUS:
					// прохождение авторизации клиентом
					if (static_cast<TEvent_net_send_AuthStatus*>(e.get())->Stat()) {
						ClientByID(static_cast<TEvent_net_send_AuthStatus*>(e.get())->CLID())->authorised = true;
						ClientByID(static_cast<TEvent_net_send_AuthStatus*>(e.get())->CLID())->Name = static_cast<TEvent_net_send_AuthStatus*>(e.get())->Name();
					}
					break;
				case ET_SEND_HISTMESSAGE:
					// получение сообщения от сервера
					ClientByID(static_cast<TEvent_net_send_HistMesage*>(e.get())->CLID())->data.InsertMessage(
						  static_cast<TEvent_net_send_HistMesage*>(e.get())->UserName()
						, (static_cast<TEvent_net_send_HistMesage*>(e.get())->Whisp())?"you":"ALL"
						, static_cast<TEvent_net_send_HistMesage*>(e.get())->Text());
					break;
				case ET_SEND_MESSAGE:
					// в теории оно сюда не попадёт...
					break;
				case ET_SEND_DISCONNECT:
					//отключение клиента
					ClientByID(static_cast<TEvent_net_send_Disconnect*>(e.get())->CLID())->Disconnect();
					break;
				case ET_SEND_EMPTY:
					disp = true;
					break;									
				default:
					break;
				}
				UI.DispatchEvent(e.get(), disp);//отправка сообщения в интерфейс
			}
		}
		else {
			// удаление отключенных клиентов
			for (int n = 0; n < clients.size();n++) {
				IClient* c = clients[n];
				if (c->Status() != CLI_STATUS_CONNECTED) {
					clients.erase(clients.begin()+n);
					n--;
					delete c;
					UI.ReDraw();
				}
			}
			//обновление интерфейса
			UI.Update();
			UI.Draw();
			screen.draw();
			this_thread::sleep_for(std::chrono::milliseconds(20));
		}

	}
}
void TMainController::SendAppEvent(shared_ptr<IEvent> e) {
	AppEvents.insert(e);
}
void TMainController::AddClient(string srvIP, string srvPort) {
	clients.push_back(new IClient(this,srvIP,srvPort));
}
bool TMainController::CreateClientConnection(string ip, string port) {
	IClient* cl;
	try {//можно это засчитать как применение исключений и обработку ошибок? :)
		cl = new IClient(this, ip, port);
	}
	catch (...) {
		return false;
	}
	if (cl->Status() == CLI_STATUS_FAIL) {
		delete cl;
		return false;
	}
	clients.push_back(cl);
	return true;
}