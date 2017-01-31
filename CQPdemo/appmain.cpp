/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����

using namespace std;

#include "myfunc.h"
#include "mytimer.h"
int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
DWORD WINAPI funproc(LPVOID lpparentet);

/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	mytimer_start();
	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {
	mytimer_end();
	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	//ostringstream os;
	int ret = 0;
	string sendmsg("");
	//vector<string> rcvmsg = split(msg, " ");

	//os << fromQQ;
	//sendmsg = "�յ�����" + os.str() + "����Ϣ����Ϣ������"+msg;
	ret = charge(msg, sendmsg);
	if (ret == 0)
	{
		return EVENT_IGNORE;
	}
	if (ret == 10 && fromQQ == MY_QQNUM){
		int64_t fromGroup = 0;
		fromGroup = sendgroupmsg(msg, sendmsg);
		if (fromGroup > 0 && !sendmsg.empty())
		{		
			CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());
		}
		return EVENT_BLOCK;
	}else if (!sendmsg.empty() && sendmsg != "")
	{
		CQ_sendPrivateMsg(ac, fromQQ, sendmsg.c_str());
		return EVENT_BLOCK;
	} 
	else
	{
		return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
	}
	
}


/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	//ostringstream os;
	string sendmsg("");
	int ret = 0;
	//vector<string> rcvmsg = split(msg, " ");

	//os << fromQQ;
	//sendmsg = "�յ�����" + os.str() + "����Ϣ����Ϣ������"+msg;
	ret = charge(msg, sendmsg);
	
	//CQ_sendPrivateMsg(ac, 526975248, msg);
	if (ret == 0)
	{
		return EVENT_IGNORE;
	}else if (ret/10 == 2) //Զ���ķ���
	{
		switch(ret%10){
		case 0: //ʧ��
			return EVENT_IGNORE;
			break;
		case 1:
		case 2:
			if (!sendmsg.empty() && sendmsg != "")
			{
				CQ_sendPrivateMsg(ac, fromQQ, sendmsg.c_str());
				return EVENT_BLOCK;
			} 
			break;
		}
	}
	

	if (!sendmsg.empty() && sendmsg != "")
	{
		CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());
		return EVENT_BLOCK;
	} 
	else
	{
		return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
	}	
	
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	if(strlen(msg) >= strlen("loveakashi")){
		if(strstr(msg,"loveakashi") != NULL){
			CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "������ȷ");
		}else{
			CQ_setFriendAddRequest(ac, responseFlag, REQUEST_DENY, "�������");
		}
	}
	return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}
	if (subType == 1) {
		//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	} else if (subType == 2) {
		if( fromGroup == MY_GRPNUM){
			CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
		}
	}
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

void mytimer_start()
{
	CreateThread(NULL, 0, funproc, NULL, 0, NULL);
}

void mytimer_end()
{
	CloseHandle(funproc);//HANDLE hObject ������
}



DWORD WINAPI funproc(LPVOID lpparentet)
{
	//time_t now_time;
	//now_time = time(NULL);
	char timez[100] = {0};
	string now_time;
	//BOOL isFirst = true;
	BOOL nFlag = true; //��¼�Ƿ񱨹���ǰʱ,trueΪû����ʱ��falseΪ����
	int lasthour = -1;
	SYSTEMTIME sys;
	//struct tm *aa;
	//time_t tt;
	//time(&tt);
	//aa = localtime(&tt);

	//��ȡϵͳʱ�䣬ÿ�ο�������ִ��һ��
	/*GetLocalTime(&sys);
	string firstlogin;
	firstlogin += "������";
	firstlogin += to_string(sys.wHour);// aa->tm_hour;
	firstlogin += "ʱ";
	firstlogin += to_string(sys.wMinute);// aa->tm_min;
	firstlogin += "��";
	firstlogin += to_string(sys.wSecond);// aa->tm_sec;
	firstlogin += "�룬�ᶽ���ã����ϰ���~";
	CQ_sendGroupMsg(ac, MY_GRPNUM, firstlogin.c_str());
	//CQ_sendGroupMsg(ac, KANC_GRPNUM, firstlogin.c_str());
	//CQ_sendGroupMsg(ac, TEST_GRPNUM, firstlogin.c_str());*/



	while (TRUE) {

		//memset(timez, 0, sizeof(timez));
		GetLocalTime(&sys);
		//sprintf(timez, "%4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
		//now_time = timez;
		if (sys.wMinute == 0) {
			if (nFlag) { //���û����ʱ �ͱ�һ��ʱ���Ҽ�¼�����һ�α�ʱ��Сʱ��
				string sendmsg;
				ifstream File;
				char * FileName = "app\\com.steve.oyodo\\baoshi\\oyodo.txt";
				char databuf[256];

				File.open(FileName);
				if (!File){
					cerr << "error oprening file myname!" << endl;
					//exit(-1);
					return 0;//���ļ�ʧ�ܷ�����0
				}
				else{
					for (size_t i = 0; !File.eof(); i++)  //���ѭ���ҵ�Сʱ��Ӧ��2�б�ʱ����
					{
						File.getline(databuf, 256);
						if (i == 3 * sys.wHour){
							File.getline(databuf, 256);
							sendmsg += databuf;
							sendmsg += "\n";
							File.getline(databuf, 256);
							sendmsg += databuf;
							sendmsg += "\n";
							break;
						}
					}

					if (sendmsg.empty() == false)
					{
						sendmsg.erase(sendmsg.size() - 1);
					}
					File.close();
				}
				/*sendmsg += "������";
				sendmsg += to_string(sys.wHour);// aa->tm_hour;
				sendmsg += "ʱ";
				sendmsg += to_string(sys.wMinute);// aa->tm_min;
				sendmsg += "��";
				sendmsg += to_string(sys.wSecond);// aa->tm_sec;
				sendmsg += "��";*/
				CQ_sendGroupMsg(ac, MY_GRPNUM, sendmsg.c_str());
				//CQ_sendGroupMsg(ac, TEST_GRPNUM, sendmsg.c_str());
				//CQ_sendGroupMsg(ac, KANC_GRPNUM, sendmsg.c_str());
				//CQ_sendPrivateMsg(ac, MY_QQNUM, sendmsg.c_str());
				nFlag = false;
				lasthour = sys.wHour;
			}
			else//��������ˣ���
			{
				if (lasthour != sys.wHour)
				{
					nFlag = true;
				}
			}
			
		}
		Sleep(20000);
		
	}

	return 0;
}


/*
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
*/