/*класс - накопитель сообщений*/
/*вроде как стандартные контэйнеры потокобезопасны... но я им не верю*/
#pragma once
#include "Const.h"
#include "Predef.h"

#include "TEvent.h"

using namespace std;

class TEventQue
{
public:
	TEventQue();
	~TEventQue();

	void insert(shared_ptr<IEvent> item);
	void extract(deque<shared_ptr<IEvent>> *q);

	bool HaveEvents();
private:
	deque<shared_ptr<IEvent>> items;
	mutex q_mutex;
	bool empty;
};
