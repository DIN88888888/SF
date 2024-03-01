#pragma once
#include <atomic>
#include "BlockGuard.h"

template<class T> struct item {
	item() { m.store(0); data = nullptr; next = nullptr; };
	item(T* v) { m.store(0); data = v; next = nullptr; };

	std::atomic<int> m;
	T* data;
	item* next;
};

template<class T> class TSQueue
{
public:
	TSQueue() {
		last = &root;
	}
	void push(T* v) {
		while (true) {
			while (last->m.exchange(1) != 0);
			ABG g(last->m,true);
			if (last->next)
				continue;
			last->next = new item<T>(v);
			item<T>* tmp = last;

			ABG g2(&last->next->m);
			last = last->next;
			return;
		}
	}
	T* pop() {
		ABG gr(&root.m);
		if (!root.next)
			return nullptr;
		ABG gn(&root.next->m);
		if (last == root.next)
			last = &root;

		T* res = root.next->data;
		item<T>* del = root.next;
		del->data = nullptr;
		
		root.next = root.next->next;
		gn.unlock();
		delete del;

		return res;
	}	
	~TSQueue() {
		T* v = nullptr;
		do{
			v = pop();
			delete v;			
		} while (v);
	}
private:	
	item<T> root;
	item<T>* last;	

};

