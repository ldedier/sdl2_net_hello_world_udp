/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/12 14:54:59 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/12 14:54:59 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

UDPpacket *packet;
UDPsocket ourSocket;

int InitSDL_Net()
{
	if (SDLNet_Init() == -1)
		return 0; 
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
	if (!CreatePacket(packet_size))
		return 0;
	return 1;
}

void CheckForData()
{
	char *str;
	static int i = 0;
	//	Check if there is a packet waiting for us...
	if (SDLNet_UDP_Recv(ourSocket, packet))
	{
		str = ft_strndup((char *)packet->data, packet->status);
		printf("on recoit :%s\n", str);
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
