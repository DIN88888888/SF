#include "TNetIOParser.h"

void TNetIOParser::AddData(char* buf) {
	bufer += buf;
	checkData();
}
void TNetIOParser::checkData() {
	int first = -1;
	int sec = -1;
	int i;
	//ищем разделители и записываем, если нашли
	for (i = 0; i < bufer.length(); i++)
		if (bufer[i] == MSG_SPLIT) {
			first = i++;
			break;
		}
	for (; i < bufer.length(); i++)
		if (bufer[i] == MSG_SPLIT) {
			sec = i;
			break;
		}
	_haveDataToRead = false;
	if (first >= 0 && sec >= 0) {
		_haveDataToRead = true;
		_datast = first;
		_dataen = sec;
	}	
	//что-то пошло не так - очищаем буфер
	if (first == -1)
		bufer = "";
}

shared_ptr<IEvent> TNetIOParser::GetNextEvent() {
	vector<string> data;
	int pos = _datast+1;
	int lpos = _datast+1;
	//вырезаем из буфера данные и запихиваем в вектор
	for (; pos < _dataen; pos++) {
		if (bufer[pos] == MSG_ARG_SPLIT) {
			data.push_back(bufer.substr(lpos,pos-lpos));
			lpos = pos + 1;
		}
		else if ((bufer[pos] == 1)) {
			break;
		}		
	}
	bufer.erase(0,_dataen+1);
	
	checkData();
	return CreateIncomingEvent(data);
}

shared_ptr<IEvent>  TNetIOParser::CreateIncomingEvent(vector<string> &v) {
	if (v.size() == 0) return make_shared<TEvent_net_send_Empty>();
	switch (stoi(v[0]))
	{
	case ET_SEND_SRVNAME:
		if(v.size()==2)return make_shared<TEvent_net_send_ServerName>(v[1],ClientID);
	case ET_SEND_LOGINPASS:
		if (v.size() == 3)return make_shared<TEvent_net_send_LoginPass>(v[1], v[2]);
	case ET_SEND_AUTHSTATUS:
		if (v.size() == 3)return make_shared<TEvent_net_send_AuthStatus>(v[1]=="SUCESS", v[2],ClientID);
	case ET_SEND_HISTMESSAGE:
		if (v.size() == 5)return make_shared<TEvent_net_send_HistMesage>(stoi(v[1]),v[2],v[3],v[4]=="W", ClientID);
	case ET_SEND_MESSAGE:
		if (v.size() == 3)return make_shared<TEvent_net_send_Mesage>(v[1], v[2]);
	case ET_SEND_DISCONNECT:
		if (v.size() == 2)return make_shared<TEvent_net_send_Disconnect>(v[1], ClientID);
	default: return make_shared<TEvent_net_send_Empty>();		
	}	
}