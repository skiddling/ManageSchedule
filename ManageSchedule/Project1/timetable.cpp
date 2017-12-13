#include "timetable.h"

void TimeTable::CalFitness(int& crash) {
	/*
	检查每一节课是否存在冲突
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
//	//更新roomtalbe信息记录	
//	//for()
//}
//
//TimeTable & TimeTable::operator=(const TimeTable & t) {
//	
//	// TODO: 在此处插入 return 语句
//}

void TimeTable::Init() {
	//直接按照现有顺序把每节课放到课表当中去
	//具体的节次顺序已经在数据库生成节次的最后对clsque进行了排序
	//具体的顺序可以看classunit的细节
	//for (auto j = 0, k = 0; j < periods_; j++) {
	//	for (auto i = 0; i < days_;) {
	//		//只有当当前节次为空，而且该节课也未被安排才能将当前课安排到当前节次
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
	//			//当所有课都已经被安排完毕
	//			if (k == clsque_.size())return;
	//		}
	//	}
	//}
	//新版本的init
	for (auto j = 0, k = 0; j < periods_; j++) {
		for (auto i = 0; i < days_;) {
			
		}
	}
}

bool TimeTable::PutIntoTable(ClassUnit * cptr) {
	//把这节课放到课表当中去
	//如果尝试了所有空余的节次都无法把这个课放入到合适的时间那么就认为无法为课表初始化，直接失败
	int i = pp, j = pd;
	for (; i < periods_; i++) {
		for (; j < days_; j++) {
			if ((roomtable_[i][j] == nullptr) && cptr->PutIntoTable(j, i, true)) {
				//更新pp和pd
				if (i == pp && j == pd) {
					pp = (i + 1) % periods_;
					pd = (j + 1) % days_;
				}
				////更新表
				//这个操作在unit当中完成
				//for (auto k = 0; k < cptr->duration_; k++) {
				//	roomtable_[i][j + k] = cptr;
				//}
				////更新head指针
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
	//存在空指针的情况
	if(origin != NULL)origin->ChangeTime(target->stime_);
	if(target != NULL)target->ChangeTime(origin->stime_);
}
