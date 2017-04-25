#pragma once
//#include "ga.h"
#include "schedule.h"
//#include 


//using namespace std;

//class GA;
//
//class InterruptFlag
//{
//public:
//	inline void set() {
//		lock_guard<mutex> guard(_mtx);
//		_set = true;
//	}
//
//	inline bool is_set() {
//		lock_guard<mutex> guard(_mtx);
//		return _set;
//	}
//
//private:
//	mutex _mtx;
//	bool _set;
//};
//
extern thread_local InterruptFlag this_thread_interrupt_flag;
//extern thread_local promise<Schedule>* pro_ptr_;

//extern thread_local int sid;

//typedef void(GA::*Funcptr)(int);

class InterruptibleThread
{
public:
	//default_random_engine e_;
	//uniform_int_distribution<int> u_;
	thread _internal_thread;
	promise<Schedule>* pro_ptr_;//用来存放结果
	//int sid_;

	InterruptibleThread() {};
	template<typename FunctionType, typename ObjType>
	//template<Funcptr f>
	InterruptibleThread(ObjType objptr, FunctionType f, int i, promise<Schedule>* proptr, future<Schedule>* fut){
	//InterruptibleThread(FunctionType f, GA* ga, int i){
		//f(i);
		//pro_ptr_ = proptr;
		//此局部变量p用来对interruptflag进行设置，相当于是一个启动器，
		//给threadlocal变量interruptflag赋初值，因为interruptthread具有是否需要中断的这个开关需求（打开之后设置了中断才能表明能被打断，否则不打开即使设置了中断也无法触发中断）
		//将proprt也设置成threadlocal是因为结果被保存在相应
		promise<InterruptFlag*> p;//因为这个p只是一个传递真执行线程的this_thread_interrupt_flag的值的一个工具载体，所以其实只需要一个局部变量就行
		_internal_thread = thread([f, &p, i, &objptr, proptr, this, &fut]()
		//_internal_thread = thread([f, &p, i, &ga]()
		{
			//sid_ = sid = i;

			p.set_value(&this_thread_interrupt_flag);
			this->pro_ptr_ = proptr;//在具体执行线程当中设置该变量
			//f(ga, i);
			//auto nf = bind(f, i);
			//nf();
			((*objptr).*f)(i, this, fut);
		});
		_interrupt_flag = p.get_future().get();
	}

	inline void interrupt()
	{
		if (_interrupt_flag != nullptr)
		{
			_interrupt_flag->set();
		}
	}

	void join() {
		_internal_thread.join();
	}

	thread::id get_id() {
		return this_thread::get_id();
	}

	InterruptFlag* getInterruptFlag() {
		return &this_thread_interrupt_flag;
	}

private:
	InterruptFlag* _interrupt_flag;
};
