#include "dbutils.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

void Dbutils::GetDBData(char* argv[], map<string, int>& teachersmap, map<string, int>& coursesmap, vector<Teacher>& teachers, vector<Course>& courses, vector<TimeTable>& timetables) {
	GetDBInfo(argv);
	GetPKTaskInfo();
	GetPKCourse(coursesmap, courses);
	GetClassCourseLessonConfigInfo(teachersmap, coursesmap, teachers, courses, timetables);
}

void Dbutils::GetDBInfo(char* argv[]) {
	//task_id_ = string(argv[1]);
	fstream fin("dbinfo.txt");
	fin >> dbname_ >> dbuser_name_ >> dbuser_pwd_;
	cout << dbname_ << " " << dbuser_name_ << " " << dbuser_pwd_ << endl;
	fin.close();
}

void Dbutils::GetPKTaskInfo() {
	cout << "连接数据库，从SQL Server  读入PKTaskInfo信息" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr对象指针实例化失败！！！" << endl;
		return;
	}
	else {
		try {
			//本地服务器 打开PKTaskInfo数据库 登录名为abc, 密码为123
			string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
				+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "记录集对象指针实例化失败！" << endl;
			return;
		}
		try {
			//string s = "SELECT * FROM T_PKTask where id=" + task_id_;
			string s = "SELECT * FROM T_PKTask where id=2";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句		
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		try {
			int dpw, ppd, pim, pnum;
			m_pRecordset->MoveFirst();
			dpw = (int)m_pRecordset->Fields->GetItem(_variant_t("dayNum"))->Value;
			pim = (int)m_pRecordset->Fields->GetItem(_variant_t("amNum"))->Value;
			pnum = (int)m_pRecordset->Fields->GetItem(_variant_t("pmNum"))->Value;
			ppd = pim + pnum;
			cout << dpw << "   " << ppd << "  " << pim << endl;
			TimeTable::days_per_week_ = dpw;
			TimeTable::period_in_moring_ = pim;
			TimeTable::period_per_day_ = ppd;
			ClassUnit::pim = pim;
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
	}
}

void Dbutils::GetPKCourse(map<string, int> &coursesmap, vector<Course> &courses) {
	cout << "连接数据库，从SQL Server  读入PKCourse信息" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr对象指针实例化失败！！！" << endl;
		return;
	}
	else {
		try {
			//本地服务器 打开PKTaskInfo数据库 登录名为abc, 密码为123
			string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
				+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "记录集对象指针实例化失败！" << endl;
			return;
		}
		try {
			//string s = "SELECT * FROM T_PKTask where id=" + task_id_;
			string s = "SELECT * FROM T_PKCourse";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句		
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		try {
			_variant_t var;
			string coursename;
			int i = 0;
			m_pRecordset->MoveFirst();
			while (!m_pRecordset->adoEOF) {
				var = m_pRecordset->Fields->GetItem(_variant_t("name"))->Value;
				coursename = (const char*)_bstr_t(var);
				coursesmap[coursename] = i;
				courses.push_back(*(new Course(i++, coursename)));
				m_pRecordset->MoveNext();
			}
			for (i = 0; i < courses.size(); i++) {
				cout << courses[i].course_id_ << "  " << courses[i].course_name_ << endl;
			}
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
	}
}

void Dbutils::GetClassCourseLessonConfigInfo(map<string, int>& teachersmap, map<string, int>& coursesmap, vector<Teacher>& teachers, vector<Course>& courses, vector<TimeTable>& timetables) {
	cout << "连接数据库，从SQL Server  读入ClassCourseLessonConfigInfo信息" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr对象指针实例化失败！！！" << endl;
		return;
	}
	else {
		try {
			//本地服务器 打开PKTaskInfo数据库 登录名为abc, 密码为123
			string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
				+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "记录集对象指针实例化失败！" << endl;
			return;
		}
		try {
			//string s = "SELECT * FROM T_PKTask where id=" + task_id_;
			string s = "SELECT * FROM T_ClassCourseLessonConfig";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句		
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		try {
			/*coursesmap.clear();
			teachersmap.clear();*/
			_variant_t var;
			string classname, teachername, coursename;
			map<string, int> cname2id;
			int i = 0, clid, coid, courselenth, dbid;
			//vector<vector<ClassUnit *> > classunits;
			vector<ClassUnit *> tempque;
			m_pRecordset->MoveFirst();
			while (!m_pRecordset->adoEOF) {
				var = m_pRecordset->Fields->GetItem(_variant_t("id"))->Value;
				dbid = static_cast<int>(var);

				var = m_pRecordset->Fields->GetItem(_variant_t("className"))->Value;
				classname = (const char*)_bstr_t(var);
				if (cname2id.find(classname) == cname2id.end()) {
					int csize = cname2id.size();
					cname2id[classname] = csize;
					classunits.push_back(tempque);
				}
				clid = cname2id[classname];

				var = m_pRecordset->Fields->GetItem(_variant_t("ncourseName"))->Value;
				coursename = (const char*)_bstr_t(var);
				coid = coursesmap[coursename];

				var = m_pRecordset->Fields->GetItem(_variant_t("teacherName"))->Value;
				teachername = (const char*)_bstr_t(var);
				if (teachersmap.find(teachername) == teachersmap.end()) {
					int tid = teachersmap.size();
					//cout << tid << endl;
					//teachersmap[teachername] = teachersmap.size();
					teachersmap[teachername] = tid;
					Teacher *tea = new Teacher(teachersmap[teachername], teachername);
					teachers.push_back(*tea);
					courses[coid].teacher_queue_.push_back(*tea);
				}
				
				var = m_pRecordset->Fields->GetItem(_variant_t("lessonNum"))->Value;
				courselenth = static_cast<int>(var);
				//cout << teachersmap[teachername] << endl;
				teachers[teachersmap[teachername]].class_que_[clid] = vector<int>(courselenth);
				for (int k = 0; k < courselenth; k++) {
					ClassUnit *clsu = new ClassUnit(teachers[teachersmap[teachername]], clid, classname, courses[coid].course_name_, courses[coid].course_id_, dbid);
					//cout << teachersmap[teachername] << endl;
					teachers[teachersmap[teachername]].class_que_[clid][k] = classunits[clid].size();
					clsu->unit_id_ = classunits[clid].size();
					classunits[clid].push_back(clsu);
				}
				//cout << "hello world" << endl;
				m_pRecordset->MoveNext();
			}
			Schedule::teachers_map_ = teachersmap;
			Schedule::courses_map_ = coursesmap;
			TimeTable::courses_map_ = coursesmap;
			int classnum = cname2id.size();
			timetables = vector<TimeTable>(classnum);
			for (int i = 0; i < classnum; i++) {
				vector<ClassUnit *>::iterator itc = classunits[i].begin();
				for (; itc != classunits[i].end(); itc++) {
					timetables[(*itc)->class_id_].class_que_.push_back(*(*itc));
				}
			}
			Out(teachersmap, coursesmap, teachers, courses, timetables);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
	}
}

void Dbutils::Out(map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable>& timetables) {
	ofstream fout("out.txt");
	fout << "输出老师的个数和老师各自的序号" << endl;
	vector<Teacher> ::iterator itt = teachers.begin();
	for (; itt != teachers.end(); itt++) {
		fout << itt->id_ << ' ' << itt->teacher_name_ << "\n";
	}
	fout << endl << endl;
	fout << "输出每个科目的下有哪些老师" << endl;
	vector<Course> ::iterator itc = courses.begin();
	for (; itc != courses.end(); itc++) {
		fout << itc->course_id_ << ' ' << itc->course_name_ << "\n";
		itt = itc->teacher_queue_.begin();
		for (; itt != itc->teacher_queue_.end(); itt++) {
			fout << itt->id_ << ' ' << itt->teacher_name_ << "\n";
		}
		fout << endl;
	}
	fout << endl << endl;
	fout << "输出所有的班级当中所有的课程数量" << endl;
	/*vector<ClassUnit *> :: iterator itu = classunits.begin();
	for (; itu != classunits.end(); itu++) {
	fout << (*itu)->class_id_ << ' ' << (*itu)->class_name_ << ' ' << (*itu)->teacher_.id_ << ' ' << (*itu)->teacher_.teacher_name_ << "\n";
	}*/
	for (int i = 0; i < classunits.size(); i++) {
		vector<ClassUnit *>::iterator itu = classunits[i].begin();
		for (; itu != classunits[i].end(); itu++) {
			fout << (*itu)->class_id_ << ' ' << (*itu)->class_name_ << ' ' << (*itu)->teacher_.id_ << ' ' << (*itu)->teacher_.teacher_name_ << "\n";
		}
	}
	fout << endl << endl;
	fout << "输出每个班级当中有哪些老师上课，上多少次课" << endl;
	for (int i = 0; i < timetables.size(); i++) {
		fout << "class " << i << "\n";
		for (int j = 0; j < timetables[i].class_que_.size(); j++) {
			fout << timetables[i].class_que_[j].class_id_ << ' '
				<< timetables[i].class_que_[j].class_name_ << ' '
				<< timetables[i].class_que_[j].teacher_.teacher_name_ << "\n";
		}
		fout << endl;
	}
	fout.close();
}

void Dbutils::OutPutPKTeaching(Schedule res) {
	cout << "连接数据库，从SQL Server  输出PKTeaching信息" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr对象指针实例化失败！！！" << endl;
		return;
	}
	else {
		try {
			//本地服务器 打开PKTaskInfo数据库 登录名为abc, 密码为123
			string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
				+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "记录集对象指针实例化失败！" << endl;
			return;
		}
		try {
			//string s = "SELECT * FROM T_PKTask where id=" + task_id_;
			string s = "SELECT * FROM T_PKTeaching";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句		
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		try {
			_variant_t var;
			for (int i = 0; i < res.time_tables_.size(); i++) {
				for (int x = 0; x < TimeTable::days_per_week_; x++) {
					for (int y = 0; y < TimeTable::period_per_day_; y++) {
						if (res.time_tables_[i].table_[x][y] != NULL) {
							m_pRecordset->AddNew();
							var = static_cast<_variant_t>(x + 1);
							m_pRecordset->PutCollect("weekday", var);
							var = static_cast<_variant_t>(y + 1);
							m_pRecordset->PutCollect("section", var);
							var = static_cast<_variant_t>(res.time_tables_[i].table_[x][y]->dbid_);
							m_pRecordset->PutCollect("classCourseLessonConfig", var);
						}
					}
				}
			}
			m_pRecordset->Update();
			m_pRecordset->Close();
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
	}
}
