/*парсер сообщений, переданных по сети*/
#pragma once
#include "Const.h"
#include "Predef.h"

#include "TEventQue.h"
#include "TEvent.h"
class TNetIOParser
{
	/*сообщения разделены кодом 1, данные разделены кодом 2, некорректные сообщения отбрасываются*/
public:
	TNetIOParser(int CLID) 
		:bufer("") 
		, ClientID(CLID)
	{};
	void AddData(char* buf);//добавление данных

	TEventQue q;//очередь 

	bool HaveDataToRead() { return _haveDataToRead; };
	shared_ptr<IEvent> GetNextEvent();	//сообщение после распаковки
private:
	string bufer;//буфер
	bool _haveDataToRead;//после ввода данных есть что прочитать
	int _datast;//начало сообщения
	int _dataen;// конец сообщения
	int ClientID;//ид клиента для генерации сообщений

	shared_ptr<IEvent> CreateIncomingEvent(vector<string> &v);//формуем из текста - сообщение
	void checkData();
};

