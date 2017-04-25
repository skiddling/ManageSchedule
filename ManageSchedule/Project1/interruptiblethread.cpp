//#include "foo.h"
//#include "ga.h"
#include "interruptiblethread.h"

//using std::function;

//mutex mut;
//condition_variable cv;
//int tag;

//thread_local InterruptFlag this_thread_interrupt_flag;
thread_local promise<Schedule>* pro_ptr_;
//thread_local int sid;

//struct thread_interrupted {};

//void interruption_point()
//{
//	if (this_thread_interrupt_flag.is_set()) {
//		throw thread_interrupted();
//	}
//}

//void foo(InterruptibleThread& t)
//{
//	while (true) {
//		int res = t.u_(t.e_);
//		if (res == 7) {
//			if (!tag) {
//				unique_lock<mutex> lk(mut);
//				t.pro_ptr_->set_value(this_thread::get_id());
//				tag = 1;
//			}
//			break;
//		}
//		cout << this_thread::get_id() << " **** " << res << endl;
//		try {
//			interruption_point();
//		}
//		catch (const thread_interrupted& interrupt) {
//			break;
//		}
//	}
//	while (true) {
//		try {
//			interruption_point();
//		}
//		catch (const thread_interrupted& interrupt) {
//			break;
//		}
//	}
//}

//
//int main()
//{
//	uniform_int_distribution<int> u(0, 9);
//	promise<decltype(this_thread::get_id())> pro;
//	future<decltype(this_thread::get_id())> fut = pro.get_future();
//	vector<InterruptibleThread> threads;
//	cout << thread::hardware_concurrency() - 1 << endl;
//	for (auto i = 0; i < thread::hardware_concurrency() - 1; i++) {
//		threads.push_back(InterruptibleThread(foo, &pro, u));
//	}
//	if (fut.wait_for(chrono::milliseconds(1000)) == future_status::ready) {
//		cout << "get it!\n";
//	}
//
//	for (auto& t : threads) {
//		t.interrupt();
//		t.join();
//	}
//	getchar();
//	return 0;
//}