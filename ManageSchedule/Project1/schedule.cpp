#include "schedule.h"

int flag;
mutex mtx;

Schedule::Schedule(){
	e_ = default_random_engine(time(NULL));
}

Schedule::Schedule(const Schedule& s):
	crash_(s.crash_), 
	outtime_(s.outtime_),
	timetables_(s.timetables_),
	clsque_(s.clsque_),
	teachers_(s.teachers_),
	e_(s.e_){
	//每个课表进行拷贝的时候需要进行相应的修改
	/*因为当前只有时间是准确的，对于classunit当中的ttbptr_
	和timetable_当中的所有的值都需要进行修正
	*/
	UpdatePtrs();
}

Schedule & Schedule::operator=(const Schedule & s){
	crash_ = s.crash_;
	outtime_ = s.outtime_;
	timetables_ = s.timetables_;
	clsque_ = s.clsque_;
	teachers_ = s.teachers_;
	e_ = s.e_;
	UpdatePtrs();
	return *this;
}

void Schedule::GetSchedule(InterruptibleThread * t, future<Schedule>* fut) {
	auto t1 = chrono::system_clock::now(), t2 = t1;
	chrono::duration<int, ratio<60, 1>> dur(outtime_);
	
	while (true) {
		t2 = chrono::system_clock::now();
		CalFitness();
		if (crash_ == 0) {
			break;
		}
		if (t2 - t1 > dur) {
			break;
		}
		try {
			interruption_point();
		}
		catch(const thread_interrupted& interrupt){
			break;
		}
		Cross();
		CalFitness();
		Modify();
	}
	if (flag) {
		mtx.lock();
		if (flag && crash_ == 0) {
			flag = 0;
			t->pro_ptr_->set_value(*this);
		}
		mtx.unlock();
	}
}

void Schedule::init() {
	UpdatePtrs();
	//2.然后安排每节课的时间
	for (auto& t : timetables_)
		t.Init();
}

void Schedule::CalFitness() {
	/*
	检查每节课是否都符合要求
	*/
	for (auto& c : clsque_)
		crash_ += c.CalFitness();
}

void Schedule::UpdatePtrs() {
	for (auto& c : clsque_) {
		//1.先将老师和自己的每节课进行挂钩
		teachers_[c.GetTeacherIdInVec()].clsque_.push_back(&c);
		c.teacher_ = &teachers_[c.GetTeacherIdInVec()];
		
		//2.把这个课和相应的上课教室挂钩
		timetables_[c.GetTimeTableIdInVec()].clsque_.push_back(&c);
		c.ttbptr_ = &timetables_[c.GetTimeTableIdInVec()];
		c.headptr_ = &(c.ttbptr_->roomtable_[c.stime_.first][c.stime_.second]);
		
		//3.更新课的课表当中的指向
		for (auto i = 0; i < c.GetDuration(); i++) {
			c.ttbptr_->roomtable_[c.stime_.first][c.stime_.second + i] = &c;
		}

		//4.更新合班课的指针指向
		for (auto i = 0; i < c.unioncls_.size(); i++) {
			c.unioncls_[i] = &(clsque_[c.unioclsid_[i]]);
		}
	}
}

void Schedule::Modify() {
	//先判冲突再解决冲突
	for (auto& c : clsque_) {
		if ((!c.CheckPeriod(c.stime_)) || (!c.teacher_->CheckUnit(&c))) {
			//发现需要进行调换	
			NeedToSwap(c);
		}
	}

}

void Schedule::NeedToSwap(ClassUnit& firstcls) {
	//这节课需要被进行调换
	//先获得可用时间的交集
	vector<pair<int, int>> canbeput = firstcls.GetRandAvailTime();
	set<pair<int, int>> teavper = firstcls.teacher_->GetAvailPeriods();
	vector<int> normaleappear = firstcls.teacher_->normalappear_;
	for (auto it = canbeput.begin(); it != canbeput.end();) {
		if (!firstcls.GetType()) {
			if(!normaleappear[it->first])
				it = canbeput.erase(it);
		}
		if (teavper.find(*it) == teavper.end()) {
			it = canbeput.erase(it);
		}
		else it++;
	}
	//剩下的时间段就是可以进行调换的	
	pair<int, int> tmp;
	for (auto i = 0; i < canbeput.size(); i++) {
		//timedelta对于0组来说是要进行相减的，1组的是要进行相加
		tmp.first = firstcls.stime_.first - canbeput[i].first;
		tmp.second = firstcls.stime_.second - canbeput[i].second;
		//表示融合成功并且交换结束
		if (UnionClsUnits(firstcls, tmp)) {
			//表示已经成功放入wait4swap当中，然后准备相互之间的交换
			break;
		}
	}	
}

void Schedule::Cross() {
	/*
	这里比modify少判断教师上课情况，
	也就是这里只判断该节课自己的能否被安排的情况，
	*/
	double cp;
	uniform_real_distribution<double> u(0.0, 1.0);
	for (auto& c : clsque_) {
		cp = u(e_);
		if (cp < mxpocross_) {
			SwapClsUnit(c);
		}
	}

}

void Schedule::SwapClsUnit(ClassUnit& firstcls) {
	vector<pair<int, int>> canbeput = firstcls.GetRandAvailTime();
	if (canbeput.empty())return;
	//0用来装当前的节次组，1用来装将来的节次组
	pair<int, int> tmp;
	for (auto i = 0; i < canbeput.size(); i++) {
		//timedelta对于0组来说是要进行相减的，1组的是要进行相加
		tmp.first = firstcls.stime_.first - canbeput[i].first;
		tmp.second = firstcls.stime_.second - canbeput[i].second;
		//表示融合成功并且交换结束
		if (UnionClsUnits(firstcls, tmp)) {
			//表示已经成功放入wait4swap当中，然后准备相互之间的交换
			break;
		}
	}
}

bool Schedule::UnionClsUnits(ClassUnit& firstcls, pair<int, int> timedelta) {
	//准备工作
	//wait4swap用来装ttb当中的指针，而不是clsunit的地址
	vector<vector<ClassUnit**>> wait4swap{ 2 };
	//wait4swap[0].push_back(firstcls.ttbptr_->GetClsUnitPtr(firstcls.stime_.first, firstcls.stime_.second));
	vector<vector<ClassUnit**>::iterator> it;
	for (auto i = 0; i < 2; i++) {
		it.push_back(wait4swap[i].begin());
	}
	
	//通过bfs整个队列进行union操作
	set<ClassUnit**> clstab;
	if (!AddUnitPtrIntoVec(firstcls.headptr_, timedelta, 0, wait4swap, clstab))return false;
	//clstab.insert(firstcls.ttbptr_->GetClsUnitPtr(firstcls.stime_.first, firstcls.stime_.second));
	//lambda
	auto fun = [&]()-> bool {
		for (auto i = 0; i < 2; i++) {
			if (it[i] != wait4swap[i].end())
				return true;
		}
		return false;
	};
	//用来标示是否成功把某节次的课放入队列当中，如果当中有一个失败则这次就都失败
	while (fun()) {
		for (auto i = 0; i < 2; i++) {
			if (it[i] != wait4swap[i].end()) {
				if ((*(it[i]) != NULL) && (!AddUnitPtrIntoVec(*(it[i]), timedelta, i, wait4swap, clstab))) {
					return false;
				}
				it[i]++;
			}
		}
	}
	SwapUnitsInVec(wait4swap);
	return true;
}

bool Schedule::AddUnitPtrIntoVec(ClassUnit** cptr, pair<int, int> timedelta,
	int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab) {
	//将当前这个节次的课和这个节次对应的课都放到列表当中
	auto cls = *(*cptr);//cls是这节课
	//从当前这个节次开始
	for (auto i = 0; i < cls.GetDuration(); i++) {
		//当前这个部分
		if(!PutInSetVec(cptr, timedelta, k, i, wait4swap, clstab))return false;
		//if(!PutInSetVec(cptr, timedelta, 1 - k, i, wait4swap, clstab))return false;
	}
	//再判断这个是否是合班课
	if (cls.unioclsid_.size()) {
		for (auto& uc : cls.unioncls_) {
			if (!PutInSetVec(uc->headptr_, timedelta, k, 0, wait4swap, clstab))return false;
			//if (!PutInSetVec(uc->headptr_, timedelta, 1 - k, 0, wait4swap, clstab))return false;
		}
	}
	return true;
}

bool Schedule::PutInSetVec(ClassUnit ** cptr, pair<int, int> timedelta,
	int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab) {
	//此函数将需要相互交换的两节课都一起放进去
	//此部分需要加入检查机制，检查调换区域是否能够被放过去
	auto cls = *(*cptr);//cls是当前这节课
	pair<int, int> tmp;
	//这里的tmp表示将要被调过去的时间
	if (k)tmp = pair<int, int>(cls.stime_.first + timedelta.first, cls.stime_.second + timedelta.second + add);
	else tmp = pair<int, int>(cls.stime_.first - timedelta.first, cls.stime_.second - timedelta.second + add);
	//target表示需要被交换的那节课
	auto target = *(cls.ttbptr_->roomtable_[tmp.first][tmp.second]);
	if (!CanBeSwap(cls, target))return false;

	//c表示要放到vec和set当中的这个节次
	auto c = &(cls.ttbptr_->roomtable_[cls.stime_.first][cls.stime_.second + add]);
	auto tp = &(target.ttbptr_->roomtable_[target.stime_.first][target.stime_.second]);
	if (clstab.find(c) == clstab.end() && clstab.find(tp) == clstab.end()) {
		clstab.insert(c);
		clstab.insert(tp);
		wait4swap[k].push_back(c);
		wait4swap[1 - k].push_back(tp);
	}
	return true;
}

bool Schedule::CanBeSwap(ClassUnit c, ClassUnit target) {
	//检查两个将要被放入set和vec当中的两节课能否相互被交换
	if (!c.CheckPeriod(target.stime_))return false;
	if (!target.CheckPeriod(c.stime_))return false;
	return true;
}

void Schedule::SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap) {
	//把队列当中的节次都进行相互交换
	for (auto i = 0; i < wait4swap.size(); i++) {
		auto origin = *(wait4swap[0][i]);
		auto target = *(wait4swap[1][i]);
		if(origin->headptr_ == wait4swap[0][i])
			origin->ttbptr_->SwapUnits(origin, target);
		*(wait4swap[0][i]) = &(*target);
		*(wait4swap[1][i]) = &(*origin);
	}
}

