#include "myfunc.h"

using namespace std;
sqlite3 *pDB;

#define MY_QQNUM 526975248
#define MY_GRPNUM 45694267
#define TEST_QQNUM 2043128624
#define TEST_GRPNUM 237712764
#define KANC_GRPNUM 531550460

//�ַ����ָ��
vector<string> split(string str,string pattern)
{
  string::size_type pos;
  vector<std::string> result;
  str+=pattern;//��չ�ַ����Է������
  int size=str.size();
 
  for(int i=0; i<size; i++)
  {
    pos=str.find(pattern,i);
    if(pos<size)
    {
      string s=str.substr(i,pos-i);
      result.push_back(s);
      i=pos+pattern.size()-1;
    }
  }
  return result;
}

//�ַ�����������
int charge(const char *rcvmsg, string & sendmsg)
{
	int ret = 0;
	string tmpstr = rcvmsg;

	while (!tmpstr.empty() && tmpstr[0] == ' ')
	{
		tmpstr.erase(0, 1);
	}
	while (!tmpstr.empty() && tmpstr[tmpstr.size()-1] == ' ')
	{
		tmpstr.erase(tmpstr.size()-1, 1);
	}

	if (strstr(tmpstr.c_str(), "���ϰ�") != NULL  && tmpstr.size() <= 12) {
		return 99;
	}
	if (strstr(tmpstr.c_str(), "���°�") != NULL  && tmpstr.size() <= 12) {
		return 98;
	}
	if (strstr(tmpstr.c_str(),"[CQ:at,qq=2469931868]") != NULL) {
		tmpstr = replace_all_distinct(tmpstr, "[CQ:at,qq=2469931868]", " ");
		return charge(tmpstr.c_str(), sendmsg);
	}
	if (strstr(tmpstr.c_str(),"Զ��") != NULL)
	{
		//sql_search(rcvmsg, sendmsg);
		if (tmpstr.size() > 100) {
			return 0;
		}
		if(strstr(tmpstr.c_str(),"Զ��") != tmpstr){
			sendmsg = "�ᶽ�����˽�Զ����ص��µĻ������롰Զ�����һ���ϸ�������ģ�";
			ret = 1;
		}else{
			ret = yuanzheng(tmpstr.c_str(), sendmsg);; //Զ��
		}
	}else if (strstr(tmpstr.c_str(),"����") != NULL)
	{
		if (tmpstr.size() > 100) {
			return 0;
		}
		if(strstr(tmpstr.c_str(),"����") != tmpstr){
			sendmsg = "�ᶽ�����˽⹥����ص��µĻ������롰���ԡ��һ���ϸ�������ģ�";
			ret = 2;
		}else{
			ret = gonglue(tmpstr.c_str(), sendmsg);; //����
		}
	}else if (strstr(tmpstr.c_str(),"��Ⱥ") != NULL)
	{
		//sendgroupmsg(rcvmsg, sendmsg);
		ret = 10; //����QQ���ƻ����˷���Ϣ��ָ��Ⱥ
	}else
	{
		return ret;
	}
	//vector<string> rcvmsg = split(msg, " ");

	return ret;
}

//���ض�Ⱥ����ָ����Ϣ
long long sendgroupmsg(const char *msg, string & sendmsg)
{
	string tempmsg;
	vector<string> rcvmsg;
	long long ret = 0;
	//���ֶΡ�����Ϣ����ʼ���ַ�����ȡ����sendmsg
	tempmsg = strstr(msg,"��Ⱥ");
	tempmsg.erase(0,4);

	rcvmsg = split(tempmsg, " ");
	if (rcvmsg.size() > 1)
	{
		/*if (rcvmsg[0] == "")
		{		
			ret = atoi(rcvmsg[1].c_str());
			sendmsg = rcvmsg[2];
		}else{
			ret = atoi(rcvmsg[0].c_str());
			sendmsg = rcvmsg[1];
		}*/
		if (rcvmsg[0] == "")
		{
			ret = atoi(rcvmsg[1].c_str());
			for (int i = 2; i < rcvmsg.size(); i++) {
				sendmsg += rcvmsg[i];
				sendmsg += ' ';
			}
		}
		else {
			ret = atoi(rcvmsg[0].c_str());
			for (int i = 1; i < rcvmsg.size(); i++) {
				sendmsg += rcvmsg[i];
				sendmsg += ' ';
			}
		}


	}
	return ret;
}

// ����Զ���ĺ���
int yuanzheng(const char *msg, string & sendmsg)
{
	string tempmsg;
	vector<string> rcvmsg;
	vector<string>::iterator sit;
	int i = 0;
	int ret = 0;

	//���ֶΡ�Զ������ʼ���ַ�����ȡ����sendmsg
	tempmsg = strstr(msg,"Զ��");
	tempmsg.erase(0,4);

	rcvmsg = split(tempmsg, " ");

	//�������ѭ����vector����������Ϊ�����͡� ������ȫ��ɾ��
	for (i=0,sit = rcvmsg.begin(); sit != rcvmsg.end();)
	{
		if (rcvmsg[i] == ""|| rcvmsg[i] == " ")
		{
			sit = rcvmsg.erase(sit);
		}else{
			++sit;
			++i;
		}
	}

	switch(rcvmsg.size())
	{
		case 0:
			ret = yuanzhenghelp(sendmsg);
			break;
		case 1:
			//ֻ��1�����������
			//sendmsg = "ֻ��1�����������"+rcvmsg[0];
			//����Ϊ''
			if (isNum(rcvmsg[0])) //���ֻ��һ��������������
			{
				int num = 0;
				num = atoi(rcvmsg[0].c_str());
				if (0 < num && num <= 40)
				{
					string inputstr = " where NUM = ";
					inputstr += rcvmsg[0];
					ret = sql_searchbynum(inputstr.c_str(), sendmsg);
				}else{ //��������ֲ���ĿǰԶ����ŷ�Χ�ڵĻ�
					sendmsg = "�ᶽ������������ȷ��Զ����ţ�(Զ��1-40)";
					ret = 1;
				}
			}else if (rcvmsg[0] == "��") //��ѯȼ��Զ��
			{
				string inputstr = "GETY";// where GETY > 500 and GETY != ''";
				sendmsg = "����������Զ����\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "��") //��ѯ��ҩԶ��
			{
				string inputstr = "GETD";
				sendmsg = "����������Զ����\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "��") //��ѯ�ֲ�Զ��
			{
				string inputstr = "GETG";
				sendmsg = "����������Զ����\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "��") //��ѯ����Զ��
			{
				string inputstr = "GETL";
				sendmsg = "����������Զ����\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "����" || rcvmsg[0] == "help" || rcvmsg[0] == "?" || rcvmsg[0] == "��")
			{
				ret = yuanzhenghelp(sendmsg);
			}
			break;
		case 2:
			//ֻ��2�����������
			//sendmsg = "ֻ��2�����������"+rcvmsg[0]+"\\"+rcvmsg[1];
			break;
		default:
			//�������
			//sendmsg = "����Ĳ���̫�࣬Ŀǰ���ֻ������2������";
			break;
	}
	return ret;
}

// ���ļ����ݶ�ȡ��sendmsg��
int readfiletomsg(char * FileName, string & sendmsg)
{
	ifstream File;
	//char * FileName = "F:\\software\\kuq\\��Q Air\\app\\com.steve.oyodo\\yuanzheng\\help.txt";
	char databuf[256];

	File.open(FileName);
	if(!File){
		cerr<<"error oprening file myname!"<<endl;
		//exit(-1);
		return 0;//���ļ�ʧ�ܷ�����0
	}else{
		while(!File.eof()){
			//File.read(databuf,128);
			File.getline(databuf,256);
			sendmsg += databuf;
			sendmsg += "\n";
		}
		if (sendmsg.empty() == false)
		{
			sendmsg.erase(sendmsg.size()-1);
		}
		File.close();
		return 1;//���ļ��ɹ�����1
	}

}

// Զ������
int yuanzhenghelp(string & sendmsg)
{
	//char * FileName = "F:\\software\\kuq\\��Q Air\\app\\com.steve.oyodo\\yuanzheng\\help.txt";
	char * FileName = "app\\com.steve.oyodo\\yuanzheng\\help.txt";
	return readfiletomsg(FileName,sendmsg);
}

// ���԰���
int gongluehelp(string & sendmsg)
{
	//char * FileName = "F:\\software\\kuq\\��Q Air\\app\\com.steve.oyodo\\gonglue\\help.txt";
	char * FileName = "app\\com.steve.oyodo\\gonglue\\help.txt";
	return readfiletomsg(FileName,sendmsg);
}

// ���ƹ��Եĺ���
int gonglue(const char *msg, string & sendmsg)
{
	int ret = 0;
	string tempmsg;
	vector<string> rcvmsg;
	vector<string>::iterator sit;
	int i = 0;
	//char * FilePath = "F:\\software\\kuq\\��Q Air\\app\\com.steve.oyodo\\gonglue\\";
	char * FilePath = "app\\com.steve.oyodo\\gonglue\\";

	//���ֶΡ����ԡ���ʼ���ַ�����ȡ����sendmsg
	tempmsg = strstr(msg,"����");
	tempmsg.erase(0,4);

	rcvmsg = split(tempmsg, " ");
	//�������ѭ����vector����������Ϊ�����͡� ������ȫ��ɾ��
	for (i=0,sit = rcvmsg.begin(); sit != rcvmsg.end();)
	{
		if (rcvmsg[i] == ""|| rcvmsg[i] == " ")
		{
			sit = rcvmsg.erase(sit);
		}else{
			++sit;
			++i;
		}
	}

	switch(rcvmsg.size()){
	case 0:
		ret = gongluehelp(sendmsg);;
		break;
	case 1:{	
		if (rcvmsg[0] == "����" || rcvmsg[0] == "help" || rcvmsg[0] == "?" || rcvmsg[0] == "��")
		{
			ret = gongluehelp(sendmsg);
			break;
		}
		char FileName[128] ={0};
		sprintf(FileName,"%s%s%s",FilePath,rcvmsg[0].c_str(),".txt");
		readfiletomsg(FileName, sendmsg);
		ret = 1;
		break;
	}
	case 2:{
		if (rcvmsg[1] == "��" || rcvmsg[1] == "��ϸ")
		{
			char FileName[128] ={0};
			char FileName2[128] ={0};
			sprintf(FileName,"%s%s%s", FilePath, rcvmsg[0].c_str(),"x.txt");
			sprintf(FileName2,"%s%s%s", FilePath, rcvmsg[0].c_str(),".txt");
			if (readfiletomsg(FileName, sendmsg) == 1)  //������ϸ���Ե������
			{
				ret = 21;
			}else if(readfiletomsg(FileName2, sendmsg) == 1){ //��������ϸ�������ڼ��Թ���
				ret = 22;
			}else{ //��������
				ret = 20;
			}
		}	
		break;
	}
	default:
		//gongluehelp(sendmsg);
		break;
	}

	return ret;
} 

// ������Դ���Ͳ���Զ�����
int sql_searchbycat(const char *msg, string & sendmsg)
{
	int ret = 0;
	string strSql = "select NUM,USETIME,";
	//from yuanzheng ";
	//string strSql = "select NUM, NAME from yuanzheng ";
	char ** pResult;
	char * errmsg;
	string spaceforcmp = UnicodeToUtf8(s2ws(""));
	int nRow;   
	int nCol;

	//string filename = "F:\\software\\kuq\\��Q Air\\app\\com.steve.oyodo\\yuanzheng\\kanyuanzheng.db";
	string filename = "app\\com.steve.oyodo\\yuanzheng\\kanyuanzheng.db";
	wstring wsfn = s2ws(filename);
	sqlite3_open(UnicodeToUtf8(wsfn).c_str(), &pDB);

	strSql += msg;
	strSql += " from yuanzheng where ";
	strSql += msg;
	strSql += " > 1 and ";
	strSql += msg;
	strSql += " != '' order by ";
	strSql += msg;
	strSql += " desc limit 0, 3";
	int nResult = sqlite3_get_table(pDB,strSql.c_str(),&pResult,&nRow,&nCol,&errmsg); 
	if (nResult != SQLITE_OK)  
	{  
		sqlite3_close(pDB);  
		//cout << errmsg << endl;  
		sendmsg += errmsg;
		sqlite3_free(errmsg);  
		return ret;  
	}
	//sendmsg += "SQLITE_OK\n";
	string strOut;  
	int nIndex = nCol;  
	for(int i=0;i<nRow;i++)  
	{  
		strOut += UnicodeToUtf8(s2ws("Զ��"));
		strOut += pResult[nIndex*i+3]; //���		
		strOut += UnicodeToUtf8(s2ws(" ��ʱ��"));
		strOut += pResult[nIndex*i+4]; //��ʱ
		strOut += UnicodeToUtf8(s2ws("���� ��Դ"));
		strOut += pResult[nIndex*i+5]; //����
		strOut += "\n";
	} 
	if (nRow > 0)
	{
		strOut.erase(strOut.size()-1);
	}
	sqlite3_free_table(pResult);  //ʹ���������ͷ�Ϊ��¼������ڴ棬������ڴ�й©
	//cout<<strOut<<endl;
	sendmsg += ws2s(Utf8ToUnicode(strOut));
	//sendmsg = strOut;
	sqlite3_close(pDB);  
	ret = nRow;
	return ret; 
}

// ����Զ����Ų��ҽ��
int sql_searchbynum(const char *msg, string & sendmsg)
{
	int ret = 0;
	string strSql = "select * from yuanzheng ";
	//string strSql = "select NUM, NAME from yuanzheng ";
	char ** pResult;
	char * errmsg;
	string spaceforcmp = UnicodeToUtf8(s2ws(""));
	int nRow;   
	int nCol;

	//sendmsg += "before open\n";
	//string filename = "F:\\software\\kuq\\��Q Air\\app\\com.steve.oyodo\\yuanzheng\\kanyuanzheng.db";
	string filename = "app\\com.steve.oyodo\\yuanzheng\\kanyuanzheng.db";
	wstring wsfn = s2ws(filename);
	sqlite3_open(UnicodeToUtf8(wsfn).c_str(), &pDB);
	//sqlite3_open("F:\\kanyuanzheng.db", &pDB);
	//sendmsg += "open succes\n";

	strSql += msg;
	int nResult = sqlite3_get_table(pDB,strSql.c_str(),&pResult,&nRow,&nCol,&errmsg); 
	if (nResult != SQLITE_OK)  
	{  
		sqlite3_close(pDB);  
		//cout << errmsg << endl;  
		sendmsg += errmsg;
		sqlite3_free(errmsg);  
		return ret;  
	}
	//sendmsg += "SQLITE_OK\n";
	string strOut;// = "starout=\n";  
	int nIndex = nCol; 
	// ����
	for(int i=0;i<nRow;i++)  
	{  
		strOut += UnicodeToUtf8(s2ws("Զ��"));
		strOut += pResult[nIndex]; //���
		strOut += " ";
		strOut += pResult[nIndex+1]; //����
		strOut += UnicodeToUtf8(s2ws("\n��ʱ��"));
		strOut += pResult[nIndex+3]; //��ʱ
		strOut += UnicodeToUtf8(s2ws("����\n���ģ���"));
		if (pResult[nIndex+4] != NULL && pResult[nIndex+4] != spaceforcmp) //����
		{
			strOut += pResult[nIndex+4];

		}else{
			strOut += "-";
		}
		strOut += UnicodeToUtf8(s2ws("/��"));
		if (pResult[nIndex+5] != NULL && pResult[nIndex+5] != spaceforcmp) //�ĵ�
		{
			strOut += pResult[nIndex+5];
		}else{
			strOut += "-";
		}
		strOut += UnicodeToUtf8(s2ws("\n�콢�ȼ���")); //�ȼ�Ҫ��
		strOut += pResult[nIndex+2];
		strOut += UnicodeToUtf8(s2ws("\n����Ҫ��")); //����Ҫ��
		strOut += pResult[nIndex+16];
		if (pResult[nIndex] != UnicodeToUtf8(s2ws("33")) && pResult[nIndex] != UnicodeToUtf8(s2ws("34"))) //��֧ԮԶ��
		{
			strOut += "\n";
			if (pResult[nIndex+6] != NULL && pResult[nIndex+6] != spaceforcmp)
			{
				strOut += UnicodeToUtf8(s2ws("�ᶽ���飺")); //�ᶽ����
				strOut += pResult[nIndex+6];
				strOut += " ";
			}
			if (pResult[nIndex+7] != NULL && pResult[nIndex+7] != spaceforcmp)
			{
				strOut += UnicodeToUtf8(s2ws("���ﾭ�飺")); //���ﾭ��
				strOut += pResult[nIndex+7];
			}
			strOut += UnicodeToUtf8(s2ws("\n��Դ��ȡ��"));
			if (pResult[nIndex+8] != NULL && pResult[nIndex+8] != spaceforcmp) //�����
			{
				strOut += UnicodeToUtf8(s2ws("��"));
				strOut += pResult[nIndex+8];
				strOut += " ";
			}
			if (pResult[nIndex+9] != NULL && pResult[nIndex+9] != spaceforcmp) //��ҩ
			{
				strOut +=UnicodeToUtf8(s2ws( "��"));
				strOut += pResult[nIndex+9];
				strOut += " ";
			}
			if (pResult[nIndex+10] != NULL && pResult[nIndex+10] != spaceforcmp) //��
			{
				strOut += UnicodeToUtf8(s2ws("��"));
				strOut += pResult[nIndex+10];
				strOut += " ";
			}
			if (pResult[nIndex+11] != NULL && pResult[nIndex+11] != spaceforcmp) //��
			{
				strOut += UnicodeToUtf8(s2ws("��"));
				strOut += pResult[nIndex+11];
				strOut += " ";				
			}
			if (pResult[nIndex+12] != NULL && pResult[nIndex+12] != spaceforcmp) //�����޸�Ͱ
			{
				strOut += UnicodeToUtf8(s2ws("����"));
				strOut += pResult[nIndex+12];
				strOut += " ";
			}
			if (pResult[nIndex+13] != NULL && pResult[nIndex+13] != spaceforcmp) //���
			{
				strOut += UnicodeToUtf8(s2ws("���"));
				strOut += pResult[nIndex+13];
				strOut += " ";
			}
			if (pResult[nIndex+14] != NULL && pResult[nIndex+14] != spaceforcmp) //�ʲ�
			{
				strOut += UnicodeToUtf8(s2ws("�ʲ�"));
				strOut += pResult[nIndex+14];
				strOut += " ";
			}
			if (pResult[nIndex+15] != NULL && pResult[nIndex+15] != spaceforcmp) //�Ҿ���
			{
				strOut += UnicodeToUtf8(s2ws("�Ҿ���"));
				strOut += pResult[nIndex+15];
			}
		}
		 
	}
	ret = nRow;
	sqlite3_free_table(pResult);  //ʹ���������ͷ�Ϊ��¼������ڴ棬������ڴ�й©
	//cout<<strOut<<endl;  
	sendmsg += ws2s(Utf8ToUnicode(strOut));
	sqlite3_close(pDB);  

	return ret;
}

// UTF-8ת��UNICODE��ʽ
wstring Utf8ToUnicode(const string& str) 
{  
	// Ԥ��-�������п��ֽڵĳ���    
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);  
	// ��ָ�򻺳�����ָ����������ڴ�    
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);  
	// ��ʼ�򻺳���ת���ֽ�    
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen);  
	wstring ret_str = pUnicode;  
	free(pUnicode);  
	return ret_str;  
} 

// UNICODEת��UTF-8��ʽ
string UnicodeToUtf8(const wstring& wstr) 
{  
	// Ԥ��-�������ж��ֽڵĳ���    
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);  
	// ��ָ�򻺳�����ָ����������ڴ�    
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);  
	// ��ʼ�򻺳���ת���ֽ�    
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);  
	string ret_str = pAssii;  
	free(pAssii);  
	return ret_str;  
} 

// wstringת��string
string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}
// stringת��wstring
wstring s2ws(const std::string& s)  
{  
	setlocale(LC_ALL, "chs");   
	const char* _Source = s.c_str();  
	size_t _Dsize = s.size() + 1;  
	wchar_t *_Dest = new wchar_t[_Dsize];  
	wmemset(_Dest, 0, _Dsize);  
	mbstowcs(_Dest,_Source,_Dsize);  
	std::wstring result = _Dest;  
	delete []_Dest;  
	setlocale(LC_ALL, "C");  
	return result;  
}

//�ж�str�Ƿ�������
bool isNum(string str)  
{  
	stringstream sin(str);  
	double d;  
	char c;  
	if(!(sin >> d))  
		return false;  
	if (sin >> c)  
		return false;  
	return true;  
} 


// �滻�ַ������Ӵ�
string&   replace_all_distinct(string&   str, const  string&  old_value, const   string&   new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return   str;
}


// �ж��Ƿ��ǹ���ԱQQ
bool isManagerQQ(int64_t fromQQ)
{
	int64_t managerQQ[3] = { MY_QQNUM , TEST_QQNUM, 839551855};
	
	for (int i = 0; i < sizeof(managerQQ)/sizeof(int64_t); i++) {
		if (fromQQ == managerQQ[i]) {
			return true;
		}
	}
	return false;
}
