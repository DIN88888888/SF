#include "TEventQue.h"


TEventQue::TEventQue():empty(true){};
TEventQue::~TEventQue(){};
void TEventQue::insert(shared_ptr<IEvent> item){
	q_mutex.lock();
	items.push_back(item);
	empty = false;
	q_mutex.unlock();
}
void TEventQue::extract(deque<shared_ptr<IEvent>>* q){
	q_mutex.lock();	
	swap(*q,items);
	empty = true;
	q_mutex.unlock();	
}

bool TEventQue::HaveEvents() {
	return !empty;
}