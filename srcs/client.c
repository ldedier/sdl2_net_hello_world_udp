/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/06 01:13:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

UDPpacket *packet;
IPaddress serverIP;
UDPsocket ourSocket;

int InitSDL_NetClient()
{
	if (SDLNet_Init() == -1)
		return 0; 
	return 1;
}

int CreatePacketClient( size_t packetSize )
{
	//	Allocate memory for the packet
	packet = SDLNet_AllocPacket( packetSize );
	if (packet == NULL)
		return 0; 
	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	packet->address.host = serverIP.host; 
	packet->address.port = serverIP.port;
	return 1;
}

int OpenPortClient(void)
{
	//Sets our socket with our local port

	ourSocket = SDLNet_UDP_Open(0);
	if (ourSocket == NULL )
	{
		printf("coudln't open socket\n");
		return 0;
	}
	return 1;
}

int SetIPAndPortClient( char *ip, int port)
{
	if (SDLNet_ResolveHost( &serverIP, ip, port )  == -1 )
		return 0; 
	return 1; 
}

int InitClient( char *ip, int remotePort, size_t packet_size)
{
	if (!InitSDL_NetClient())
		return 0;
	if ( !OpenPortClient())
		return 0;
	if ( !SetIPAndPortClient(ip, remotePort))
		return 0;
	if (!CreatePacketClient(packet_size))
		return 0;
	return 1;
}

//Send data. 
int Send(char *str)
{
	int length;
	int nb_packets;
	
	length = ft_strlen(str);
	
	//Set the data
	memcpy(packet->data, str, length);
	packet->len = length;
	//	SDLNet_UDP_Send returns number of packets sent. 0 means error
	if ((nb_packets = SDLNet_UDP_Send(ourSocket, -1, packet)) == 0)
	{
		printf("fail de send\n");
		return 0; 
	}
	else
	{
		printf("successfully sent %d\n", nb_packets);
	}
	return 1;
}

void	ft_process_client(char *serverName, char *port, char *message)
{
	if (!InitClient(serverName, atoi(port), 1000))
		exit(1);
	while (1)
	{
		Send(message);
	}
}
