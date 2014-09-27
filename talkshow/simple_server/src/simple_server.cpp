// simple_server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "server_def.h"
#include "NetCore.h"
#include "Thread.h"

int main(int argc, char* argv[])
{
	netcore::CNetCore* pTest = new netcore::CNetCore();
	pTest->Init(8888);
	netcore::CThread<netcore::CNetCore>* pThread = new netcore::CThread<netcore::CNetCore>;
	pThread->Create(pTest, &netcore::CNetCore::Run);
	while (true)
	{
		Sleep(1);
	}
	pThread->EndThread();
	return 0;
}

