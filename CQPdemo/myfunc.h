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


enum WorkStatus // ����״̬�淶
{
	WORK = 0,   // ����״̬
	NOTWORK,    // �ǹ���״̬
	DEVELOPMODE // ����ģʽ
};

enum ResNum { // ����ֵ�淶
	NO = 0, // ��񶨣�һ�㲻�ظ�����Ӧ
	YES,    // ��϶���ͨ�ã�ֱ�ӻظ�sendmsg������ݼ��ɣ�ʵ���ϣ�0�����ֵ��ֻҪsendmsg���ǿգ������лظ���

	SEND_CERTAINMSG = 10,  //����QQ���ƻ����˷���Ϣ��ָ��Ⱥ


	CANNOT_FIND_GONGLUE = 20, // �Ҳ���ָ���Ĺ���
	FIND_GONGLUE = 21, // ���ڼ򵥹��ԣ�����������ϸ�汾
	FIND_GONGLUE_XX = 22, // ������ϸ����

	BAN = 30, // ����

	STOP_BAOSHI = 96, // ֹͣ��ʱ
	START_BAOSHI = 97, // ��ʼ��ʱ
	STOP_WORK = 98, // �°�
	START_WORK = 99,  // �ϰ�

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