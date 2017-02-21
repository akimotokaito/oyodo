#include "myfunc.h"

using namespace std;
sqlite3 *pDB;

#define MY_QQNUM 526975248
#define MY_GRPNUM 45694267
#define TEST_QQNUM 2043128624
#define TEST_GRPNUM 237712764
#define KANC_GRPNUM 531550460

//字符串分割函数
vector<string> split(string str,string pattern)
{
  string::size_type pos;
  vector<std::string> result;
  str+=pattern;//扩展字符串以方便操作
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

//字符串解析函数
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

	if (strstr(tmpstr.c_str(), "淀酱上班") != NULL  && tmpstr.size() <= 12) {
		return 99;
	}
	if (strstr(tmpstr.c_str(), "淀酱下班") != NULL  && tmpstr.size() <= 12) {
		return 98;
	}
	if (strstr(tmpstr.c_str(),"[CQ:at,qq=2469931868]") != NULL) {
		tmpstr = replace_all_distinct(tmpstr, "[CQ:at,qq=2469931868]", " ");
		return charge(tmpstr.c_str(), sendmsg);
	}
	if (strstr(tmpstr.c_str(),"远征") != NULL)
	{
		//sql_search(rcvmsg, sendmsg);
		if (tmpstr.size() > 100) {
			return 0;
		}
		if(strstr(tmpstr.c_str(),"远征") != tmpstr){
			sendmsg = "提督您想了解远征相关的事的话，输入“远征”我会详细告诉您的！";
			ret = 1;
		}else{
			ret = yuanzheng(tmpstr.c_str(), sendmsg);; //远征
		}
	}else if (strstr(tmpstr.c_str(),"攻略") != NULL)
	{
		if (tmpstr.size() > 100) {
			return 0;
		}
		if(strstr(tmpstr.c_str(),"攻略") != tmpstr){
			sendmsg = "提督您想了解攻略相关的事的话，输入“攻略”我会详细告诉您的！";
			ret = 2;
		}else{
			ret = gonglue(tmpstr.c_str(), sendmsg);; //攻略
		}
	}else if (strstr(tmpstr.c_str(),"发群") != NULL)
	{
		//sendgroupmsg(rcvmsg, sendmsg);
		ret = 10; //主控QQ控制机器人发消息到指定群
	}else
	{
		return ret;
	}
	//vector<string> rcvmsg = split(msg, " ");

	return ret;
}

//向特定群发送指定消息
long long sendgroupmsg(const char *msg, string & sendmsg)
{
	string tempmsg;
	vector<string> rcvmsg;
	long long ret = 0;
	//从字段‘发消息’开始将字符串截取存入sendmsg
	tempmsg = strstr(msg,"发群");
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

// 控制远征的函数
int yuanzheng(const char *msg, string & sendmsg)
{
	string tempmsg;
	vector<string> rcvmsg;
	vector<string>::iterator sit;
	int i = 0;
	int ret = 0;

	//从字段‘远征’开始将字符串截取存入sendmsg
	tempmsg = strstr(msg,"远征");
	tempmsg.erase(0,4);

	rcvmsg = split(tempmsg, " ");

	//下面这个循环把vector里所有内容为“”和“ ”的项全部删除
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
			//只有1个参数的情况
			//sendmsg = "只有1个参数的情况"+rcvmsg[0];
			//参数为''
			if (isNum(rcvmsg[0])) //如果只有一个参数且是数字
			{
				int num = 0;
				num = atoi(rcvmsg[0].c_str());
				if (0 < num && num <= 40)
				{
					string inputstr = " where NUM = ";
					inputstr += rcvmsg[0];
					ret = sql_searchbynum(inputstr.c_str(), sendmsg);
				}else{ //输入的数字不再目前远征编号范围内的话
					sendmsg = "提督，请您输入正确的远征编号！(远征1-40)";
					ret = 1;
				}
			}else if (rcvmsg[0] == "油") //查询燃料远征
			{
				string inputstr = "GETY";// where GETY > 500 and GETY != ''";
				sendmsg = "油最多的三个远征：\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "弹") //查询弹药远征
			{
				string inputstr = "GETD";
				sendmsg = "弹最多的三个远征：\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "钢") //查询钢材远征
			{
				string inputstr = "GETG";
				sendmsg = "钢最多的三个远征：\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "铝") //查询铝土远征
			{
				string inputstr = "GETL";
				sendmsg = "铝最多的三个远征：\n";
				ret = sql_searchbycat(inputstr.c_str(), sendmsg);
			}else if (rcvmsg[0] == "帮助" || rcvmsg[0] == "help" || rcvmsg[0] == "?" || rcvmsg[0] == "？")
			{
				ret = yuanzhenghelp(sendmsg);
			}
			break;
		case 2:
			//只有2个参数的情况
			//sendmsg = "只有2个参数的情况"+rcvmsg[0]+"\\"+rcvmsg[1];
			break;
		default:
			//其他情况
			//sendmsg = "输入的参数太多，目前最多只能输入2个参数";
			break;
	}
	return ret;
}

// 把文件内容读取到sendmsg里
int readfiletomsg(char * FileName, string & sendmsg)
{
	ifstream File;
	//char * FileName = "F:\\software\\kuq\\酷Q Air\\app\\com.steve.oyodo\\yuanzheng\\help.txt";
	char databuf[256];

	File.open(FileName);
	if(!File){
		cerr<<"error oprening file myname!"<<endl;
		//exit(-1);
		return 0;//读文件失败反返回0
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
		return 1;//读文件成功返回1
	}

}

// 远征帮助
int yuanzhenghelp(string & sendmsg)
{
	//char * FileName = "F:\\software\\kuq\\酷Q Air\\app\\com.steve.oyodo\\yuanzheng\\help.txt";
	char * FileName = "app\\com.steve.oyodo\\yuanzheng\\help.txt";
	return readfiletomsg(FileName,sendmsg);
}

// 攻略帮助
int gongluehelp(string & sendmsg)
{
	//char * FileName = "F:\\software\\kuq\\酷Q Air\\app\\com.steve.oyodo\\gonglue\\help.txt";
	char * FileName = "app\\com.steve.oyodo\\gonglue\\help.txt";
	return readfiletomsg(FileName,sendmsg);
}

// 控制攻略的函数
int gonglue(const char *msg, string & sendmsg)
{
	int ret = 0;
	string tempmsg;
	vector<string> rcvmsg;
	vector<string>::iterator sit;
	int i = 0;
	//char * FilePath = "F:\\software\\kuq\\酷Q Air\\app\\com.steve.oyodo\\gonglue\\";
	char * FilePath = "app\\com.steve.oyodo\\gonglue\\";

	//从字段‘攻略’开始将字符串截取存入sendmsg
	tempmsg = strstr(msg,"攻略");
	tempmsg.erase(0,4);

	rcvmsg = split(tempmsg, " ");
	//下面这个循环把vector里所有内容为“”和“ ”的项全部删除
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
		if (rcvmsg[0] == "帮助" || rcvmsg[0] == "help" || rcvmsg[0] == "?" || rcvmsg[0] == "？")
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
		if (rcvmsg[1] == "详" || rcvmsg[1] == "详细")
		{
			char FileName[128] ={0};
			char FileName2[128] ={0};
			sprintf(FileName,"%s%s%s", FilePath, rcvmsg[0].c_str(),"x.txt");
			sprintf(FileName2,"%s%s%s", FilePath, rcvmsg[0].c_str(),".txt");
			if (readfiletomsg(FileName, sendmsg) == 1)  //存在详细攻略的情况下
			{
				ret = 21;
			}else if(readfiletomsg(FileName2, sendmsg) == 1){ //不存在详细，但存在简略攻略
				ret = 22;
			}else{ //都不存在
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

// 根据资源类型查找远征结果
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

	//string filename = "F:\\software\\kuq\\酷Q Air\\app\\com.steve.oyodo\\yuanzheng\\kanyuanzheng.db";
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
		strOut += UnicodeToUtf8(s2ws("远征"));
		strOut += pResult[nIndex*i+3]; //编号		
		strOut += UnicodeToUtf8(s2ws(" 用时："));
		strOut += pResult[nIndex*i+4]; //用时
		strOut += UnicodeToUtf8(s2ws("分钟 资源"));
		strOut += pResult[nIndex*i+5]; //名称
		strOut += "\n";
	} 
	if (nRow > 0)
	{
		strOut.erase(strOut.size()-1);
	}
	sqlite3_free_table(pResult);  //使用完后务必释放为记录分配的内存，否则会内存泄漏
	//cout<<strOut<<endl;
	sendmsg += ws2s(Utf8ToUnicode(strOut));
	//sendmsg = strOut;
	sqlite3_close(pDB);  
	ret = nRow;
	return ret; 
}

// 根据远征编号查找结果
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
	//string filename = "F:\\software\\kuq\\酷Q Air\\app\\com.steve.oyodo\\yuanzheng\\kanyuanzheng.db";
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
	// 构造
	for(int i=0;i<nRow;i++)  
	{  
		strOut += UnicodeToUtf8(s2ws("远征"));
		strOut += pResult[nIndex]; //编号
		strOut += " ";
		strOut += pResult[nIndex+1]; //名称
		strOut += UnicodeToUtf8(s2ws("\n用时："));
		strOut += pResult[nIndex+3]; //用时
		strOut += UnicodeToUtf8(s2ws("分钟\n消耗：油"));
		if (pResult[nIndex+4] != NULL && pResult[nIndex+4] != spaceforcmp) //耗油
		{
			strOut += pResult[nIndex+4];

		}else{
			strOut += "-";
		}
		strOut += UnicodeToUtf8(s2ws("/弹"));
		if (pResult[nIndex+5] != NULL && pResult[nIndex+5] != spaceforcmp) //耗弹
		{
			strOut += pResult[nIndex+5];
		}else{
			strOut += "-";
		}
		strOut += UnicodeToUtf8(s2ws("\n旗舰等级：")); //等级要求
		strOut += pResult[nIndex+2];
		strOut += UnicodeToUtf8(s2ws("\n配置要求：")); //配置要求
		strOut += pResult[nIndex+16];
		if (pResult[nIndex] != UnicodeToUtf8(s2ws("33")) && pResult[nIndex] != UnicodeToUtf8(s2ws("34"))) //非支援远征
		{
			strOut += "\n";
			if (pResult[nIndex+6] != NULL && pResult[nIndex+6] != spaceforcmp)
			{
				strOut += UnicodeToUtf8(s2ws("提督经验：")); //提督经验
				strOut += pResult[nIndex+6];
				strOut += " ";
			}
			if (pResult[nIndex+7] != NULL && pResult[nIndex+7] != spaceforcmp)
			{
				strOut += UnicodeToUtf8(s2ws("舰娘经验：")); //舰娘经验
				strOut += pResult[nIndex+7];
			}
			strOut += UnicodeToUtf8(s2ws("\n资源获取："));
			if (pResult[nIndex+8] != NULL && pResult[nIndex+8] != spaceforcmp) //获得油
			{
				strOut += UnicodeToUtf8(s2ws("油"));
				strOut += pResult[nIndex+8];
				strOut += " ";
			}
			if (pResult[nIndex+9] != NULL && pResult[nIndex+9] != spaceforcmp) //弹药
			{
				strOut +=UnicodeToUtf8(s2ws( "弹"));
				strOut += pResult[nIndex+9];
				strOut += " ";
			}
			if (pResult[nIndex+10] != NULL && pResult[nIndex+10] != spaceforcmp) //钢
			{
				strOut += UnicodeToUtf8(s2ws("钢"));
				strOut += pResult[nIndex+10];
				strOut += " ";
			}
			if (pResult[nIndex+11] != NULL && pResult[nIndex+11] != spaceforcmp) //铝
			{
				strOut += UnicodeToUtf8(s2ws("弹"));
				strOut += pResult[nIndex+11];
				strOut += " ";				
			}
			if (pResult[nIndex+12] != NULL && pResult[nIndex+12] != spaceforcmp) //快速修复桶
			{
				strOut += UnicodeToUtf8(s2ws("快修"));
				strOut += pResult[nIndex+12];
				strOut += " ";
			}
			if (pResult[nIndex+13] != NULL && pResult[nIndex+13] != spaceforcmp) //喷火
			{
				strOut += UnicodeToUtf8(s2ws("喷火"));
				strOut += pResult[nIndex+13];
				strOut += " ";
			}
			if (pResult[nIndex+14] != NULL && pResult[nIndex+14] != spaceforcmp) //资材
			{
				strOut += UnicodeToUtf8(s2ws("资材"));
				strOut += pResult[nIndex+14];
				strOut += " ";
			}
			if (pResult[nIndex+15] != NULL && pResult[nIndex+15] != spaceforcmp) //家具箱
			{
				strOut += UnicodeToUtf8(s2ws("家具箱"));
				strOut += pResult[nIndex+15];
			}
		}
		 
	}
	ret = nRow;
	sqlite3_free_table(pResult);  //使用完后务必释放为记录分配的内存，否则会内存泄漏
	//cout<<strOut<<endl;  
	sendmsg += ws2s(Utf8ToUnicode(strOut));
	sqlite3_close(pDB);  

	return ret;
}

// UTF-8转成UNICODE格式
wstring Utf8ToUnicode(const string& str) 
{  
	// 预算-缓冲区中宽字节的长度    
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);  
	// 给指向缓冲区的指针变量分配内存    
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);  
	// 开始向缓冲区转换字节    
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen);  
	wstring ret_str = pUnicode;  
	free(pUnicode);  
	return ret_str;  
} 

// UNICODE转成UTF-8格式
string UnicodeToUtf8(const wstring& wstr) 
{  
	// 预算-缓冲区中多字节的长度    
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);  
	// 给指向缓冲区的指针变量分配内存    
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);  
	// 开始向缓冲区转换字节    
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);  
	string ret_str = pAssii;  
	free(pAssii);  
	return ret_str;  
} 

// wstring转成string
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
// string转成wstring
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

//判断str是否是数字
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


// 替换字符串中子串
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


// 判断是否是管理员QQ
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
