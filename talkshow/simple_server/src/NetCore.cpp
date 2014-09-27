#include "NetCore.h"
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <stdio.h>


namespace netcore
{
	// Copied from Multiplayer.cpp
	// If the first byte is ID_TIMESTAMP, then we want the 5th byte
	// Otherwise we want the 1st byte
	unsigned char GetPacketIdentifier(RakNet::Packet *p)
	{
		if (p == 0)
			return 255;

		if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		{
			RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
			return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
		}
		else
			return (unsigned char)p->data[0];
	}



	CNetCore::CNetCore()
		: m_pServer(NULL)
	{
	}


	CNetCore::~CNetCore()
	{
	}


	int CNetCore::Init(unsigned short uPort, unsigned int uMaxConnections)
	{
		m_pServer = RakNet::RakPeerInterface::GetInstance();
		//RakNet::RakNetStatistics *pRss;
		m_pServer->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
		m_pServer->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
		//	RakNet::PacketLogger packetLogger;
		//	m_pServer->AttachPlugin(&packetLogger);

#if LIBCAT_SECURITY==1
		cat::EasyHandshake handshake;
		char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
		char private_key[cat::EasyHandshake::PRIVATE_KEY_BYTES];
		handshake.GenerateServerKey(public_key, private_key);
		server->InitializeSecurity(public_key, private_key, false);
		FILE *fp = fopen("publicKey.dat", "wb");
		fwrite(public_key, sizeof(public_key), 1, fp);
		fclose(fp);
#endif


		RakNet::SocketDescriptor socketDescriptors[2];
		socketDescriptors[0].port = uPort;
		socketDescriptors[0].socketFamily = AF_INET; // Test out IPV4
		socketDescriptors[1].port = uPort;
		socketDescriptors[1].socketFamily = AF_INET6; // Test out IPV6
		bool bStarted = m_pServer->Startup(uMaxConnections, socketDescriptors, 2) == RakNet::RAKNET_STARTED;
		m_pServer->SetMaximumIncomingConnections(uMaxConnections);
		if (!bStarted)
		{
			// Try again, but leave out IPV6
			bStarted = m_pServer->Startup(uMaxConnections, socketDescriptors, 1) == RakNet::RAKNET_STARTED;
			if (!bStarted)
			{
				return NC_ERROR_SERVER_STARTUP_FAILED;
			}
		}

		m_pServer->SetOccasionalPing(true);
		m_pServer->SetUnreliableTimeout(1000);

		return NC_ERROR_SUCCESS;
	}


	int CNetCore::Recv()
	{
		// Message now holds what we want to broadcast
		char message[2048];

		if (!m_pServer)
		{
			return NC_ERROR_SERVER_IS_NULL;
		}
		// Holds packets
		RakNet::Packet* pPacket = m_pServer->Receive();
		if (!pPacket)
		{
			return NC_ERROR_SUCCESS;
		}
		// GetPacketIdentifier returns this
		unsigned char uPacketIdentifier = GetPacketIdentifier(pPacket);

		

		// Check if this is a network message packet
		switch (uPacketIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION from %s\n", pPacket->systemAddress.ToString(true));;
			break;


		case ID_NEW_INCOMING_CONNECTION:
			// Somebody connected.  We have their IP now
			printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", pPacket->systemAddress.ToString(true), pPacket->guid.ToString());
			//clientID = p->systemAddress; // Record the player ID of the client

			printf("Remote internal IDs:\n");
			for (int index = 0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
			{
				RakNet::SystemAddress internalId = m_pServer->GetInternalID(pPacket->systemAddress, index);
				if (internalId != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
				{
					printf("%i. %s\n", index + 1, internalId.ToString(true));
				}
			}

			break;

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;

		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", pPacket->systemAddress.ToString(true));
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST from %s\n", pPacket->systemAddress.ToString(true));;
			break;

		default:
			// The server knows the static data of all clients, so we can prefix the message
			// With the name data
			printf("%s\n", pPacket->data);

			// Relay the message.  We prefix the name for other clients.  This demonstrates
			// That messages can be changed on the server before being broadcast
			// Sending is the same as before
			sprintf(message, "%s", pPacket->data);
			m_pServer->Send(message, (const int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPacket->systemAddress, true);

			break;
		}

		m_pServer->DeallocatePacket(pPacket);

		return NC_ERROR_SUCCESS;
	}

	int netcore::CNetCore::Stop()
	{
		if (!m_pServer)
		{
			return NC_ERROR_SERVER_IS_NULL;
		}
		m_pServer->Shutdown(300);
		// We're done with the network
		RakNet::RakPeerInterface::DestroyInstance(m_pServer);
		return NC_ERROR_SUCCESS;
	}

	void CNetCore::Run()
	{
		while (true)
		{
			Recv();
		}

	}
}











