#pragma once
#include <atomic>
//atomic block guard
class ABG
{
public:
	ABG(std::atomic<int>* mm):m(mm), locked(false) {
		lock();
	};
	ABG(std::atomic<int>& mm,bool status) :m(&mm), locked(status) {};
	~ABG() {
		unlock();
	};

	void unlock() {
		if (locked)
			m->store(0);
		locked = false;
	};
	void lock() {
		while (m->exchange(1) != 0);
		locked = true;
	};
private:
	std::atomic<int>* m;
	bool locked;
};

