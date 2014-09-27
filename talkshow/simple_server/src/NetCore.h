#ifndef __NET_CORE_H_
#endif
#include "server_def.h"

namespace RakNet
{
	class RakPeerInterface;
}

namespace netcore
{
	enum NetCoreErrorCode
	{
		NC_ERROR_SUCCESS = 0,
		NC_ERROR_SERVER_IS_NULL = -1,
		NC_ERROR_SERVER_STARTUP_FAILED = -2,
	};

	class CNetCore
	{
	public:
		CNetCore();
		~CNetCore();
		int Init(unsigned short uPort, unsigned int uMaxConnections = 300);
	private:
		
		RakNet::RakPeerInterface* m_pServer;
	public:
		int Recv();
		int Stop();
		void Run();
	};

}