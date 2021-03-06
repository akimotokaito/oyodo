﻿/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载

using namespace std;

#include "myfunc.h"
#include "mytimer.h"
int ac = -1; //AuthCode 调用酷Q的方法时需要用到
int workingst = NOTWORK; //工作状态
int baoshist = NOTWORK;  //默认是否整点报时
bool enabled = false;
DWORD WINAPI funproc(LPVOID lpparentet);

/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	mytimer_start();
	//workingst = WORK;
	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {
	mytimer_end();
	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	//ostringstream os;
	int ret = 0;
	string sendmsg("");
	long sendint = 0;
	//vector<string> rcvmsg = split(msg, " ");

	//os << fromQQ;
	//sendmsg = "收到来自" + os.str() + "的消息，消息内容是"+msg;
	ret = charge(msg, sendmsg, sendint);
	if (ret == STOP_WORK || ret == START_WORK) {
		if (isManagerQQ(fromQQ) && workingst != START_WORK -ret) {
			workingst = START_WORK - ret;
			if (workingst == WORK){
				CQ_sendPrivateMsg(ac, fromQQ, "上班！");
			}
			else if (workingst == NOTWORK) {
				CQ_sendPrivateMsg(ac, fromQQ, "下班~");
			}
		}
		return EVENT_IGNORE;
	}
	else if (workingst == NOTWORK) {
		return EVENT_IGNORE;
	}

	if (ret == 0){
		/*char sendmsg2[1024];
		strcpy(sendmsg2, CQ_getGroupMemberInfoV2(ac, TEST_GRPNUM, 1784667245, 0));
		CQ_sendPrivateMsg(ac, fromQQ, sendmsg2);*/
		return EVENT_IGNORE;
	}
	if (ret == SEND_CERTAINMSG && fromQQ == MY_QQNUM){
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
		return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
	}
	
}


/*
* Type=2 群消息
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	//ostringstream os;
	string sendmsg("");
	long sendint = 0;
	int ret = 0;
	//vector<string> rcvmsg = split(msg, " ");

	//os << fromQQ;
	//sendmsg = "收到来自" + os.str() + "的消息，消息内容是"+msg;
	ret = charge(msg, sendmsg, sendint);
	
	// 管理员帐号控制上下班
	if (ret == START_WORK || ret == STOP_WORK) {
		if (isManagerQQ(fromQQ)) {
			if (workingst != START_WORK - ret) {
				workingst = START_WORK - ret;
				if (workingst == WORK) {
					CQ_sendGroupMsg(ac, fromGroup, "淀酱，开始上班了！");
				}
				else if (workingst == NOTWORK) {
					CQ_sendGroupMsg(ac, fromGroup, "终于下班啦~");
				}
			} else {
				if (workingst == WORK) {
					CQ_sendGroupMsg(ac, fromGroup, "淀酱已经在努力工作了！");
				}
				else if (workingst == NOTWORK) {
					CQ_sendGroupMsg(ac, fromGroup, "淀酱已经是下班状态了哦。");
				}
			}
		
		} else {
			CQ_sendGroupMsg(ac, fromGroup, "父亲不让淀酱听陌生人的话。");
		}
		return EVENT_IGNORE;
	}
	else if (workingst == NOTWORK) {
		return EVENT_IGNORE;
	}

	// 管理员帐号控制是否报时
	if (ret == STOP_BAOSHI || ret == START_BAOSHI) {
		if (isManagerQQ(fromQQ)) {		
			if (baoshist != START_BAOSHI - ret) {
				baoshist = START_BAOSHI - ret;
				if (baoshist == WORK) {
					CQ_sendGroupMsg(ac, fromGroup, "那么就让淀酱为您报时吧！");
				}
				else if (baoshist == NOTWORK) {
					CQ_sendGroupMsg(ac, fromGroup, "淀酱不再报时了哦，提督请自己注意时间~");
				}
			}
			else {
				if (baoshist == WORK) {
					CQ_sendGroupMsg(ac, fromGroup, "交给淀酱了，会记得提醒您时间的。");
				}
				else if (baoshist == NOTWORK) {
					CQ_sendGroupMsg(ac, fromGroup, "好的，提督，淀酱就不罗嗦啦。");
				}
			}
		}
		else {
			CQ_sendGroupMsg(ac, fromGroup, "父亲不让淀酱听陌生人的话呢。");
		}
		return EVENT_IGNORE;
	}
	else if (workingst == NOTWORK) {
		return EVENT_IGNORE;
	}

	// 管理员帐号控制禁言
	if (ret == BAN) {
		if (isManagerQQ(fromQQ)) {
			if (sendint == 0) {
				//CQ_sendPrivateMsg(ac, fromQQ, sendmsg.c_str());
				if (strcmp(sendmsg.c_str(), "2469931868") != 0) {
					CQ_setGroupBan(ac, fromGroup, stoi(sendmsg), 600);
					CQ_sendGroupMsg(ac, fromGroup, "请好好反省一下。");
				}
				else {
					CQ_sendGroupMsg(ac, fromGroup, "看来您的大脑需要让明石给修理一下。");
				}
				return EVENT_BLOCK;
			}else{
				//CQ_sendPrivateMsg(ac, fromQQ, sendmsg.c_str());
			if (strcmp(sendmsg.c_str(), "2469931868") != 0) {
				CQ_setGroupBan(ac, fromGroup, stoi(sendmsg), sendint);
				CQ_sendGroupMsg(ac, fromGroup, "请好好反省一下呢。");
					}
				else {
					CQ_sendGroupMsg(ac, fromGroup, "看来您的大脑需要让明石给修理一下。");
				}
				return EVENT_BLOCK;
			}
		}
		else {
			CQ_sendGroupMsg(ac, fromGroup, "对不起，您的军衔并没有该权限哦~");
			return EVENT_IGNORE;
		}
	}

	//CQ_sendPrivateMsg(ac, 526975248, msg);
	if (ret == 0)
	{
		return EVENT_IGNORE;
	}else if (ret/10 == 2) //想要查询详细攻略的反馈
	{
		switch(ret%10){
		case 0: //没有详细攻略
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
		return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
	}	
	
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	string sendmsg;
	

	string sSubType = itos(subType);
	string sSendTime = itos(sendTime);
	string sFromGroup = lltos(fromGroup);
	string sFromQQ = lltos(fromQQ);
	string sBeingOpQQ = lltos(beingOperateQQ);

	/*
	string sdmsg;
	sdmsg += sSubType;
	sdmsg += ", ";
	sdmsg += sSendTime;
	sdmsg += ", ";
	sdmsg += sFromGroup;
	sdmsg += ", ";
	sdmsg += sFromQQ;
	sdmsg += ", ";
	sdmsg += sBeingOpQQ;
	sdmsg += ", ";

	CQ_sendGroupMsg(ac, fromGroup, sdmsg.c_str());*/



	if (subType == 1) { //主动退群
		sendmsg += "淀酱要告诉大家一个很不幸的消息，\n就在刚才，接到司令部的文件。\n编号为：[";
		sendmsg += sBeingOpQQ;
		sendmsg += "]的提督主动辞职，\n离开了我们镇守府…";
		CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());
		
	}
	else if (subType == 2) { //被T
		/*sendmsg += "于时间";
		sendmsg += sSendTime;
		sendmsg += "，";
		sendmsg += sBeingOpQQ;
		sendmsg += "被";
		sendmsg += sFromQQ;
		sendmsg += "辞退，离开了";
		CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());*/
		sendmsg += "淀酱要告诉大家一个很不幸的消息，\n就在刚才，接到司令部的文件。\n编号为：[";
		sendmsg += sBeingOpQQ;
		sendmsg += "]的提督因[不可名状的理由]，\n被其上司，编号为：[";
		sendmsg += sFromQQ; 
		sendmsg += "]的提督强行辞退，\n离开了我们镇守府…";
		CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());
		
	}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	string sendmsg;

	string sSubType = itos(subType);
	string sSendTime = itos(sendTime);
	string sFromGroup = lltos(fromGroup);
	string sFromQQ = lltos(fromQQ);
	string sBeingOpQQ = lltos(beingOperateQQ);

	if (subType == 1) { //管理员已同意
		sendmsg += "各位提督，十分抱歉，淀酱打扰一下~\n欢迎编号为：[";
		sendmsg += sBeingOpQQ;
		sendmsg += "]的提督加入我们镇守府!\n祝您武运昌隆！";
		CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());
	}
	else if (subType == 2) { //管理员邀请
		sendmsg += "打扰各位提督一下~\n欢迎编号为：[";
		sendmsg += sBeingOpQQ;
		sendmsg += "]的提督加入我们镇守府!\n请问您和编号为：[";
		sendmsg += sFromQQ;
		sendmsg += "]的提督是否有什么不可告人的关系？\n咳咳…淀酱多嘴了，总之祝您武运昌隆！";
		CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());
		/*sendmsg += "淀酱要告诉大家一个很不幸的消息，\n就在刚才，接到司令部的文件。\n编号为：[";
		sendmsg += sBeingOpQQ;
		sendmsg += "]的提督因[不可名状的理由]，\n被其上司，编号为：[";
		sendmsg += sFromQQ;
		sendmsg += "]的提督强行辞退，\n离开了我们镇守府…";
		CQ_sendGroupMsg(ac, fromGroup, sendmsg.c_str());*/
	}
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	if(strlen(msg) >= strlen("loveakashi")){
		if(strstr(msg,"loveakashi") != NULL){
			CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "密码正确");
		}else{
			CQ_setFriendAddRequest(ac, responseFlag, REQUEST_DENY, "密码错误");
		}
	}
	return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
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
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

void mytimer_start()
{
	CreateThread(NULL, 0, funproc, NULL, 0, NULL);
}

void mytimer_end()
{
	CloseHandle(funproc);//HANDLE hObject 对象句柄
}



DWORD WINAPI funproc(LPVOID lpparentet)
{
	//time_t now_time;
	//now_time = time(NULL);
	char timez[100] = {0};
	string now_time;
	//BOOL isFirst = true;
	BOOL nFlag = true; //记录是否报过当前时,true为没报过时，false为报过
	BOOL newFirst = true; //新年第一次报时flag
	int lastYxFlag = -1; //记录当天是否提醒过演习
	int lasthour = -1;
	SYSTEMTIME sys;
	//struct tm *aa;
	//time_t tt;
	//time(&tt);
	//aa = localtime(&tt);

	//获取系统时间，每次开启程序执行一次(9点到10点间开启的画才会上班第一次报时)
	GetLocalTime(&sys);
	if (sys.wHour >= 9 && sys.wHour <= 10) {
		string firstlogin;
		firstlogin += "现在是北京时间";
		firstlogin += to_string(sys.wHour);// aa->tm_hour;
		firstlogin += "时";
		firstlogin += to_string(sys.wMinute);// aa->tm_min;
		firstlogin += "分";
		//firstlogin += to_string(sys.wSecond);// aa->tm_sec;
		firstlogin += "，提督，早上好！新的一天开始了，淀酱也要开始上班啦~";
		CQ_sendGroupMsg(ac, MY_GRPNUM, firstlogin.c_str());
		//CQ_sendGroupMsg(ac, KANC_GRPNUM1, firstlogin.c_str());
		//CQ_sendGroupMsg(ac, TEST_GRPNUM, firstlogin.c_str());*/
	}


	// 根据当前时间确认是否为上班
	// 如果开启的时间为8点到23点之间，则状态为上班
	GetLocalTime(&sys);
	if (sys.wHour >= 8 && sys.wHour <= 22) {
		workingst = WORK;
	}else{
		workingst = NOTWORK;
	}

	// 时钟循环
	while (TRUE) {

		//memset(timez, 0, sizeof(timez));
		GetLocalTime(&sys);
		//sprintf(timez, "%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
		//now_time = timez;

		/*if (newFirst && sys.wDay == 28 && sys.wMonth == 1 && sys.wYear == 2017){ //28号后报时一次
			string sendmsg;
			string sendmsgmygrp;
			string sendmsgkcgrp;
			newFirst = false;
			sendmsg += "已是新的一年了，新年快乐~！\n今年也请各位多多关照。\n这里是大淀我一点小小心意，\n感谢各位长时间以来的支持！\n";
			sendmsgmygrp += sendmsg;
			sendmsgmygrp += "请您打开支付宝-口令红包，\n输入：“淀酱祝您新年快乐哟”\n领取由@苹果 赞助的红包~";
			CQ_sendGroupMsg(ac, MY_GRPNUM, sendmsgmygrp.c_str());
			sendmsgkcgrp += sendmsg;
			sendmsgkcgrp += "请您打开支付宝-口令红包，\n输入：“淀酱祝各位提督新年快乐”\n领取由淀酱的父亲@小秋 赞助的红包~";
			CQ_sendGroupMsg(ac, KANC_GRPNUM, sendmsgkcgrp.c_str());
		}else */
		
		// 下面if里是2点前夕的演习提醒
		if (sys.wDay != lastYxFlag && sys.wHour == 13 && ( sys.wMinute >= 30 && sys.wMinute <= 45)) {
			string yxString;
			if (workingst == WORK) {
				lastYxFlag = sys.wDay;
				yxString += "现在是北京时间";
				yxString += to_string(sys.wHour);// aa->tm_hour;
				yxString += "时";
				yxString += to_string(sys.wMinute);// aa->tm_min;
				yxString += "分";
				//firstlogin += to_string(sys.wSecond);// aa->tm_sec;
				yxString += "，提督您好！\n演习对手快要更新了，请不要忘记进行演习哦。\n每天有限次数的演习也是提升舰娘们战斗力的有效手段哦~";
				CQ_sendGroupMsg(ac, MY_GRPNUM, yxString.c_str());
				CQ_sendGroupMsg(ac, KANC_GRPNUM1, yxString.c_str());
				//CQ_sendGroupMsg(ac, KANC_GRPNUM2, sendmsg.c_str());
				//CQ_sendPrivateMsg(ac, MY_QQNUM, sendmsg.c_str());
			}

		}

		// 下面if里是整点报时
		if (sys.wMinute == 0) {
			if (nFlag) { //如果没报过时 就报一次时，且记录下最后一次报时的小时数
				string sendmsg;
				ifstream File;
				char * FileName = "app\\com.steve.oyodo\\baoshi\\oyodo.txt";
				char databuf[256];

				File.open(FileName);
				if (!File){
					cerr << "error oprening file myname!" << endl;
					//exit(-1);
					return 0;//读文件失败反返回0
				}
				else{
					for (size_t i = 0; !File.eof(); i++)  //这个循环找到小时对应的2行报时语音
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
				/*sendmsg += "现在是";
				sendmsg += to_string(sys.wHour);// aa->tm_hour;
				sendmsg += "时";
				sendmsg += to_string(sys.wMinute);// aa->tm_min;
				sendmsg += "分";
				sendmsg += to_string(sys.wSecond);// aa->tm_sec;
				sendmsg += "秒";*/
				if(workingst == WORK && baoshist == WORK){
					CQ_sendGroupMsg(ac, MY_GRPNUM, sendmsg.c_str());				
					CQ_sendGroupMsg(ac, KANC_GRPNUM1, sendmsg.c_str());
					//CQ_sendGroupMsg(ac, KANC_GRPNUM2, sendmsg.c_str());
					//CQ_sendPrivateMsg(ac, MY_QQNUM, sendmsg.c_str());
				}
				nFlag = false;
				lasthour = sys.wHour;
			}
			else//如果报过了，就
			{
				if (lasthour != sys.wHour)
				{
					nFlag = true;
				}
			}
			
		}
		Sleep(15000);
		
	}

	return 0;
}