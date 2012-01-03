#ifndef SERVER_CPP
#define SERVER_CPP
#include "Server.h"

C_Server::C_Server(bool turnServerOn)
{
	m_serverActive = turnServerOn;
	if(m_serverActive){
		//init
		m_connected = false;
		m_serverSocket = NULL;
		m_clientSocket = NULL;
		m_bytesSent = 0;
	
	
		if(SDLNet_Init() == -1)
			printf("SDLNet_Init: %s\n\n", SDLNet_GetError());

		//Setup server IP and port
		if(SDLNet_ResolveHost(&m_ipaddress, NULL, SERVER_PORT)== -1)
			printf("SDLNet_Init: %s\n", SDLNet_GetError());

		//Output HostIp
		if(!(m_hostIP = SDLNet_ResolveIP(&m_ipaddress)))
			printf("SDLNet_ResolveIP: %s\n", SDLNet_GetError());
		else
			printf("Host is: %s \n\n", m_hostIP);

		//Get a TCPsocket
		m_serverSocket = SDLNet_TCP_Open(&m_ipaddress);
		if(!m_serverSocket)
			printf("SDLNet_TCP_Open: %s\n\n", SDLNet_GetError());
		else
			printf("Server up and running. Awaiting client connection!....\n\n");

		//Create socket set AND add socket to monitor(handle up to 16 sockets) 
		m_socketSet = SDLNet_AllocSocketSet(1);
		if(!m_socketSet)
			printf("SDLNet_AllocSocketSet: %s\n\n", SDLNet_GetError());

	}//endif	
}
C_Server::~C_Server()
{
	if(m_serverActive){
		SDLNet_TCP_Close(m_clientSocket);
		SDLNet_Quit();
		//close
	}
}

bool C_Server::FindConnection()
{
	m_clientSocket = SDLNet_TCP_Accept(m_serverSocket);
	if(m_clientSocket){
		m_connected = true;
		m_socketsUsed = SDLNet_TCP_AddSocket(m_socketSet, m_clientSocket);
		if(m_socketsUsed == -1)
			printf("SDLNet_TCP_AddSocket: %s\n\n", SDLNet_GetError());
	}
	else
		m_connected = false;

	return m_connected;
}

void C_Server::UpdateClient()
{
	m_numOfSocketsReady = SDLNet_CheckSockets(m_socketSet, 0);
	if(m_numOfSocketsReady == -1){
		printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
		//most of the time this is a system error, where perror might help you.
		perror("SDLNet_CheckSockets");
	}
	else if(m_connected)
		m_result = SDLNet_TCP_Send(m_clientSocket, &s_outPacket, sizeof(s_outPacket));	
		std::cout << "bytes sent: " << (m_bytesSent += m_result) << std::endl;
}

void C_Server::Receive()
{
	if(m_clientSocket)
		m_numOfSocketsReady = SDLNet_CheckSockets(m_socketSet, 0);
	if(m_numOfSocketsReady == -1)
		printf("SDLNet_CheckSockets: %s\n\n", SDLNet_GetError());

	s_gamePacket tempPacket;
	if(m_numOfSocketsReady > 0){
		m_result = SDLNet_TCP_Recv(m_clientSocket, &tempPacket, sizeof(tempPacket));
		if(m_result > 0){
		
			s_inPacket.m_playerPosition.x = tempPacket.m_playerPosition.x;
			s_inPacket.m_playerPosition.y = tempPacket.m_playerPosition.y;
			std::cout << "bytes received: " << (m_bytesReceived += m_result) << "\n";
		}
	}
}


#endif