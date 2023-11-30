#include "TServer.h"
#include "TMainController.h"

#define DEBUG(MSG) mc()->Screen()->debug_write(MSG);

TServer::TServer(TMainController* mc):_mc(mc) {
    _srvStatus = 0;
    //так и не понял как это работает... главное верить...
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        _mc->SendAppEvent(make_shared<TEvent_ErrMsg>("SRV:WSA startup fails",true));
        _srvStatus = SRV_STATUS_FAIL;
        return;
    }    

    _port = SRV_DEFAULT_PORT;

    _srvStatus = SRV_STATUS_READY;
}

void TServer::Start(int port, string name) {
    if (_srvStatus != SRV_STATUS_READY) return;
    _port = port;
    _name = name;

    // create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        _mc->SendAppEvent(make_shared<TEvent_ErrMsg>("SRV:could not create socket", true));
        _srvStatus = SRV_STATUS_FAIL;
        return;
    }    

    // prepare the sockaddr_in structure
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // bind socket
    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        _mc->SendAppEvent(make_shared<TEvent_ErrMsg>("SRV:could not bind socket", true));
        _srvStatus = SRV_STATUS_FAIL;        
        return;
    }    
    _srvStatus = SRV_STATUS_STARTED;
    thlisen = thread(ListenJob, this);    //запуск потока приёма клиентов 
}

void TServer::ListenJob(TServer* srv) {
    // listen to incoming connections
    TIMEVAL delay{ 0,25 };
    listen(srv->server_socket, 1);
    fd_set readfds;
    while (srv->_srvStatus == SRV_STATUS_STARTED) {
        // clear the socket fdset
        FD_ZERO(&readfds);

        // add master socket to fdset
        FD_SET(srv->server_socket, &readfds);
        // wait for an activity on any of the socket, timeout is NULL, so wait indefinitely
        if (select(0, &readfds, NULL, NULL, &delay) == SOCKET_ERROR) {
            srv->_mc->SendAppEvent(make_shared<TEvent_ErrMsg>("SRV:select function call failed with error code :" + to_string(WSAGetLastError()), true));
            srv->_srvStatus = SRV_STATUS_FAIL;
            return;
        }

        // if something happened on the master socket, then its an incoming connection
        SOCKET new_socket; // new client socket
        sockaddr_in address;
        int addrlen = sizeof(sockaddr_in);
        if (FD_ISSET(srv->server_socket, &readfds)) {
            if ((new_socket = accept(srv->server_socket, (sockaddr*)&address, &addrlen)) < 0) {
                srv->_mc->SendAppEvent(make_shared<TEvent_ErrMsg>("SRV:accept function error", false));
                continue;
            }
            //запускаем поток обмена с клиентом
            TServer_Connection* new_client = srv->CreateClientConnection(new_socket);
            new_client->SendMsg(make_shared<TEvent_net_send_ServerName>(srv->_name,0));           
        }
        //удаляем отключившехся клиентов
        for (int n = 0; n < srv->clients.size();n++) {
            TServer_Connection* c = srv->clients[n];            
            if (!c->Active()) {
                srv->clients.erase(srv->clients.begin()+n);
                n--;
                delete c;
            }
        }
    }
}

TServer_Connection* TServer::CreateClientConnection(SOCKET socket) {
    clients.push_back(new TServer_Connection(socket, this));
    return clients.back();
}
void TServer::SendMessageToClient(string userName, string message) {
    for (auto c : clients) {
        if (userName == "")
        {
            SendMessageToClient(c, message);
        }
    }
}
void TServer::SendMessageToClient(TServer_Connection* c, string message) {
        send(c->Socket(), message.c_str(), message.length(), 0);
}
void TServer::Stop() {
    //я пытался... но чёт не получилось. падает повторная инициализация.
    for (auto c : clients) {
        c->SendMsg(make_shared<TEvent_net_send_Disconnect>(c->AuthorizedUserName(), 0));        
    }
    this_thread::sleep_for(std::chrono::milliseconds(3000));

    _srvStatus = SRV_STATUS_STOPING;
    if (thlisen.joinable())
        thlisen.join();

    for (auto c : clients) {
        c->Stop();
        delete c;
    }
    WSACleanup();

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        _mc->SendAppEvent(make_shared<TEvent_ErrMsg>("SRV:WSA startup fails", true));
        _srvStatus = SRV_STATUS_FAIL;
        return;
    }

    _port = SRV_DEFAULT_PORT;    

    _srvStatus = SRV_STATUS_READY;
}
TServer::~TServer() {
    Stop();    
   
}
//добавление текстовых сообщений в базу сервера и отправка их клиентам
void TServer::InsertNewMessage(string author, string to, string msg) {
    data.InsertMessage(author, to, msg);//добавляем в базу
    bool toall = to == "ALL";//сообщение для всех?
    for (auto uc : clients) {
        if (uc->AuthorizedUserName().length() > 0) {//пользователь авторизовался
            bool whisp = to == uc->AuthorizedUserName();        //это сообщение только для этого пользователя?
            bool self = author == uc->AuthorizedUserName();//это автор сообщения?
            if ( toall || whisp || self) {//отправляем если звёзды сошлись
                uc->SendMsg(make_shared<TEvent_net_send_HistMesage>(0, author, msg, !toall, 0));
            }
        }
    }
}