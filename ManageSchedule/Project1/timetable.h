#pragma once
#include "classunit.h"

//�༶�α�,��ʵ����һ���εļ���
class TimeTable
{
public:
	int days_, periods_;//һ��������ÿ��ڴ���
	int roomid_;//����༶��id
	string roomname_;//������ҵ�����
	vector<vector<ClassUnit*>> roomtable_;//���ҿα�
	vector<ClassUnit> clsque_;//�ڴ�����
	//vector<vector<bool>> tabletag_;//������ʱ�ڶ��Ƿ��ܱ����ŷſΣ���ʼ���α���
	void Init();
private:
	

};