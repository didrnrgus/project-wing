// ProjectWing 멀티플레이 소켓 서버.
// 방장 클라이언트가 ../Bin/Server/{Debug|Release}/project-wing-socket-server.exe 로 띄운다.
// 프로토콜: docs/05-network-protocol.md, 서버 동작: docs/06-server.md

#include "GameServer.h"

#include <cstdio>
#include <cstdlib>

namespace
{
	CGameServer gServer;

	BOOL WINAPI ConsoleCtrlHandler(DWORD _ctrlType)
	{
		switch (_ctrlType)
		{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
			gServer.Stop();
			return TRUE;
		default:
			return FALSE;
		}
	}
}

int main(int argc, char* argv[])
{
	setvbuf(stdout, nullptr, _IONBF, 0);	// 자식 프로세스로 떠도 로그가 바로 보이도록

	unsigned short port = SERVER_PORT;
	if (argc > 1)
		port = (unsigned short)atoi(argv[1]);

	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

	if (!gServer.Init(port))
	{
		gServer.Shutdown();
		return 1;
	}

	gServer.Run();
	gServer.Shutdown();
	return 0;
}
