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
	promise<Schedule>* pro_ptr_;//������Ž��
	//int sid_;

	InterruptibleThread() {};
	template<typename FunctionType, typename ObjType>
	//template<Funcptr f>
	InterruptibleThread(ObjType objptr, FunctionType f, int i, promise<Schedule>* proptr, future<Schedule>* fut){
	//InterruptibleThread(FunctionType f, GA* ga, int i){
		//f(i);
		//pro_ptr_ = proptr;
		//�˾ֲ�����p������interruptflag�������ã��൱����һ����������
		//��threadlocal����interruptflag����ֵ����Ϊinterruptthread�����Ƿ���Ҫ�жϵ�����������󣨴�֮���������жϲ��ܱ����ܱ���ϣ����򲻴򿪼�ʹ�������ж�Ҳ�޷������жϣ�
		//��proprtҲ���ó�threadlocal����Ϊ�������������Ӧ
		promise<InterruptFlag*> p;//��Ϊ���pֻ��һ��������ִ���̵߳�this_thread_interrupt_flag��ֵ��һ���������壬������ʵֻ��Ҫһ���ֲ���������
		_internal_thread = thread([f, &p, i, &objptr, proptr, this, &fut]()
		//_internal_thread = thread([f, &p, i, &ga]()
		{
			//sid_ = sid = i;

			p.set_value(&this_thread_interrupt_flag);
			this->pro_ptr_ = proptr;//�ھ���ִ���̵߳������øñ���
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
