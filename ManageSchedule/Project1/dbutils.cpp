#include "dbutils.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

void Dbutils::GetDBData(char* argv[], map<string, int>& teachersmap, map<string, int>& coursesmap, vector<Teacher>& teachers, vector<Course>& courses, vector<TimeTable>& timetables) {
	GetDBInfo(argv);
	GetPKTaskInfo();
}

void Dbutils::GetDBInfo(char* argv[]) {
	task_id_ = string(argv[1]);
	fstream fin("dbinfo.txt");
	fin >> dbname_ >> dbuser_name_ >> dbuser_pwd_;
	cout << dbname_ << " " << dbuser_name_ << " " << dbuser_pwd_ << endl;
	fin.close();
}

void Dbutils::GetPKTaskInfo() {
	cout << "�������ݿ⣬��SQL Server  ����PKTaskInfo��Ϣ" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr����ָ��ʵ����ʧ�ܣ�����" << endl;
		return;
	}
	else {
		try {
			//���ط����� ��PKTaskInfo���ݿ� ��¼��Ϊabc, ����Ϊ123
			string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
				+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";
			//_bstr_t strConnect = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=paikezhuti;uid=test;pwd=123;";
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << (char*)e.Description();
		}
		_RecordsetPtr m_pRecordset; //��¼������ָ�룬����ִ��SQL��䲢��¼��ѯ���
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "��¼������ָ��ʵ����ʧ�ܣ�" << endl;
			return;
		}
		try {
			string s = "SELECT * FROM T_PKTask where id=" + task_id_;
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���		
		}
		catch (_com_error &e) {
			cerr << (char*)e.Description();
		}
		try {
			int dpw, ppd, pim, pnum;
			m_pRecordset->MoveFirst();
			dpw = (int)m_pRecordset->Fields->GetItem(_variant_t("dayNum"))->Value;
			pim = (int)m_pRecordset->Fields->GetItem(_variant_t("amNum"))->Value;
			pnum = (int)m_pRecordset->Fields->GetItem(_variant_t("pmNum"))->Value;
			ppd = pim + pnum;
			cout << dpw << "   " << ppd << "  " << pim << endl;
		}
		catch (_com_error &e) {
			cerr << (char*)e.Description();
		}
	}
}