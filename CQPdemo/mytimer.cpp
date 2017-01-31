#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "cqp.h"
using namespace std;
DWORD WINAPI funproc(LPVOID lpparentet);



void mytimer_start() 
{
	CreateThread(NULL, 0, funproc, NULL, 0, NULL);
}

void mytimer_end()
{
	CloseHandle(funproc);//HANDLE hObject ¶ÔÏó¾ä±ú
}



DWORD WINAPI funproc(LPVOID lpparentet)
{
	while (1) {
		Sleep(3000);
		CQ_sendPrivateMsg(-1, 526975248, "Timer test");
	}
	
	return 0;
}