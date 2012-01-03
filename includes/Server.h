#ifndef SERVER_H
#define SERVER_H
#include <SDL_net.h>
#include <iostream>
#include "Config.h"



class C_Server{

	public:		
			bool m_connected;
			s_gamePacket s_playerDataPacket, s_inPacket, s_outPacket;

	protected:
			bool m_serverActive;
			long m_bytesSent, m_bytesReceived;
			const char* m_hostIP;//will host computer name ie: BIGGLES-PC
			TCPsocket m_serverSocket, m_clientSocket;
			SDLNet_SocketSet m_socketSet;
			IPaddress m_ipaddress;
			int m_socketsUsed, m_result, m_numOfSocketsReady;

	public:
			
			C_Server(bool turn_server_on);
			~C_Server();

			bool FindConnection();
			void UpdateClient();// This sends a packet with game info to clients
			void Receive();// Gets packets from clients
};
#endif