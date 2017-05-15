#include "timetable.h"

void TimeTable::CalFitness(int& crash) {
	/*
	检查每一节课是否存在冲突
	*/
	for (auto& c : clsque_) {
		crash += c->CalFitness();
	}
}

TimeTable::TimeTable(const TimeTable & t):
	clsque_(t.clsque_), days_(t.days_), periods_(t.periods_),
	roomid_(t.roomid_), roomname_(t.roomname_), roomtable_(t.roomtable_){
	//更新roomtalbe信息记录	
	//for()
}

TimeTable & TimeTable::operator=(const TimeTable & t) {
	
	// TODO: 在此处插入 return 语句
}

void TimeTable::Init() {
	//直接按照现有顺序把每节课放到课表当中去
	/*
	1.普通单节课
	2.连堂课
	3.合班课
	4.辅助类型单节课
	*/
	for (auto j = 0, k = 0; j < periods_; j++) {
		for (auto i = 0; i < days_;) {
			//只有当当前节次为空，而且该节课也未被安排才能将当前课安排到当前节次
			if (roomtable_[i][j] == NULL && clsque_[k]->hasbeenput_ == false) {
				clsque_[k]->PutIntoTable(i, j);
			}
			else if (roomtable_[j][i] != NULL) {
				i++;
			}
			else {
				k++;
				//当所有课都已经被安排完毕
				if (k == clsque_.size())return;
			}
		}
	}
}

void TimeTable::UpdatePtrs() {
	/*
	更新所有的从res当中拷贝而来的指针，也就是让所有的指针都进行本地化
	目前唯一已知的就是当前这个节次的时间
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
	//交换这两个课
	//先更新roomtable当中的内容
	origin->ChangeTime(target->stime_);
	target->ChangeTime(origin->stime_);
}
