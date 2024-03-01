#pragma once
#include <future>
#include <mutex>
#include "TSQueue.h"
#include <thread>
#include <condition_variable>

class TP;
class TP_job;
class TP_task;
class TP_taskQueue;
class TP_thread;

using jobType = std::shared_ptr<TP_job>;
using pFunc = std::function<void()>;
using pFreeFunc = std::function<void(void *)>;

class TP_job
{
public:
	TP_job(TP* ptp);
	TP_job() = delete;
	TP_job(TP_job&) = delete;
	TP_job(TP_job&&) = delete;

	void Set_Start_Function(pFunc func);
	void Add_Continue_Function(pFunc func);
	void Set_FreeMem_Function(pFreeFunc func);

	void push_task(pFunc);
	void Task_Finished();
	void Start();
	void Wait();
	~TP_job();
private:
	TP* tp;
	bool started;
	bool finished;

	pFunc fstart;
	int fc_step;
	std::vector<pFunc> fcontinue;
	pFreeFunc ffree;

	std::atomic<int> tasks_started;
	
	std::mutex m_wait;
	std::condition_variable cv_wait;
};

class TP
{
public:
	TP();
	~TP();
	static TP* pool;
	thread_local static TP_job* job;

	jobType make_job();	

	TP_task* get_task();
private:
	std::vector<TP_thread*> th;
	int taskn;

	std::mutex m_jobs;
	std::vector <jobType> jobs;

	void push_task(TP_job*job, pFunc func);

	friend void TP_job::push_task(pFunc);
};



struct TP_task
{
	TP_task(TP_job* j, pFunc func);
	TP_task() = delete;
	TP_task(TP_task&) = delete;
	TP_task(TP_task&&) = delete;
	~TP_task();
	TP_job* job;
	pFunc f;
};

class TP_thread
{
public:
	TP_thread(int n, TP* parent);
	~TP_thread();

	void push_task(TP_task* task);
	TP_task* pop_task();

	void ThreadFunc();	
private:
	TP_thread() = delete;
	TP_thread(TP_thread&) = delete;
	TP_thread(TP_thread&&) = delete;
	
	TP* tp;
	int thnum;
	bool doit;
	std::mutex cm;
	std::condition_variable cv;
	std::thread th;
	TSQueue<TP_task> tasks;	
};

