/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/12 14:54:59 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/18 21:25:57 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

UDPpacket *received_packet;
UDPpacket *packet;

UDPsocket ourSocket;
UDPsocket clientSocket;

int InitSDL_Net()
{
	if (SDLNet_Init() == -1)
		return 0; 
	return 1;
}

/*
int SetIPAndPortServer( char *ip, int port)
{
	if (SDLNet_ResolveHost( &serverIP, ip, port )  == -1 )
		return 0;
	return 1;
}
*/

int CreatePacketReceived( size_t packetSize )
{
	//	Allocate memory for the packet
	received_packet = SDLNet_AllocPacket( packetSize );
	if (received_packet == NULL)
		return 0; 
	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	return 1;
}

int CreatePacket( size_t packetSize )
{
	//	Allocate memory for the packet
	packet = SDLNet_AllocPacket( packetSize );
	if (packet == NULL)
		return 0; 
	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	return 1;
}

int OpenPort( int localport )
{
	//Sets our socket with our local port
	ourSocket = SDLNet_UDP_Open(localport);
	if (ourSocket == NULL )
	{
		printf("coudln't open socket at port %d\n", localport);
		return 0; 
	}
	return 1;
}

int InitServer(int port, size_t packet_size)
{
	if (!InitSDL_Net())
		return 0;
	if ( !OpenPort(port))
		return 0;
	if (!CreatePacketReceived(packet_size))
		return 0;
	if (!CreatePacket(packet_size))
		return 0;
	return 1;
}

int SendFromServer(char *str)
{
	int length;
	int nb_packets;

	length = ft_strlen(str);
	memcpy(packet->data, str, length);
	packet->len = length;
	if ((nb_packets = SDLNet_UDP_Send(clientSocket, -1, packet)) == 0)
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

void CheckForData()
{
	char *str;
	static int i = 0;
	
	//	Check if there is a packet waiting for us...
	if (SDLNet_UDP_Recv(ourSocket, received_packet))
	{
		str = ft_strndup((char *)received_packet->data, received_packet->status);

		printf("on recoit :%s\n", str);
		printf("port: %u\n", received_packet->address.port);
		printf("host: %u\n", received_packet->address.host);
		printf("i: %d\n", i);
		packet->address.host = received_packet->address.host;
		packet->address.port = received_packet->address.port;
		if (!i)
		{
			i = 1;
			clientSocket = SDLNet_UDP_Open(received_packet->address.port);
			if (clientSocket == NULL)
			{
				printf("boloss\n");
				exit(1);
			}
		}
		SendFromServer("d barres");
	}
	else 
		printf(RED"on a recu R\n"RESET);
}

void	ft_process_server(char *port)
{
	if (!InitServer(atoi(port), 1000))
		exit(1);
	while (1)
	{
		CheckForData();
	}
}
