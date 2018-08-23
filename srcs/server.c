/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/12 14:54:59 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/23 00:38:00 by ldedier          ###   ########.fr       */
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

void	ft_init_clients(t_client_manager cm[MAX_CLIENTS])
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		cm[i].isfree = 1;
		i++;
	}
}

int		ft_get_client_index(t_client_manager cm[MAX_CLIENTS])
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		if (cm[i].isfree)
		{
			cm[i].isfree = 0;
			return (i);
		}
		i++;
	}
	return (-1);
}

int ft_init_server(t_server *server, int port)
{
	if (!ft_open_port(&(server->socket), port))
		return (0);
	if (!ft_init_client_bundle(&(server->received), sizeof(t_client_message)))
		return (0);
	if (!ft_init_server_bundle(&(server->to_send), sizeof(t_server_message)))
		return (0);
	ft_init_clients(server->cm);
	server->on = 1;
	return (1);
}

void ft_check_for_data(t_server *server)
{
	char				*str;
	int					nb_packets;
	t_client_message	*received_message;
	if (SDLNet_UDP_Recv(server->socket, server->received.packet))
	{
		received_message = (t_client_message *)(server->received.packet->data);
		
		printf("on a recu un truc de rue\n");

		server->to_send.packet->address.port = server->received.packet->address.port;
		server->to_send.packet->address.host = server->received.packet->address.host;

		printf("player index received: %d\n", received_message->player_index);
		if (received_message->player_index == -1)
			server->to_send.message->player_index = ft_get_client_index(server->cm);
		else
			server->to_send.message->player_index = received_message->player_index;
		
		printf("%d\n", server->to_send.message->player_index);

		memcpy(server->to_send.packet->data, server->to_send.message, sizeof(t_server_message));
		server->to_send.packet->len = sizeof(t_server_message);
		
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
	t_server server;
	if (!ft_init_server(&server, atoi(port)))
		exit(1);
	while (server.on)
	{
		ft_check_for_data(&server);
	}
}
