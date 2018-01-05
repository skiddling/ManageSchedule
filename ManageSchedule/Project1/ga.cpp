#include "ga.h"

GA::GA() {
	num_of_threads_ = 1;
	//num_of_threads_ = thread::hardware_concurrency();
	//����һ�������Ŀα�res_��Ȼ��ͨ����������Ӧ�ĸ�������α���ȥ
	//schedule copy construction 
	schedules_ = vector<Schedule>(num_of_threads_ * thread_schedule_size_, res_);
}

GA::GA(vector<Course> courses, vector<Teacher> teachers, vector<TimeTable> timetables, vector<ClassUnit> units):
	res_(courses, teachers, timetables, units){
	//��ʼ���α��൱�����е�scheduleӵ��һ����ͬ�ĳ�ʼ���α����������Դ��Ŀ�����ʼ����ʵ��
	//�������տα��е�ָ�뻹��Ҫ�ٸ��¹���
	res_.UpdatePtrs();
	res_.init();
	num_of_threads_ = 1;
	thread_schedule_size_ = 1;
	//num_of_threads_ = thread::hardware_concurrency();
	//����һ�������Ŀα�res_��Ȼ��ͨ����������Ӧ�ĸ�������α���ȥ
	//schedule copy construction 
	schedules_ = vector<Schedule>(num_of_threads_ * thread_schedule_size_, res_);
	//���¸��Ե�ָ��
	for (auto& s : schedules_) {
		s.UpdatePtrs();
		//����ÿ���α��е�ָ��
		for (auto& c : s.clsque_) {
			c.UpdateRoomPtr();
		}
	}
	cout << "end of Ga construct" << endl;
}

void GA::GenerateTable() {
	promise<Schedule> pro;
	future<Schedule> fut = pro.get_future();
	vector<InterruptibleThread> threads(num_of_threads_);
	
	auto t1 = chrono::system_clock::now();
	chrono::duration<int, ratio<60, 1>> dur(outtime_);
	for (auto i = 0; i < num_of_threads_; i++) {
		threads[i] = InterruptibleThread(this, &GA::GetSchedule, i, &pro, &fut);
	}

	if (fut.wait_for(dur) == future_status::ready) {
		res_ = fut.get();
		cout << "crash_ is " << res_.crash_ << endl;
		for (auto& t : threads) {
			t.interrupt();
			t.join();
		}
	}
	else {
		for (auto& t : threads) {
			t.interrupt();
			t.join();
		}
		res_ = schedules_[0];
		for (auto i = 1; i < num_of_threads_ * thread_schedule_size_; i++) {
			if (res_.crash_ > schedules_[i].crash_) {
				res_ = schedules_[i];
			}
		}
	}
}

void GA::OutPutRes() {
	ofstream fout("result.txt");
	for (auto& t : res_.timetables_) {
		fout << endl << endl;
		for (auto i = 0; i < t.roomtable_.size(); i++) {
			for (auto j = 0; j < t.roomtable_[i].size(); j++) {
				if (t.roomtable_[i][j] != nullptr) {
					fout << t.roomtable_[i][j]->teacher_->dbname_ << "  " << t.roomtable_[i][j]->couptr_->course_name_ << "         ";
				}
				else fout << "           ";
			}
			fout << endl;
		}
		fout << endl << endl;
	}
	fout.close();
}

void GA::GetSchedule(int thid, InterruptibleThread* t, future<Schedule>* fut) {
	for (auto i = 0; i < thread_schedule_size_; i++) {
		schedules_[i + thid * thread_schedule_size_].GetSchedule(t, fut);
	}
}
