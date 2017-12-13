#include "timetable.h"

void TimeTable::CalFitness(int& crash) {
	/*
	���ÿһ�ڿ��Ƿ���ڳ�ͻ
	*/
	for (auto& c : clsque_) {
		crash += c->CalFitness();
		//crash += unitset_[c].CalFitness();
	}
}

TimeTable::TimeTable(string roomname, int days, int periods, long long class_id, long long dbid) :
	roomname_(roomname), days_(days), periods_(periods), class_id_(class_id), dbid_(dbid){
	roomtable_ = vector<vector<ClassUnit*>>(periods_, vector<ClassUnit*>(days_, nullptr));
}

//TimeTable::TimeTable(const TimeTable & t):
//	clsque_(t.clsque_), days_(t.days_), periods_(t.periods_),
//	roomid_(t.roomid_), roomname_(t.roomname_), roomtable_(t.roomtable_){
//	//����roomtalbe��Ϣ��¼	
//	//for()
//}
//
//TimeTable & TimeTable::operator=(const TimeTable & t) {
//	
//	// TODO: �ڴ˴����� return ���
//}

void TimeTable::Init() {
	//ֱ�Ӱ�������˳���ÿ�ڿηŵ��α���ȥ
	//����Ľڴ�˳���Ѿ������ݿ����ɽڴε�����clsque����������
	//�����˳����Կ�classunit��ϸ��
	//for (auto j = 0, k = 0; j < periods_; j++) {
	//	for (auto i = 0; i < days_;) {
	//		//ֻ�е���ǰ�ڴ�Ϊ�գ����Ҹýڿ�Ҳδ�����Ų��ܽ���ǰ�ΰ��ŵ���ǰ�ڴ�
	//		if (roomtable_[i][j] == NULL && clsque_[k]->hasbeenput_ == false) {
	//		//if (roomtable_[i][j] == NULL && unitset_[clsque_[k]].hasbeenput_ == false) {
	//			clsque_[k]->PutIntoTable(i, j);
	//			//unitset_[clsque_[k]].PutIntoTable(i, j);
	//		}
	//		else if (roomtable_[j][i] != NULL) {
	//			i++;
	//		}
	//		else {
	//			k++;
	//			//�����пζ��Ѿ����������
	//			if (k == clsque_.size())return;
	//		}
	//	}
	//}
	//�°汾��init
	for (auto j = 0, k = 0; j < periods_; j++) {
		for (auto i = 0; i < days_;) {
			
		}
	}
}

bool TimeTable::PutIntoTable(ClassUnit * cptr) {
	//����ڿηŵ��α���ȥ
	//������������п���Ľڴζ��޷�������η��뵽���ʵ�ʱ����ô����Ϊ�޷�Ϊ�α��ʼ����ֱ��ʧ��
	int i = pp, j = pd;
	for (; i < periods_; i++) {
		for (; j < days_; j++) {
			if ((roomtable_[i][j] == nullptr) && cptr->PutIntoTable(j, i, true)) {
				//����pp��pd
				if (i == pp && j == pd) {
					pp = (i + 1) % periods_;
					pd = (j + 1) % days_;
				}
				////���±�
				//���������unit�������
				//for (auto k = 0; k < cptr->duration_; k++) {
				//	roomtable_[i][j + k] = cptr;
				//}
				////����headָ��
				//if (cptr->duration_ > 1) {
				//	cptr->headptr_ = &roomtable_[i][j];
				//}
				return true;
			}
		}
	}
	return false;
}

void TimeTable::UpdatePtrs() {
	/*
	�������еĴ�res���п���������ָ�룬Ҳ���������е�ָ�붼���б��ػ�
	ĿǰΨһ��֪�ľ��ǵ�ǰ����ڴε�ʱ��
	*/
	for (auto& c : clsque_) {
		c->ttbptr_ = this;
	}
	for (auto i = 0; i < clsque_.size(); i++) {
		auto day = clsque_[i]->stime_.first, period = clsque_[i]->stime_.second;
		roomtable_[day][period] = clsque_[i];
	}
}

ClassUnit** TimeTable::GetClsUnitPtr(int day, int period) {
	return &roomtable_[day][period];
}

void TimeTable::SwapUnits(ClassUnit * origin, ClassUnit * target) {
	//������������
	//�ȸ���roomtable���е�����
	//���ڿ�ָ������
	if(origin != NULL)origin->ChangeTime(target->stime_);
	if(target != NULL)target->ChangeTime(origin->stime_);
}
