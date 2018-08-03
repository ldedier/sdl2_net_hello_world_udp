/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/03 23:45:26 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

struct termios orig_termios;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

int kbHit()
{
	long waitSeconds      = 1L;
	long waitMicroSeconds = 0L;
	struct timeval tv = { waitSeconds, waitMicroSeconds };
	
	fd_set fds;
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

char *ft_strjoin_char(char *str, char c)
{
	char *new;
	int length;

	length = strlen(str);
	new = ft_strnew(length + 1);
	strcpy(new, str);
	new[length] = c;
	return (new);
}

int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0)
	{
		return r;
	}
	else
	{
		return c;
	}
}

void	ft_process_client()
{

}

/*
void	ft_process_client()
{
	const char *host;

	IPaddress serverIP;
	TCPsocket clientSocket;
	char *userInput = ft_strnew(0);
	char *serverName = strdup("10.13.3.11");

	int inputLength  = 0;
	char buffer[BUFF_SIZE];

	if (SDLNet_Init() < 0)
		ft_error("could not init sdlnet");

	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	if (socketSet == NULL)
		ft_error("failed to allocate the socket set");
	else
		printf("allocated socket set successfully\n");

	int hostResolved = SDLNet_ResolveHost(&serverIP, serverName, PORT);
	if (hostResolved == -1)
		printf("failed to resolve the server host name\n");
	else
	{
		Uint8 * dotQuad = (Uint8*)&serverIP.host;
		printf("succesfully resolved host to IP %d.%d.%d.%d\n",
			(unsigned short)dotQuad[0],
			(unsigned short)dotQuad[1],
			(unsigned short)dotQuad[2],
			(unsigned short)dotQuad[3]);
		printf("port %d\n" ,SDLNet_Read16(&serverIP.port));		
	}

	if ((host = SDLNet_ResolveIP(&serverIP)) == NULL)
		printf("Failed to resolve the server IP address");
	else
		printf("Successfully resolved IP to host %s", host);
	
	int shutdownClient = 1;

	clientSocket = SDLNet_TCP_Open(&serverIP);
	if (!clientSocket)
		ft_error("Failed to open socket to server");
	else
	{
		printf("Connection okay, about to read connection status from the server...");
		
		SDLNet_TCP_AddSocket(socketSet, clientSocket);
		int activeSockets = SDLNet_CheckSockets(socketSet, 5000);
		int gotServerResponse = SDLNet_SocketReady(clientSocket);
		if (gotServerResponse != 0)
		{
			int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFF_SIZE);
			printf("Got the following from server: %s", buffer);
			if ( strcmp(buffer, "OK\n") == 0 )
			{
				shutdownClient = 0;
				printf("Joining..\n");
			}
			else
				printf("server full\n");
		}
		else
			printf("no response from server\n");	
	}

	int wrotePrompt = 0;
	int sendMessage = 0;

	while (shutdownClient == 0)
	{
		if (wrotePrompt == 0)
		{
		//	printf("write something:\n");
			wrotePrompt = 1;
		}

		set_conio_terminal_mode();
		int status = kbHit();
		reset_terminal_mode();
//		printf("status is %d\n", status);

		if (status != 0)
		{
		//	printf("key was pressed\n");
			set_conio_terminal_mode();
			char theChar = getch();
			reset_terminal_mode();

			printf("%c", theChar);
			fflush(stdout);

			if ((int)theChar != 13)
				userInput = ft_strjoin_char(userInput, theChar);
			else
			{
		//		printf("user pressed return\n");

				strcpy( buffer, userInput );
				printf("on envoie %s\n", (char *)buffer);
				inputLength = strlen(buffer) + 1;

				if (SDLNet_TCP_Send(clientSocket, (void *)buffer, inputLength) < inputLength)
					ft_error("failed to send message");
				else
				{
					printf("message sent successfuly\n");

					if (sendMessage == 1 && (!strcmp(userInput, "quit") || !strcmp(userInput, "exit") || !strcmp(userInput, "shutdown")))
						shutdownClient = 1;
					wrotePrompt = 0;
					sendMessage = 0;
					userInput = ft_strnew(0);
				}
			}
		}

		int socketActive = SDLNet_CheckSockets(socketSet, 0);

		if (socketActive != 0)
		{
			int messageFromServer = SDLNet_SocketReady(clientSocket);

			if (messageFromServer != 0)
			{
				printf("got a response from the server...\n");
				int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFF_SIZE);
				printf("received: %s\n", buffer);
				
				if (strcmp(buffer, "shutdown") == 0)
				{
					printf("server is going down...\n");
					shutdownClient = 1;
				}
			}
			else
				printf("no response from the server...\n");
		}
	}
	SDLNet_TCP_Close(clientSocket);
	SDLNet_Quit();
	reset_terminal_mode();
}
*/
