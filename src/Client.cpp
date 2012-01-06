#ifndef CLIENT_CPP
#define CLIENT_CPP
#include "Client.h"

C_Client::C_Client()
{
	m_clientActive = false;
	if(m_clientActive){
		InitClient();
	}
	

}
C_Client::~C_Client()
{
	if(m_clientActive){
		SDLNet_TCP_Close(m_clientSocket);
		SDLNet_Quit();
		//close
	}
}

bool C_Client::FindConnection()
{
	m_clientSocket = SDLNet_TCP_Open(&m_ipaddress);
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

void C_Client::ReceivePacket()
{
	//Check if there's something to send
	if(m_clientSocket)
		m_readySockets = SDLNet_CheckSockets(m_socketSet, 0);
	if(m_readySockets == -1)
		printf("SDLNet_CheckSockets: %s\n\n", SDLNet_GetError());
	
	s_gamePacket tempPacket;
	if(m_readySockets > 0){
		m_result = SDLNet_TCP_Recv(m_clientSocket, &tempPacket, sizeof(tempPacket));
		if(m_result > 0){
		
			s_inPacket.m_playerPosition.x = tempPacket.m_playerPosition.x;
			s_inPacket.m_playerPosition.y = tempPacket.m_playerPosition.y;
			std::cout << "bytes received: " << (m_bytesReceived += m_result) << "\n";
		}
	}
}

void C_Client::Send()
{
	m_readySockets = SDLNet_CheckSockets(m_socketSet, 0);
	if(m_readySockets == -1){
		printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
		//most of the time this is a system error, where perror might help you.
		perror("SDLNet_CheckSockets");
	}
	else if(m_connected)
			m_result = SDLNet_TCP_Send(m_clientSocket, &s_outPacket, sizeof(s_outPacket));	
		std::cout << "bytes sent: " << (m_bytesSent += m_result) << std::endl;
}
void C_Client::isActive(bool isOn)
{
	m_clientActive = isOn;
}
void C_Client::InitClient()
{
	
		//init
		m_connected = false;
		m_clientSocket = NULL;
		m_bytesReceived = 0;
	

		if(SDLNet_Init() == -1)
			printf("SDLNet_Init: %s\n\n", SDLNet_GetError());

		//Setup server IP and port
		if(SDLNet_ResolveHost(&m_ipaddress, SERVER_IP, SERVER_PORT)== -1)
			printf("SDLNet_Init: %s\n", SDLNet_GetError());

		//Output HostIp
		if(!(m_hostIP = SDLNet_ResolveIP(&m_ipaddress)))
			printf("SDLNet_ResolveIP: %s\n", SDLNet_GetError());
		else
			printf("Client: attempting to connect to:  %s \n\n", m_hostIP);


		//Create socket set AND add socket to monitor(handle up to 16 sockets) 
		m_socketSet = SDLNet_AllocSocketSet(1);
		if(!m_socketSet)
			printf("SDLNet_AllocSocketSet: %s\n\n", SDLNet_GetError());
}
#endif