#include "timetable.h"

void TimeTable::CalFitness(int& crash) {
	/*
	���ÿһ�ڿ��Ƿ���ڳ�ͻ
	*/
	for (auto& c : clsque_) {
		crash += c->CalFitness();
	}
}

TimeTable::TimeTable(const TimeTable & t):
	clsque_(t.clsque_), days_(t.days_), periods_(t.periods_),
	roomid_(t.roomid_), roomname_(t.roomname_), roomtable_(t.roomtable_){
	//����roomtalbe��Ϣ��¼	
	//for()
}

TimeTable & TimeTable::operator=(const TimeTable & t) {
	
	// TODO: �ڴ˴����� return ���
}

void TimeTable::Init() {
	//ֱ�Ӱ�������˳���ÿ�ڿηŵ��α���ȥ
	/*
	1.��ͨ���ڿ�
	2.���ÿ�
	3.�ϰ��
	4.�������͵��ڿ�
	*/
	for (auto j = 0, k = 0; j < periods_; j++) {
		for (auto i = 0; i < days_;) {
			//ֻ�е���ǰ�ڴ�Ϊ�գ����Ҹýڿ�Ҳδ�����Ų��ܽ���ǰ�ΰ��ŵ���ǰ�ڴ�
			if (roomtable_[i][j] == NULL && clsque_[k]->hasbeenput_ == false) {
				clsque_[k]->PutIntoTable(i, j);
			}
			else if (roomtable_[j][i] != NULL) {
				i++;
			}
			else {
				k++;
				//�����пζ��Ѿ����������
				if (k == clsque_.size())return;
			}
		}
	}
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
	origin->ChangeTime(target->stime_);
	target->ChangeTime(origin->stime_);
}
