#include "TEvent.h"
IEvent::~IEvent() {}
string TEvent_net_send_ServerName::ToString() {
	return string("") + MSG_SPLIT + to_string(event_type) + MSG_ARG_SPLIT + sName + MSG_ARG_SPLIT + MSG_SPLIT;
}
string TEvent_net_send_LoginPass::ToString() {
	return string("") + MSG_SPLIT + to_string(event_type) + MSG_ARG_SPLIT + _login + MSG_ARG_SPLIT + _pass + MSG_ARG_SPLIT + MSG_SPLIT;
}
string TEvent_net_send_AuthStatus::ToString() {
	return string("") + MSG_SPLIT + to_string(event_type) + MSG_ARG_SPLIT + ((_st)?"SUCESS":"DENY") + MSG_ARG_SPLIT+ _name + MSG_ARG_SPLIT + MSG_SPLIT;
}
string TEvent_net_send_HistMesage::ToString() {
	return string("") + MSG_SPLIT + to_string(event_type) + MSG_ARG_SPLIT + to_string(_t) + MSG_ARG_SPLIT + _un + MSG_ARG_SPLIT + _txt + MSG_ARG_SPLIT + ((_whisp)?"W":"A") + MSG_ARG_SPLIT + MSG_SPLIT;
}
string TEvent_net_send_Mesage::ToString() {
	return string("") + MSG_SPLIT + to_string(event_type) + MSG_ARG_SPLIT + _to + MSG_ARG_SPLIT + _msg + MSG_ARG_SPLIT + MSG_SPLIT;
}
string TEvent_net_send_Disconnect::ToString() {
	return string("") + MSG_SPLIT + to_string(event_type) + MSG_ARG_SPLIT + _name + MSG_ARG_SPLIT + MSG_SPLIT;
}