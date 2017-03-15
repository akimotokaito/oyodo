#ifndef _MYFUNC_H_
#define _MYFUNC_H_


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "sqlite3.h"  
#include <sstream> 
#include <ctime>
#include "windows.h"  
//#include <afxinet.h>


using namespace std;


enum WorkStatus // 工作状态规范
{
	WORK = 0,   // 工作状态
	NOTWORK,    // 非工作状态
	DEVELOPMODE // 调试模式
};

enum ResNum { // 返回值规范
	NO = 0, // 表否定，一般不回复或响应
	YES,    // 表肯定，通用，直接回复sendmsg里的内容即可，实际上，0以外的值，只要sendmsg不是空，都是有回复的

	SEND_CERTAINMSG = 10,  //主控QQ控制机器人发消息到指定群


	CANNOT_FIND_GONGLUE = 20, // 找不到指定的攻略
	FIND_GONGLUE = 21, // 存在简单攻略，但不存在详细版本
	FIND_GONGLUE_XX = 22, // 存在详细攻略

	BAN = 30, // 禁言

	STOP_BAOSHI = 96, // 停止报时
	START_BAOSHI = 97, // 开始报时
	STOP_WORK = 98, // 下班
	START_WORK = 99,  // 上班

};


std::vector<std::string> split(std::string str,std::string pattern);
int charge(const char *rcvmsg, string & sendmsg, long & sendint);
int yuanzheng(const char *msg, string & sendmsg);
int gonglue(const char *msg, string & sendmsg);
int getRandomNum(const char *msg, string & sendmsg);
long long sendgroupmsg(const char *msg, string & sendmsg);
int ban(const char *msg, string & sendmsg, long & sendint);
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