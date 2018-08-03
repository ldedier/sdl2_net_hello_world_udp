/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 22:24:58 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/03 23:44:20 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_process_server()
{

}

/*
void	ft_process_server()
{
	IPaddress			ip;

	TCPsocket			server_socket;
	SDLNet_SocketSet	socket_set;
	TCPsocket			clientSocket[MAX_CLIENTS];
	int					socketIsFree[MAX_CLIENTS];
	int					receivedByteCount = 0; 
	int					clientCount = 0; 
	const char			buffer[BUFF_SIZE];

	ft_printf("SERVER SIDE\n");
	if (SDLNet_ResolveHost(&ip, NULL, PORT) == -1)
		ft_error("could not resolveHost");
	server_socket = SDLNet_TCP_Open(&ip);
	if (server_socket == NULL)
		ft_error("could not open socket");
	socket_set = SDLNet_AllocSocketSet(MAX_SOCKETS);
	if (socket_set == NULL)
		ft_error("could not alloc socket set");

	if(SDLNet_TCP_AddSocket(socket_set, server_socket) == -1)
		ft_error("could not add socket");

	for (int loop = 0; loop < MAX_CLIENTS; loop++)
	{
		clientSocket[loop] = NULL;
		socketIsFree[loop] = 1;
	}
	int running = 1;
	while (running)
	{
		int num_rdy = SDLNet_CheckSockets(socket_set, 1000);
		if (num_rdy <= 0)
		{
			printf("no activity\n");
		}
		else
		{
			int serverSocketActivity = SDLNet_SocketReady(server_socket);
			if (serverSocketActivity)
			{
				if (clientCount < MAX_CLIENTS)
				{
					int freeSpot = -99;
					for (int loop = 0; loop < MAX_CLIENTS; loop++)
					{
						if (socketIsFree[loop] == 1)
						{
							socketIsFree[loop] = 0;
							freeSpot = loop;
							break;
						}
					}
					clientSocket[freeSpot] = SDLNet_TCP_Accept(server_socket);
					SDLNet_TCP_AddSocket(socket_set, clientSocket[freeSpot]);
					clientCount++;
					memset((char *)buffer, 0, BUFF_SIZE);
					strcpy((char *)buffer, "OK\n");
					int msgLength = strlen(buffer) + 1;
					SDLNet_TCP_Send(clientSocket[freeSpot], (void *)buffer, msgLength);
				}
				else
				{
					TCPsocket tempSock = SDLNet_TCP_Accept(server_socket);
					memset((char *)buffer, 0, BUFF_SIZE);
					strcpy((char *)buffer, "c pas cool\n");
					int msgLength = strlen(buffer) + 1;
					SDLNet_TCP_Send(tempSock, (void *)buffer, msgLength);
					SDLNet_TCP_Close(tempSock);
				}
			}
			for (int clientNumber = 0; clientNumber < MAX_CLIENTS; clientNumber++)
			{
				int clientSocketActivity = SDLNet_SocketReady(clientSocket[clientNumber]);
				if (clientSocketActivity != 0)
				{
					receivedByteCount = SDLNet_TCP_Recv(clientSocket[clientNumber], (char *)buffer, BUFF_SIZE);
					if (receivedByteCount <= 0) //disconnected
					{
						SDLNet_TCP_DelSocket(socket_set, clientSocket[clientNumber]);
						SDLNet_TCP_Close(clientSocket[clientNumber]);
						clientSocket[clientNumber] = NULL;
						socketIsFree[clientNumber] = 1;
						clientCount--;
					}
					else
					{
						printf("on a recu %s\n", buffer);
						int originatingClient = clientNumber;
						int msgLength = strlen(buffer) + 1;
						for (int loop = 0; loop < MAX_CLIENTS; loop++)
						{
							if (msgLength > 1 && loop != originatingClient && socketIsFree[loop] == 0)
							{
								SDLNet_TCP_Send(clientSocket[loop], (void *)buffer, msgLength);
							}
						}
						if ( strcmp(buffer, "shutdown") == 0 )
							running = 0;
					}
				}
			}
		}
	}
	SDLNet_FreeSocketSet(socket_set);
	SDLNet_TCP_Close(server_socket);
	SDLNet_Quit();
}
*/


