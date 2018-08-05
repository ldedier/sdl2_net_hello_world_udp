/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/05 22:48:29 by ldedier          ###   ########.fr       */
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

char	*ft_strjoin_char(char *str, char c)
{
	char *new;
	int length;

	length = strlen(str);
	new = ft_strnew(length + 1);
	strcpy(new, str);
	new[length] = c;
	free(str);
	return (new);
}

char	*ft_strdel_char(char *str)
{
	if (strlen(str) == 0)
		return (str);
	else
	{
		str[strlen(str) - 1] = 0;
		return (str);
	}
}

int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0)
		return r;
	else
		return c;
}

void	ft_print_message(t_message *message)
{
	if(message->flags == FROM_SERVER)
	{
		ft_printf(UNDERLINE YELLOW"%s:"RESET, message->author);
		ft_printf(CYAN" %s\n"RESET, message->content);
	}
	else
	{
		ft_printf(UNDERLINE GREEN"%s:"RESET, message->author);
		ft_printf(CYAN" %s\n"RESET, message->content);
	}
}

UDPpacket *packet;
IPaddress serverIP;
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
	packet->address.host = serverIP.host; 
	packet->address.port = serverIP.port;
	return 1;
}

int OpenPort( int localport )
{
	//Sets our socket with our local port

	ourSocket = SDLNet_UDP_Open( localport );
	if (ourSocket == NULL )
	{
		printf("coudln't open socket at port %d\n", localport);
		return 0; 
	}
	return 1;
}

int SetIPAndPort( char *ip, int port )
{
	//Set IP and port number with correct endianess
	if (SDLNet_ResolveHost( &serverIP, ip, port )  == -1 )
		return 0; 
	return 1; 
}

int Init( char *ip, int remotePort, int localPort, size_t packet_size)
{
	if (!InitSDL_Net())
		return 0;
	if ( !OpenPort(localPort))
		return 0;
	if ( !SetIPAndPort(ip, remotePort))
		return 0;
	if (!CreatePacket(packet_size))
		return 0;
	return 1;
}

//Send data. 
int Send(char *str)
{
	//Set the data
	//UDPPacket::data is an Uint8, which is similar to char*
	//This means we can't set it directly.

	//std::stringstreams let us add any data to it using << ( like std::cout ) 
	//We can extract any data from a std::stringstream using >> ( like std::cin )
	
	memcpy(packet->data, str, ft_strlen(str));
	packet->len = ft_strlen(str);

	//	SDLNet_UDP_Send returns number of packets sent. 0 means error
	
	if (SDLNet_UDP_Send(ourSocket, -1, packet) == 0)
	{
		printf("fail de send\n");
		return 0; 
	}
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
		printf("on recoit :%s", str);
		//printf("on a recu %s\n", packet->data);
	}
	else 
		printf(RED"on a recu R\n"RESET);
}

void	ft_process_client(char *serverName, char *local, char *remote, char *name)
{
	if (!Init(serverName, atoi(local), atoi(remote), 1000))
		exit(1);
	while (1)
	{
		if (!strcmp(name,"sender"))
			Send("des barres\n");
		else if (!strcmp(name, "receiver"))
			CheckForData();
		else
			ft_error("role undefined");
		usleep(10);
	}
}

/*
   void	ft_process_client(char *serverName, char *port, char *name)
   {
   const char *host;
   IPaddress serverIP;
   UDPsocket clientSocket;
   char *userInput = ft_strnew(0);
   t_message		*message;
   t_message		*received;

   message = (t_message *)malloc(sizeof(t_message));
   received = (t_message *)malloc(sizeof(t_message));

   memset(message->author, 0, AUTHOR_BUFF_SIZE);
   strcpy(message->author, name);
   message->from_server = 0;

   SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
   if (socketSet == NULL)
   ft_error("failed to allocate the socket set");

   int hostResolved = SDLNet_ResolveHost(&serverIP, serverName, atoi(port));
   if (hostResolved == -1)
   printf("Failed to resolve the server host name\n");
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

   clientSocket = SDLNet_UDP_Open(atoi(port));
   if (!clientSocket)
   ft_error("Failed to open socket to server");
   else
   {
   printf("Connection okay, about to read connection status from the server...");
   SDLNet_UDP_AddSocket(socketSet, clientSocket);
   int activeSockets = SDLNet_CheckSockets(socketSet, 5000);
   int gotServerResponse = SDLNet_SocketReady(clientSocket);
   if (gotServerResponse != 0)
   {
   int serverResponseByteCount = SDLNet_UDP_Recv(clientSocket, received, sizeof(*received));
   if (!strncmp(received->content, "successfully logged in\n", 23))
   shutdownClient = 0;
   ft_print_message(received);
   }
   else
   printf("No response from server\n");	
   }

   memset(message->content, 0, BUFF_SIZE);
   message->flags = INIT_USERNAME;
   SDLNet_UDP_Send(clientSocket, (void *)message, sizeof(*message));

   int wrotePrompt = 0;
   message->flags = DEFAULT;
   while (shutdownClient == 0)
   {
   if (wrotePrompt == 0)
wrotePrompt = 1;

set_conio_terminal_mode();
int status = kbHit();
reset_terminal_mode();

if (status != 0)
{
	set_conio_terminal_mode();
	char theChar = getch();
	reset_terminal_mode();
	if (theChar != 127) Backspace
		printf("%c", theChar);
	else
	{
		printf("%c %c", '\b', '\b');
		userInput = ft_strdel_char(userInput);
	}
	fflush(stdout);
	if ((int)theChar != 13)
	{
		if (theChar != 127)
			userInput = ft_strjoin_char(userInput, theChar);
	}
	else
	{
		printf("\n");
		memset(message->content, 0, BUFF_SIZE);
		strcpy(message->content, userInput);
		printf("on envoie %s de %s\n", message->content, message->author);
		if (SDLNet_UDP_Send(clientSocket, (void *)message, sizeof(*message)) < sizeof(*message))
			ft_error("Failed to send message");
		else
		{
			if ((!strcmp(userInput, "quit") || !strcmp(userInput, "exit") || !strcmp(userInput, "shutdown")))
				shutdownClient = 1;
			wrotePrompt = 0;
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
		int serverResponseByteCount = SDLNet_UDP_Recv(clientSocket, received, sizeof(*received));
		if (strcmp(received->content, "shutdown") == 0)
		{
			printf("Server is going down...\n");
			shutdownClient = 1;
		}
		else
			ft_print_message(received);
	}
	else
		printf("No response from the server...\n");
}
}
SDLNet_UDP_Close(clientSocket);
SDLNet_Quit();
reset_terminal_mode();
}
*/
