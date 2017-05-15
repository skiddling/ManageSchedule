#pragma once
#include "classunit.h"

//�༶�α�,��ʵ����һ���εļ���
class TimeTable
{
public:
	
	int days_, periods_;//һ��������ÿ��ڴ���
	int roomid_;//����༶��id��Ҳ����vector<timetalbe>���е����
	string roomname_;//������ҵ�����
	vector<vector<ClassUnit*>> roomtable_;//���ҿα�ָ��ֻ�뵱ǰ�Լ�clsque���е�����
	vector<ClassUnit*> clsque_;//�ڴ�����
	//vector<vector<bool>> tabletag_;//������ʱ�ڶ��Ƿ��ܱ����ŷſΣ���ʼ���α���
	void CalFitness(int& crash);

	TimeTable(const TimeTable& t);
	TimeTable& operator=(const TimeTable& t);
	void Init();
	void UpdatePtrs();
	ClassUnit** GetClsUnitPtr(int day, int period);

	void SwapUnits(ClassUnit* origin, ClassUnit* target);
private:
	

};