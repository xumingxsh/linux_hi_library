#include "test.h"
#include "db/hiDB.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace std::placeholders;
static void test_hidb();

void test_main()
{
	TEST_FUN("HiDB", test_hidb);
}


static void on_db_log(Hi::HiDBLogType tp,const char* err);
static void on_db_log2(Hi::HiDBLogType tp,const char* err);
void test_hidb()
{
	Hi::HiDB db;
	Hi::HiDBLogFun fun = std::bind(on_db_log, _1, _2);
	db.set_log_function(on_db_log);
	const char* conn = "host=127.0.0.1;port=3306;dbname=information_schema;user=root;pwd=root;charset=gbk;";
	cout<<"want open database failed"<<endl;
	try
	{
		if (!db.open(conn))
		{
			cout<<"open database failed,conn="<<conn<<endl;
		}
		
	}
	catch (Hi::HiDBException&)
	{
	}
	
	ASSERT_TEST((!db.is_open(), "db is opened, this is a error"));
	if (db.is_open())
	{	
		cout<<"db is opened, this is a error"<<endl;
	}
	else
	{
		cout<<"db is not opened"<<endl;
	}
		
	db.set_log_function(&on_db_log2);
	conn = "host=127.0.0.1;port=3306;dbname=information_schema;user=root;pwd=root;charset=gbk;";
	cout<<"want open database success"<<endl;
	bool result = false;
	
	try
	{
		result = db.open(conn);			
	}
	catch (Hi::HiDBException&)
	{
	}
	ASSERT_TEST((result, "Hi::HiDB open is error"));
	if (!result)
	{
		cout<<"open database failed,this is a error, conn="<<conn<<endl;
		return;
	}
	else
	{
		cout<<"open database success,conn="<<conn<<endl;
	}
	
	
	try
	{
	
		const char* sql = "select table_name from tables limit 1";
		
		string name = db.execute_scalar(sql);
		ASSERT_TEST((!name.empty(), "Hi::execute_scaler open is error"));
		cout<<"information_schema has a table named "<<name<<endl;	
	}
	catch(Hi::HiDBException&)
	{
	}
	
	
	try
	{
	
		const char* sql = "select table_name, TABLE_TYPE from %s"
			" where TABLE_SCHEMA='%s' order by table_name";
		
		shared_ptr<Hi::HiDBTable> tb 
			= db.execute_query(sql, "tables", "information_schema");
			
		if (tb)
		{
			cout<<"----------"<<endl;
			cout<<"name type"<<endl;
			cout<<"----------"<<endl;
			for (auto it = (*tb).begin(); it != (*tb).end(); ++it)
			{				
				map<string, string>& mp = *it;
				
				auto item = mp.find("table_name");
				auto item2 = mp.find("TABLE_TYPE");
				cout<<item->second<<"	"<<item2->second<<endl;
			}
			cout<<"----------"<<endl;
		}
	}
	catch(Hi::HiDBException&)
	{
	}
	
	db.close();
}

void on_db_log(Hi::HiDBLogType tp,const char* err)
{
	const char* type = "exception";
	if (tp == Hi::HiDLT_SQL)
	{
		type = "sql";
	}
	if (tp == Hi::HiDLT_Normal)
	{
		type = "normal";
	}
	cout<<"["<<type<<"] "<<err<<endl;
}

void on_db_log2(Hi::HiDBLogType tp,const char* err)
{
	const char* type = "exception";
	if (tp == Hi::HiDLT_SQL)
	{
		type = "sql";
	}
	if (tp == Hi::HiDLT_Normal)
	{
		type = "normal";
	}
	cout<<"new log call back: ["<<type<<"] "<<err<<endl;
}