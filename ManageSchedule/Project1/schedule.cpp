#include "schedule.h"

int flag;
mutex mtx;

Schedule::Schedule(){
	e_ = default_random_engine(time(NULL));
}

Schedule::Schedule(vector<Course> couque, vector<Teacher> teachers, vector<TimeTable> timetables, vector<ClassUnit> clsque):
	couque_(couque), teachers_(teachers), timetables_(timetables), clsque_(clsque){ 

}

Schedule::Schedule(const Schedule& s):
	crash_(s.crash_), 
	outtime_(s.outtime_),
	timetables_(s.timetables_),
	clsque_(s.clsque_),
	teachers_(s.teachers_),
	e_(s.e_){
	//ÿ���α���п�����ʱ����Ҫ������Ӧ���޸�
	/*��Ϊ��ǰֻ��ʱ����׼ȷ�ģ�����classunit���е�ttbptr_
	��timetable_���е����е�ֵ����Ҫ��������
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

bool Schedule::init() {
	//��������Ѿ������ݿ⵱�����
	//UpdatePtrs();
	//2.Ȼ����ÿ�ڿε�ʱ��
	for (auto& c : clsque_) {
		if((c.hasbeenput_ == false) && !(c.ttbptr_->PutIntoTable(&c)))return false;
	}
	//3.����headptr
	for (auto &c : clsque_) {
		c.headptr_ = &(c.ttbptr_->roomtable_[c.stime_.first][c.stime_.second]);
	}
}

template<typename T>
void GetPtrsFromClsSet(vector<T>& vec, vector<ClassUnit>& clsque) {
	//�˴�����ʦ�����ң���Ŀ��������ָ��ȫ����������Ӧ�ĸ���
	for (auto& v : vec) {
		for (auto& t : v.clsqueindex_) {
			v.clsque_.push_back(&(clsque[t]));
			//*(static_cast<v.GetName()>(unitset[t].maptrs[v.GetName()])) = &t;
			clsque[t].maptrs[v.GetName()] = &v;
		}
	}
}

void Schedule::UpdatePtrs() {
	//��ʼ����������е�ָ��ĸ��µĹ���
	//����һ���ֳɿ�Ŀ����ʦ��������������������Ӧ��ָ��ĸ���
	GetPtrsFromClsSet(teachers_, clsque_);
	GetPtrsFromClsSet(timetables_, clsque_);
	GetPtrsFromClsSet(couque_, clsque_);
	for (auto& c : clsque_) {
		c.couptr_ = static_cast<Course*>(c.maptrs_["Course"]);
		c.teacher_ = static_cast<Teacher*>(c.maptrs_["Teacher"]);
		c.ttbptr_ = static_cast<TimeTable*>(c.maptrs_["TimeTable"]);
		//�ϰ�Ŀθ�����ָ��
		if (!c.union_cls_index_.empty()) {
			for (auto index : c.union_cls_index_) {
				c.unioncls_.push_back(&clsque_[index]);
			}
		}
	}
}

void Schedule::CalFitness() {
	/*
	���ÿ�ڿ��Ƿ񶼷���Ҫ��
	*/
	for (auto& c : clsque_)
		crash_ += c.CalFitness();
}


//void Schedule::UpdatePtrs() {
//	for (auto& c : clsque_) {
//		//1.�Ƚ���ʦ���Լ���ÿ�ڿν��йҹ�
//		teachers_[c.GetTeacherIdInVec()].clsque_.push_back(&c);
//		c.teacher_ = &teachers_[c.GetTeacherIdInVec()];
//		
//		//2.������κ���Ӧ���Ͽν��ҹҹ�
//		timetables_[c.GetTimeTableIdInVec()].clsque_.push_back(&c);
//		c.ttbptr_ = &timetables_[c.GetTimeTableIdInVec()];
//		//���headptr��Ҫ�ȵ�init֮�����ȷ������
//		//c.headptr_ = &(c.ttbptr_->roomtable_[c.stime_.first][c.stime_.second]);
//		
//		//3.���¿εĿα��е�ָ��
//		for (auto i = 0; i < c.GetDuration(); i++) {
//			c.ttbptr_->roomtable_[c.stime_.first][c.stime_.second + i] = &c;
//		}
//
//		//4.���ºϰ�ε�ָ��ָ��
//		for (auto i = 0; i < c.unioncls_.size(); i++) {
//			c.unioncls_[i] = &(clsque_[c.unioclsid_[i]]);
//		}
//	}
//}

void Schedule::Modify() {
	//���г�ͻ�ٽ����ͻ
	for (auto& c : clsque_) {
		if ((!c.CheckPeriod(c.stime_)) || (!c.teacher_->CheckUnit(&c, c.stime_))) {
			//������Ҫ���е���	
			NeedToSwap(c);
		}
	}

}

void Schedule::NeedToSwap(ClassUnit& firstcls) {
	//��ڿ���Ҫ�����е���
	//�Ȼ�ÿ���ʱ��Ľ���
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
	//ʣ�µ�ʱ��ξ��ǿ��Խ��е�����	
	pair<int, int> tmp;
	for (auto i = 0; i < canbeput.size(); i++) {
		//timedelta����0����˵��Ҫ��������ģ�1�����Ҫ�������
		tmp.first = firstcls.stime_.first - canbeput[i].first;
		tmp.second = firstcls.stime_.second - canbeput[i].second;
		//��ʾ�ںϳɹ����ҽ�������
		if (UnionClsUnits(true, firstcls, tmp)) {
			//��ʾ�Ѿ��ɹ�����wait4swap���У�Ȼ��׼���໥֮��Ľ���
			break;
		}
	}	
}

void Schedule::Cross() {
	/*
	�����modify���жϽ�ʦ�Ͽ������
	Ҳ��������ֻ�жϸýڿ��Լ����ܷ񱻰��ŵ������
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
	//0����װ��ǰ�Ľڴ��飬1����װ�����Ľڴ���
	pair<int, int> tmp;
	for (auto i = 0; i < canbeput.size(); i++) {
		//timedelta����0����˵��Ҫ��������ģ�1�����Ҫ�������
		tmp.first = firstcls.stime_.first - canbeput[i].first;
		tmp.second = firstcls.stime_.second - canbeput[i].second;
		//��ʾ�ںϳɹ����ҽ�������
		if (UnionClsUnits(false, firstcls, tmp)) {
			//��ʾ�Ѿ��ɹ�����wait4swap���У�Ȼ��׼���໥֮��Ľ���
			break;
		}
	}
}

bool Schedule::UnionClsUnits(bool flag, ClassUnit& firstcls, pair<int, int> timedelta) {
	//׼������
	//wait4swap����װttb���е�ָ�룬������clsunit�ĵ�ַ
	vector<vector<ClassUnit**>> wait4swap{ 2 };
	//wait4swap[0].push_back(firstcls.ttbptr_->GetClsUnitPtr(firstcls.stime_.first, firstcls.stime_.second));
	vector<vector<ClassUnit**>::iterator> it;
	for (auto i = 0; i < 2; i++) {
		it.push_back(wait4swap[i].begin());
	}
	
	//ͨ��bfs�������н���union����
	set<ClassUnit**> clstab;
	if (!AddUnitPtrIntoVec(flag, firstcls.headptr_, timedelta, 0, wait4swap, clstab))return false;
	//clstab.insert(firstcls.ttbptr_->GetClsUnitPtr(firstcls.stime_.first, firstcls.stime_.second));
	//lambda
	auto fun = [&]()-> bool {
		for (auto i = 0; i < 2; i++) {
			if (it[i] != wait4swap[i].end())
				return true;
		}
		return false;
	};
	//������ʾ�Ƿ�ɹ���ĳ�ڴεĿη�����е��У����������һ��ʧ������ξͶ�ʧ��
	while (fun()) {
		for (auto i = 0; i < 2; i++) {
			if (it[i] != wait4swap[i].end()) {
				if ((*(it[i]) != NULL) && (!AddUnitPtrIntoVec(flag, *(it[i]), timedelta, i, wait4swap, clstab))) {
					return false;
				}
				it[i]++;
			}
		}
	}
	SwapUnitsInVec(wait4swap);
	return true;
}

bool Schedule::AddUnitPtrIntoVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta,
	int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab) {
	//����ǰ����ڴεĿκ�����ڴζ�Ӧ�Ŀζ��ŵ��б���
	auto cls = *(*cptr);//cls����ڿ�
	//�ӵ�ǰ����ڴο�ʼ
	for (auto i = 0; i < cls.GetDuration(); i++) {
		//��ǰ�������
		if(!PutInSetVec(flag, cptr, timedelta, k, i, wait4swap, clstab))return false;
		//if(!PutInSetVec(cptr, timedelta, 1 - k, i, wait4swap, clstab))return false;
	}
	//���ж�����Ƿ��Ǻϰ��
	if (cls.unioclsid_.size()) {
		for (auto& uc : cls.unioncls_) {
			//if (!PutInSetVec(flag, clsque_[uc].headptr_, timedelta, k, 0, wait4swap, clstab))return false;
			if (!PutInSetVec(flag, uc->headptr_, timedelta, k, 0, wait4swap, clstab))return false;
			//if (!PutInSetVec(uc->headptr_, timedelta, 1 - k, 0, wait4swap, clstab))return false;
		}
	}
	return true;
}

bool Schedule::PutInSetVec(bool flag, ClassUnit ** cptr, pair<int, int> timedelta,
	int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab) {
	//�˺�������Ҫ�໥���������ڿζ�һ��Ž�ȥ
	//�˲�����Ҫ��������ƣ������������Ƿ��ܹ����Ź�ȥ
	//������ָ��汾
	auto cls = *cptr;//cls�ǵ�ǰ��ڿ�
	pair<int, int> tmp;
	//�����tmp��ʾ��Ҫ������ȥ��ʱ��
	if (k)tmp = pair<int, int>(cls->stime_.first + timedelta.first, cls->stime_.second + timedelta.second + add);
	else tmp = pair<int, int>(cls->stime_.first - timedelta.first, cls->stime_.second - timedelta.second + add);
	//target��ʾ��Ҫ���������ǽڿΣ������ǿ�ָ��
	auto target = cls->ttbptr_->roomtable_[tmp.first][tmp.second];
	pair<int, int> cperiod{ cls->stime_.first, cls->stime_.second + add };
	if (!CanBeSwap(flag, cls, cperiod, target, tmp))return false;

	//c��ʾҪ�ŵ�vec��set���е�����ڴ�
	auto c = &(cls->ttbptr_->roomtable_[cls->stime_.first][cls->stime_.second + add]);
	//target�����Ǹ���ָ��
	auto tp = &(cls->ttbptr_->roomtable_[tmp.first][tmp.second]);
	if (clstab.find(c) == clstab.end()) {
		clstab.insert(c);
		clstab.insert(tp);
		wait4swap[k].push_back(c);
		wait4swap[1 - k].push_back(tp);
	}
	return true;
}

bool Schedule::CanBeSwap(bool flag, ClassUnit* c, pair<int, int> cperiod, ClassUnit* target, pair<int, int> tperiod) {
	//���������Ҫ������set��vec���е����ڿ��ܷ��໥������
	if (!c->CheckPeriod(tperiod))return false;
	if (flag && !c->teacher_->CheckUnit(c, tperiod))return false;
	if (target != NULL && !(target->CheckPeriod(cperiod)))return false;
	if (flag && target != NULL && !(target->teacher_->CheckUnit(target, cperiod)))return false;
	return true;
}

void Schedule::SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap) {
	//�Ѷ��е��еĽڴζ������໥����
	//ע�⵱�д��ڿ�ָ������
	for (auto i = 0; i < wait4swap.size(); i++) {
		auto origin = *(wait4swap[0][i]);
		auto target = *(wait4swap[1][i]);
		if(origin !=NULL && origin->headptr_ == wait4swap[0][i])
			origin->ttbptr_->SwapUnits(origin, target);
		else if(target !=NULL && target->headptr_ == wait4swap[1][i])
			target->ttbptr_->SwapUnits(origin, target);
		//���ڿ�ָ��
		if(target != NULL)*(wait4swap[0][i]) = &(*target);
		else *(wait4swap[0][i]) = NULL;
		if(origin != NULL)*(wait4swap[1][i]) = &(*origin);
		else *(wait4swap[1][i]) = NULL;
	}
}

