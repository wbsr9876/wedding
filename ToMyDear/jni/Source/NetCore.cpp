/*
 * NetCore.cpp
 *
 *  Created on: 2014Äê9ÔÂ28ÈÕ
 *      Author: WangZuo
 */

#include <jni.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include <stdio.h>

#if LIBCAT_SECURITY==1
#include "SecureHandshake.h" // Include header for secure handshake
#endif


RakNet::RakPeerInterface *g_pClient = NULL;
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_tomydear_NetCore
 * Method:    Init
 * Signature: (Ljava/lang/String;C)I
 */
JNIEXPORT jint JNICALL Java_com_example_tomydear_NetCore_Init
  (JNIEnv *, jobject)
{
	g_pClient = RakNet::RakPeerInterface::GetInstance();
	if(!g_pClient) return -1;
	g_pClient->AllowConnectionResponseIPMigration(false);
	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads
	RakNet::SocketDescriptor socketDescriptor(0,0);
	socketDescriptor.socketFamily=AF_INET;
	g_pClient->Startup(8,&socketDescriptor, 1);
	g_pClient->SetOccasionalPing(true);
	return 0;
}

/*
 * Class:     com_example_tomydear_NetCore
 * Method:    Connect
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_tomydear_NetCore_Connect
  (JNIEnv *pEnv, jobject, jstring ip, jchar port)
{
	if(!g_pClient) return -1;
	const char * strIp = pEnv->GetStringUTFChars(ip,0);
#if LIBCAT_SECURITY==1
	char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
	FILE *fp = fopen("publicKey.dat","rb");
	fread(public_key,sizeof(public_key),1,fp);
	fclose(fp);
#endif

#if LIBCAT_SECURITY==1
	RakNet::PublicKey pk;
	pk.remoteServerPublicKey=public_key;
	pk.publicKeyMode=RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
	bool b = g_pClient->Connect(strIp, port, "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"), &pk)==RakNet::CONNECTION_ATTEMPT_STARTED;
#else
	RakNet::ConnectionAttemptResult car = g_pClient->Connect(strIp, port, "Rumpelstiltskin", (int) strlen("Rumpelstiltskin"));
	RakAssert(car==RakNet::CONNECTION_ATTEMPT_STARTED);
#endif
	pEnv->ReleaseStringUTFChars(ip,strIp);
	return 0;
}

/*
 * Class:     com_example_tomydear_NetCore
 * Method:    Stop
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_tomydear_NetCore_Stop
  (JNIEnv *, jobject)
{
	if(!g_pClient) return 0;
	// Be nice and let the server know we quit.
	g_pClient->Shutdown(300);

	// We're done with the network
	RakNet::RakPeerInterface::DestroyInstance(g_pClient);
	return 0;
}

/*
 * Class:     com_example_tomydear_NetCore
 * Method:    Send
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_tomydear_NetCore_Send
  (JNIEnv *, jobject)
{
	return 0;
}


// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(RakNet::Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char) p->data[0];
}

/*
 * Class:     com_example_tomydear_NetCore
 * Method:    Recv
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_tomydear_NetCore_Recv
  (JNIEnv *, jobject)
{
	if(!g_pClient) return -1;
	// Holds packets
	RakNet::Packet* pPacket = g_pClient->Receive();
	if(!pPacket) return 0;
	// GetPacketIdentifier returns this
	unsigned char packetIdentifier = GetPacketIdentifier(pPacket);
	// Get a packet from either the server or the client

	// Check if this is a network message packet
	switch (packetIdentifier)
	{
	case ID_DISCONNECTION_NOTIFICATION:
		// Connection lost normally
		printf("ID_DISCONNECTION_NOTIFICATION\n");
		break;
	case ID_ALREADY_CONNECTED:
		// Connection lost normally
		printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", pPacket->guid);
		break;
	case ID_INCOMPATIBLE_PROTOCOL_VERSION:
		printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
		break;
	case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
		printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
		break;
	case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
		printf("ID_REMOTE_CONNECTION_LOST\n");
		break;
	case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
		printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
		break;
	case ID_CONNECTION_BANNED: // Banned from this server
		printf("We are banned from this server.\n");
		break;
	case ID_CONNECTION_ATTEMPT_FAILED:
		printf("Connection attempt failed\n");
		break;
	case ID_NO_FREE_INCOMING_CONNECTIONS:
		// Sorry, the server is full.  I don't do anything here but
		// A real app should tell the user
		printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
		break;

	case ID_INVALID_PASSWORD:
		printf("ID_INVALID_PASSWORD\n");
		break;

	case ID_CONNECTION_LOST:
		// Couldn't deliver a reliable packet - i.e. the other system was abnormally
		// terminated
		printf("ID_CONNECTION_LOST\n");
		break;

	case ID_CONNECTION_REQUEST_ACCEPTED:
		// This tells the client they have connected
		printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", pPacket->systemAddress.ToString(true), pPacket->guid.ToString());
		printf("My external address is %s\n", g_pClient->GetExternalID(pPacket->systemAddress).ToString(true));
		break;
	case ID_CONNECTED_PING:
	case ID_UNCONNECTED_PING:
		printf("Ping from %s\n", pPacket->systemAddress.ToString(true));
		break;
	default:
		// It's a client, so just show the message
		printf("%s\n", pPacket->data);
		break;
	}

	g_pClient->DeallocatePacket(pPacket);

	return 0;
}

#ifdef __cplusplus
}
#endif


