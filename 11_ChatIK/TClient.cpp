#include "TClient.h"
#include "TMainController.h"

#define DEFAULT_BUFLEN 4096
#define DEBUG(MSG) mc()->Screen()->debug_write(MSG)
int IClient::IdGen = 0;
IClient::IClient(TMainController* mc, string server_ip, string port)
    :_mc(mc)
    , _srvIP(server_ip)
    , pars(IdGen)
    , _port(port)
    , ID(IdGen)
    , authorised(false)
{
    //настройки сети использую "на ощупь", просто работает...
    IdGen++;
    addrinfo hints = {};    
    _cliStatus = 0;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // resolve the server address and port
    addrinfo* result = nullptr;
    int iResult = getaddrinfo(_srvIP.c_str(), _port.c_str(), &hints, &result);
    if (iResult != 0) {
        _mc->SendAppEvent(make_shared<TEvent_ErrMsg>("CLI: getaddrinfo fails", false));
        _cliStatus = SRV_STATUS_FAIL;
        return;
    }

    addrinfo* ptr = nullptr;
    // attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // create a client-side socket for connecting to server
        client_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (client_socket == INVALID_SOCKET) {            
            _mc->SendAppEvent(make_shared<TEvent_ErrMsg>("CLI: socket failed with error:" + to_string(WSAGetLastError()), false));
            _cliStatus = SRV_STATUS_FAIL;
            return;
        }

        // connect to server
        iResult = connect(client_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(client_socket);
            client_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (client_socket == INVALID_SOCKET) {
        _mc->SendAppEvent(make_shared<TEvent_ErrMsg>("CLI: Unable to connect to server!", false));
        _cliStatus = SRV_STATUS_FAIL;
        return;
    }

    _cliStatus = CLI_STATUS_CONNECTED;
    thjob = thread(ResiveJob, this);//запуск потока связи с сервером
}
void IClient::ResiveJob(IClient* cli) {
    char response[DEFAULT_BUFLEN+1];
    TIMEVAL delay{ 1,0 };
    fd_set readfds;

    while ( cli->_cliStatus==CLI_STATUS_CONNECTED) {        
        // add master socket to fdset
        FD_ZERO(&readfds);
        FD_SET(cli->client_socket, &readfds);
        // wait for an activity on any of the socket
        if (select(0, &readfds, NULL, NULL, &delay) == SOCKET_ERROR) {
            cli->_cliStatus = SRV_STATUS_FAIL;
        }
        //получение данных с сокета
        if (FD_ISSET(cli->client_socket, &readfds))
        {
            int result = recv(cli->client_socket, response, DEFAULT_BUFLEN, 0);
            response[result] = '\0';
            cli->pars.AddData(response);
        }
        //обработка входящих сообщений
        while(cli->pars.HaveDataToRead()){ 
            cli->mc()->SendAppEvent(cli->pars.GetNextEvent());
        }
        //отправка сообщений на сервер
        if (cli->sendBuf.HaveEvents()) {
            deque< shared_ptr<IEvent> > q;
            cli->sendBuf.extract(&q);
            for (auto e : q) {
                string msg = e.get()->ToString();
                if (msg.length() > 0)
                    send(cli->client_socket, msg.c_str(), msg.length(), 0);
            }

        }
        this_thread::sleep_for(chrono::milliseconds(25));        
    }
}

IClient::~IClient() {
    _cliStatus = CLI_STATUS_STOP;
    if(thjob.joinable())
        thjob.join();
}

void IClient::Disconnect() {
    _cliStatus = CLI_STATUS_STOP;
}
