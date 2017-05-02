#include "timetable.h"

void TimeTable::Init() {
	//直接按照现有顺序把每节课放到课表当中去
	for (auto i = 0, k = 0; i < periods_; i++) {
		for (auto j = 0; j < days_;) {
			if (roomtable_[j][i] == NULL && clsque_[k].hasbeenput_ == false) {
				clsque_[k].PutIntoTable(j, i);
			}
			else if (roomtable_[j][i] != NULL) {
				j++;
			}
			else {
				k++;
			}
		}
	}
}
