#include "sql.h"


sql::sql(const string&_user, const string & _ip, const string&  _passwd\
    	,const string& _db, const int &_port)
     :user(_user),ip(_ip),passwd(_passwd),db(_db),port(_port)
{
	conn = mysql_init(NULL);
}

int sql::connect()
{
	if (mysql_real_connect(conn, ip.c_str(), user.c_str()\
				, passwd.c_str(), db.c_str(),port,NULL, 0))
	{
		return 0;
	}
	else
		return -1;
}

int sql::getdata_txt()
{
	mysql_query(conn,"set names 'utf8'");
	FILE* fp;
	char strline[1024];
	if ((fp = fopen("1.txt","r")) == NULL)
	{
		cout<<"open file erroe";
		return -1;
	}

	char *data[8];
	while (!feof(fp))
	{
		int i = 0;
		fgets(strline,1024,fp);
		char* start = strline;
		data[i++] = start;
		while (*start)
		{
			if (*start == '\n')
			{
				*start = '\0';
				break;
			}
			if (*start == ' ')
			{
				*start = '\0';
				++start;
				while(*start == ' ')
					++start;
				data[i++] = start;
			}
			else
				++ start;
		}

    	string sql = "INSERT INTO bank (question, answer1, answer2, answer3, answer4, answer, rank) values ('";
    	sql+=data[0];
    	sql+="','";
    	sql+=data[1];
    	sql+="','";
    	sql+=data[2];
    	sql+="','";
    	sql+=data[3];
    	sql+="','";
    	sql+=data[4];
    	sql+="','";
    	sql+=data[5];
	    sql+="',";
    	sql+=data[6];
		sql+=")";
       int ret = mysql_query(conn,sql.c_str());
	   cout<<"插入成功"<<endl;
	}
	return 0;
    
}


int sql::select(string& result)
{
	mysql_query(conn,"set names 'utf8'");
	string sql = "select * from result";
	int ret = mysql_query(conn, sql.c_str());
	if (ret == 0)
	{	
		MYSQL_RES *res = mysql_store_result(conn);
		mysql_data_seek(res, 0);
		MYSQL_ROW row = mysql_fetch_row(res);
		int i = 0;
		for (;i<10;++i)
			result += row[i];
		return 0;
	}
	return -1;
}

void sql::add_result(string& result)
{
	string sql = "INSERT INTO result (result1,result2, result3, result4, result5, result6, result7,\
				  result8,result9,result10) values ('";
	int index = 0;
	for(;index<9;++index)
    {
		sql+=result[index];
        sql+="','";
    }
	sql += result[index];
	sql += "')";

	int ret = mysql_query(conn, sql.c_str());
}

int  sql::select_2(string& result)
{
	mysql_query(conn,"set names 'utf8'");
	srand((unsigned)time(0));
	string sql = "select * from bank";
	int ret = mysql_query(conn, sql.c_str());
	if (ret == 0)
	{
		MYSQL_RES *res = mysql_store_result(conn);
		if (res)
		{
			int lines = mysql_num_rows(res);
			int cols= mysql_num_fields(res);

				cout<<"<html>"<<endl;
                cout<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>"<<endl;
			int i =0;
			int arr[10000] = {0};
			int x = 0;
			for (;i<10 && i< lines;i++)
			{
				do{
			     	 x = rand()%lines;
				}while(arr[x] != 0);
				arr[x] = 1;

				mysql_data_seek(res, x);
				MYSQL_ROW row = mysql_fetch_row(res);
				int j = 0;
				cout<<"<h3>";
				cout<<row[j++];
				cout<<"</h3>"<<endl;
				cout<<"<table border=\"0\">"<<endl;
				int k =0;
			    for (; k< 4;++k)
				{
					cout<<(char)('A'+k)<<"&nbsp"<<row[j++]<<"&nbsp";
				}
				result += row[j++];
				cout<<"&nbsp&nbsp 题目难度：【";
				for (k = 0; k<atoi(row[j]); ++k)
					cout<<"* ";
				cout<<"】"<<endl;
                cout<<"</table>"<<endl;
	           cout<<"<form action=\"result_cgi\" method=\"GET\" accept charset=\"UTF-8\">"<<endl;
			   cout<<"<lable for=\"A\">A</lable>"<<endl;
	           cout<<"<input type=\"radio\" name=\""<<i<<"\" value = \"a\">"<<endl;
			   cout<<"<br/>"<<endl;
			   cout<<"<lable for=\"B\">B</lable>"<<endl;
	           cout<<"<input type=\"radio\" name=\""<<i<<"\" value = \"b\">"<<endl;
			   cout<<"<br/>"<<endl;
			   cout<<"<lable for=\"C\">C</lable>"<<endl;
	           cout<<"<input type=\"radio\" name=\""<<i<<"\" value = \"c\">"<<endl;
			   cout<<"<br/>"<<endl;
			   cout<<"<lable for=\"D\">D</lable>"<<endl;
	           cout<<"<input type=\"radio\" name=\""<<i<<"\" value = \"d\">"<<endl;
			   cout<<"<br/>"<<endl;
			}
			   cout<<"<input type=\"submit\" value=\"提交\" >"<<endl;
	           cout<<"</form>"<<endl;
	    		cout<<"</html>"<<endl;
		}

		return 0;
	}
	return -1;
}

void sql::result_data(string& result,char* buf)
{
	char *argv[11];
	char *start = buf;
	int i = 0;
	while (*start)
	{
		if (*start == '=')
		{
			start ++;
		argv[i++] = start;
		continue;
	}

	if (*start == '&')
	{
		*start = '\0';
	}
	start ++;
	}
	argv[i] == NULL;
	int count = 0;
	cout<<"<html>"<<endl;
	for (int i = 0; i<10; ++i)
	{
		cout<<"<html>"<<endl;
		if ((*argv[i]-result[i]) != 32)
		{
			count ++;
			cout<<"第【"<<i+1<<"】题&nbsp&nbsp"<<"&nbsp&nbsp"<<result[i]<<endl;
			cout<<"<br/>"<<endl;
		}
	}
	cout<<"</html>"<<endl;
}

int sql::Get(string& result)
{
	cout<<"<html>"<<endl;
	cout<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>"<<endl;
	char *method = NULL;
	char *arg_string = NULL;
    char *content_len = NULL;
	char buff[1024];
	method = getenv("METHOD");
	if (method && strcasecmp(method, "GET") == 0)
	{
		arg_string = getenv("QUERY_STRING");
		if (!arg_string)
		{
			return 1;
		}
		strcpy(buff, arg_string);

	}
	else if(method && strcasecmp(method,"POST") == 0)
	{
		content_len = getenv("CONTENT_LENGTH");
		if (!content_len)
			return 2;
		int i = 0;
		char c =0;
		int nums = atoi(content_len);
		for (;i<nums;++i)
		{
			read(0,&c,i);
			buff[i] = c;
		}
		buff[i] = '\0';
	}
	else
	{
		return 1;
	}
	result_data(result,buff);
	cout<<"</html>"<<endl;
	return 0;
}

void sql::clear_res()
{
	mysql_query(conn,"delete from result");
}

void sql::clear_sql()
{
	mysql_query(conn,"delete from bank");
}

sql::~sql()
{
	mysql_close(conn);
}
