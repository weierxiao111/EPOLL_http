#ifndef _SQL_H_
#define _SQL_H_

#include <iostream>
#include <mysql.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
using namespace std;

class sql{
public:
	sql(const string& _user, const string &_ip,\
	   const string &_passwd, const string &_db, const int& _port);
	int connect();
	int insert(const string &problem, const string &answer1, const string& answer2,\ 
			const string& answer3, const string& answer4, const string& answer,
			const int& rank);
	int select(string& result);
	int Get(string& result);
	int getdata_txt();
	int select_2(string& result);
	void clear_sql();
	void clear_res();
	void result_data(string& result, char* buf);
    void add_result(string& result);
	~sql();
private:
	MYSQL *conn;
	string user;
    string ip;
	string passwd;
	string db;
	int port;
};
#endif  
