/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/12 14:54:59 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/22 01:43:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

/*
int SendFromServer(char *str)
{
	int length;
	int nb_packets;

	memset(message->data, '\0', BUFF_SIZE);
	memcpy(message->data, str, ft_strlen(str));
	memcpy(packet->data, message, sizeof(t_message));
	
	packet->len = sizeof(t_message);
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
*/

void	ft_init_sockets(t_client_socket clientSockets[MAX_CLIENTS])
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		clientSockets[i].socket = NULL;
		clientSockets[i].isfree = 1;
		i++;
	}
}

int	ft_init_bundle(t_bundle *bundle, size_t size)
{
	if (!(bundle->packet = ft_create_packet(size)))
		return (0);
	if (!(bundle->message = malloc(size)))
		return(0);
	return (1);
}

int ft_init_server(t_server *server, int port)
{
	if (!ft_open_port(&(server->socket), port))
		return (0);
	if (!ft_init_bundle(&(server->received),sizeof(t_message)))
		return (0);
	if (!ft_init_bundle(&(server->to_send),sizeof(t_message)))
		return (0);
	ft_init_sockets(server->client_sockets);
	server->on = 1;
	return (1);
}

void ft_check_for_data(t_server *server)
{
	char		*str;
	UDPpacket	*received_packet;
	t_message	*received_message;
	int			nb_packets;

	received_packet = server->received.packet;
	received_message = server->received.message;
	if (SDLNet_UDP_Recv(server->socket, received_packet))
	{
		received_message = (t_message *)(received_packet->data);
		printf("on a recu un truc de rue\n");
		
		server->to_send.packet->address.port = received_packet->address.port;
		server->to_send.packet->address.host = received_packet->address.host;
		server->to_send.packet->len = sizeof(t_message);
		
		if ((nb_packets = SDLNet_UDP_Send(server->socket, -1,
				server->to_send.packet)) == 0)
		{
			printf("fail de send\n");
		}
		else
		{
			printf("successfully sent %d\n", nb_packets);
		}
	}
}

void	ft_process_server(char *port)
{
	int stop;
	t_client_socket clientSockets[MAX_CLIENTS];
	t_server server;
	if (!ft_init_server(&server, atoi(port)))
		exit(1);
	while (server.on)
	{
		ft_check_for_data(&server);
	}
}
