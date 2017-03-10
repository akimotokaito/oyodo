#ifndef _MYFUNC_H_
#define _MYFUNC_H_


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "sqlite3.h"  
#include <sstream>  
#include "windows.h"  
//#include <afxinet.h>


using namespace std;


enum WorkStatus
{
	WORK = 0,
	NOTWORK,
	DEVELOPMODE
};

std::vector<std::string> split(std::string str,std::string pattern);
int charge(const char *rcvmsg, string & sendmsg);
int yuanzheng(const char *msg, string & sendmsg);
int gonglue(const char *msg, string & sendmsg);
long long sendgroupmsg(const char *msg, string & sendmsg);
int yuanzhenghelp(string & sendmsg);
int gongluehelp(string & sendmsg);
int sql_searchbynum(const char *msg, string & sendmsg);
int sql_searchbycat(const char *msg, string & sendmsg);
wstring Utf8ToUnicode(const string& str);
string UnicodeToUtf8(const wstring& wstr);
string ws2s(const std::wstring& ws);
wstring s2ws(const std::string& s);
bool isNum(string str);
string&   replace_all_distinct(string&   str, const  string&  old_value, const   string&   new_value);
bool isManagerQQ(int64_t fromQQ);
string lltos(long long l);
string ltos( long l);
string itos(int l);

#endif