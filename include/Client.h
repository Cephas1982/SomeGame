#ifndef CLIENT_H
#define CLIENT_H
#include <SDL_net.h>
#include <iostream>
#include "Config.h"


class C_Client{

	public:		
			bool m_connected;
			s_gamePacket s_playerDataPacket, s_outPacket, s_inPacket;

	private:
			bool m_clientActive;
			long m_bytesSent, m_bytesReceived;
			const char* m_hostIP;//will host computer name ie: BIGGLES-PC
			
			TCPsocket m_clientSocket;
			SDLNet_SocketSet m_socketSet;
			IPaddress m_ipaddress;
			int m_socketsUsed, m_readySockets, m_result;

	public:
			
			C_Client();
			~C_Client();
			bool FindConnection();
			void ReceivePacket();
			void Send();//sends packet info
			void isActive(bool on_or_off);
			void InitClient();
			
			
};
#endif