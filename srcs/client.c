/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/20 00:03:30 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

UDPpacket *packet;
UDPpacket *received_packet;

IPaddress serverIP;
IPaddress ourIP;
UDPsocket ourSocket;

t_message *received_message;
t_message *message;

int InitSDL_NetClient()
{
	if (SDLNet_Init() == -1)
		return 0; 
	return 1;
}

int CreatePacketClient( size_t packetSize ) //Packets clients vers serveur
{
	//	Allocate memory for the packet
	packet = SDLNet_AllocPacket( packetSize );
	if (packet == NULL)
		return 0; 
	// Set the destination host and port
	// We got these from calling SetIPAndPort()

	packet->address.host = serverIP.host; //info du serveur
	packet->address.port = serverIP.port; //info du serveur

	//printf("host of serverIP %u\n", serverIP.host);
	//printf("port of serverIP %u\n", serverIP.port);
	return (1);
}

int CreatePacketReceivedClient( size_t packetSize ) // Packets serveurs vers clients
{
	//	Allocate memory for the packet
	received_packet = SDLNet_AllocPacket(packetSize);
	if (received_packet == NULL)
		return 0; 
	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	return 1;
}


int OpenPortClient(void)
{
	ourSocket = SDLNet_UDP_Open(0); // 0 => take any available port
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

int InitClient(char *ip, int remotePort, size_t packet_size)
{
	if (!InitSDL_NetClient())
		return 0;
	if ( !OpenPortClient())
		return 0;
	if ( !SetIPAndPortClient(ip, remotePort))
		return 0;
	if (!CreatePacketClient(packet_size))
		return 0;
	message = malloc(sizeof(t_message));
	received_message = malloc(sizeof(t_message));
	return (1);
}

//Send data. 

int Send(char *str) //send from client to server
{
	int length;
	int nb_packets;
	length = ft_strlen(str);

	memset(message->data, '\0', BUFF_SIZE);
	memcpy(message->data, str, ft_strlen(str));
	packet->len = sizeof(t_message);
	message->from = ourIP;
	memcpy(packet->data, message, sizeof(t_message));

	//	SDLNet_UDP_Send returns number of packets sent. 0 means error
	if ((nb_packets = SDLNet_UDP_Send(ourSocket, -1, packet)) == 0)
	{
		printf("fail de send\n");
		return 0;
	}
	else
	{
		printf("on a envoye\n\tport: %u\n\tadress:%u\n", packet->address.port, packet->address.host);
//		printf("successfully sent %d\n", nb_packets);
	}
	return (1);
}

void CheckForDataBack()
{
	char *str;
	static int i = 0;
	//  Check if there is a packet waiting for us...
	if (SDLNet_UDP_Recv(ourSocket, received_packet))
	{
		received_message = (t_message *)(received_packet->data);
		str = ft_strndup(received_message->data, ft_strlen(received_message->data));
		printf("on recoit du serveur: %s\n", str);
	}
//	else
//		printf(RED"on a recu R\n"RESET);
}

void	ft_process_client(char *serverName, char *port, char *message)
{
	if (!InitClient(serverName, atoi(port), sizeof(t_message)))
		exit(1);
	while (1)
	{
		Send(message);
		CheckForDataBack();
	}
}
