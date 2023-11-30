#pragma once
#include "Const.h"
#include "Predef.h"
using namespace std;

//������
struct IEvent
{
	IEvent() : event_type(ET_NONE) {};
	IEvent(int evt) :event_type(evt) {};
	virtual ~IEvent() = 0;
	int type() //��� ���������
		{ return event_type; };
	virtual string ToString() { return ""; };//��������� ������ ������ �� ����. ��������� ��� "_send_" ���������
protected:
	int event_type;	
};
// ������� ����������
struct TEvent_Key : 
	public IEvent
{
public:
	TEvent_Key(char kt,char kv) : IEvent(ET_KEY), kType(kt), kValue(kv) {};
	int keyType() 	{	return kType;   }  //��� ������
	int keyValue()	{	return kValue;	}  //��������
private:
	int kType;
	int kValue;
};
//��������� �� ������... ������������ �� �������
struct TEvent_ErrMsg :
	public IEvent
{
public:	
	TEvent_ErrMsg(string ErrMessage, bool critical) 
		: IEvent(ET_ERRMSG)
		, _msg(ErrMessage)
		, _critical(critical) 
	{};
	string msg() { return _msg;	}
	bool critical()	{ return _critical;	}
private:
	string _msg;
	bool _critical;
};
//����� ������ �������
struct TEvent_DebugText :
	public IEvent
{
public:
	TEvent_DebugText(string ErrMessage) 
		: IEvent(ET_DEBUGTEXT)
		, _msg(ErrMessage) 
	{};
	string msg() { return _msg;	}
private:
	string _msg;
};

//�������� ����� ������� ��������
struct TEvent_net_send_ServerName :
	public IEvent
{
public:
	TEvent_net_send_ServerName(string name, int clid)
		:IEvent(ET_SEND_SRVNAME)
		,sName(name)
		,_clid(clid)
	{};
	string Name() { return sName; }
	auto CLID() { return _clid; };
	string ToString() override;
private:
	string sName;
	int _clid;//���� ����������� �������
};
//������ ����������� �� ������� �� ������
struct TEvent_net_send_LoginPass :
	public IEvent
{
public:
	TEvent_net_send_LoginPass(string login, string pass)
		:IEvent(ET_SEND_LOGINPASS)
		, _login(login)
		, _pass(pass)
	{};
	string Login() { return _login; }
	string Pass() { return _pass; }
	string ToString() override;
private:
	string _login;
	string _pass;
};
//��������� ������� ����������� � ������� �� ������
struct TEvent_net_send_AuthStatus :
	public IEvent
{
public:
	TEvent_net_send_AuthStatus(bool stat,string name, int clid)
		:IEvent(ET_SEND_AUTHSTATUS)
		, _st(stat)
		,_clid(clid)
		, _name(name)
	{};
	bool Stat() { return _st; }
	auto CLID() { return _clid; }
	auto Name() { return _name; }
	string ToString() override;
private:
	bool _st;
	int _clid;
	string _name;
};
//�������� ������ ��������� � ������� �� ������.
struct TEvent_net_send_HistMesage :
	public IEvent
{
public:
	TEvent_net_send_HistMesage(time_t t, string username, string txt, bool whisp, int clid)
		:IEvent(ET_SEND_HISTMESSAGE)
		, _t(t)
		, _un(username)
		, _txt(txt)
		,_whisp(whisp)
		, _clid(clid)
	{};
	auto Time() { return _t; }
	auto UserName() { return _un; }
	auto Text() { return _txt; }
	auto CLID() { return _clid; }
	auto Whisp() { return _whisp; }
	string ToString() override;
private:
	time_t _t;//����� �������
	string _un;//�����
	string _txt;//�����
	bool _whisp;//����� - ��������� ������� ������ ������- ����������
	int _clid;
};
//�������� ��������� � �������
struct TEvent_net_send_Mesage :
	public IEvent
{
public:
	TEvent_net_send_Mesage(string to, string msg)
		:IEvent(ET_SEND_MESSAGE)		
		, _to(to)
		, _msg(msg)
	{};	
	auto To() { return _to; }
	auto Msg() { return _msg; }
	string ToString() override;
private:	
	string _to;//����
	string _msg;//���
};
//��������� �� ���������� �������
struct TEvent_net_send_Disconnect :
	public IEvent
{
public:
	TEvent_net_send_Disconnect(string name, int clid)
		:IEvent(ET_SEND_DISCONNECT)
		,_name(name)
		, _clid(clid)
	{};
	string ToString() override;
	auto Name() { return _name; };
	auto CLID() { return _clid; };
private:
	string _name;
	int _clid;
};
//��������
struct TEvent_net_send_Empty :
	public IEvent
{
public:
	TEvent_net_send_Empty()
		:IEvent(ET_SEND_EMPTY)
	{};
};

