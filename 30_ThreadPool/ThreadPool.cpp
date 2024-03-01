#include "ThreadPool.h"
#include <iostream>

using namespace std;
TP*  TP::pool = nullptr;
thread_local TP_job* TP::job = nullptr;

TP::TP() {
	int threadscount = thread::hardware_concurrency();
	if (threadscount < 2) threadscount = 2;
	//threadscount = 1;

	for (int i = 0; i < threadscount; i++) {
		th.push_back(new TP_thread(i,this));
	}
	pool = this;
}

jobType TP::make_job() {
	std::lock_guard<std::mutex> lk(m_jobs);
	jobType newJ = make_shared<TP_job>(this);
	jobs.push_back(newJ);
	return newJ;
}
void TP::push_task(TP_job* job, pFunc func) {
	th[taskn % th.size()]->push_task(new TP_task(job, func));	
	taskn++;
}
TP_task* TP::get_task() {
	TP_task* t;
	for (int i = 0; i < th.size(); i++) {
		t = th[i]->pop_task();
		if (t) return t;
	}
	return nullptr;
}

TP::~TP() {	
	for (auto t : th) {
		delete t;
	}
}
//==================================================================================================
TP_job::TP_job(TP* ptp) {
	tp = ptp;
	started = false;
	finished = false;
	fstart = nullptr;
	ffree = nullptr;
	fc_step = 0;
	tasks_started.store(0);
}

void TP_job::Set_Start_Function(pFunc func) {
	if (started) return;
	fstart = func;
}
void TP_job::Add_Continue_Function(pFunc func) {
	if (started) return;
	fcontinue.push_back(func);

}
void TP_job::Set_FreeMem_Function(pFreeFunc func) {
	if (started) return;
	ffree = func;
}

void TP_job::Start() {
	started = true;
	finished = false;
	fc_step = 0;
	push_task(fstart);
}
void TP_job::Wait() {
	std::unique_lock<std::mutex> lg(m_wait);
	while(!finished) cv_wait.wait(lg, [] {return true; });
}
void TP_job::Task_Finished() {
	tasks_started += -1;
	if (tasks_started.load() != 0) return;
	int tr = tasks_started.exchange(1);
	if (tr != 0) return;
	if (fcontinue.size() > fc_step) {
		tasks_started += -1;
		fc_step++;
		push_task(fcontinue[fc_step - 1]);
	}
	else {
		finished = true;
		std::unique_lock<std::mutex> lg(m_wait);
		cv_wait.notify_all();
	}
}
void TP_job::push_task(pFunc f) {
	tasks_started += 1;
	tp->push_task(this, f);
}
TP_job::~TP_job() {
	if (ffree)
		ffree(nullptr);	
}

//==================================================================================================
TP_thread::TP_thread(int n, TP* parent) {
	thnum = n;
	doit = true;
	tp = parent;
	th = thread(&TP_thread::ThreadFunc,this);
}
void TP_thread::push_task(TP_task* task) {
	tasks.push(task);
	cv.notify_all();
}
TP_task* TP_thread::pop_task() {
	return tasks.pop();
}
void TP_thread::ThreadFunc() {
	TP_task* task = nullptr;
	while (doit) {
		if (!task) {
			task = tasks.pop();
			if (!task) 
				task = tp->get_task();			
		}
		if (task) {
			TP::job = task->job;
			task->f();
			TP::job->Task_Finished();
			TP::job = nullptr;
			delete task;
			task = nullptr;
		}
		else {
			std::unique_lock<std::mutex> lk(cm);
			cv.wait(lk);
		}
	}
}
TP_thread::~TP_thread() {
	doit = false;
	{
		std::unique_lock<std::mutex> lk(cm);
		cv.notify_all();
	}
	th.join();
}
//========================================================================
TP_task::TP_task(TP_job* j, pFunc func) {
	job = j;
	f = func;
	//mt_obj += 1;
}
TP_task::~TP_task() {
	//mt_obj -= 1;
}