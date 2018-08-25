/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/23 00:37:01 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

int ft_send_data(t_client *client) //send from client to server
{
	int length;
	int nb_packets;
	
	client->to_send.message->player_index = client->player_index;
	memcpy(client->to_send.packet->data, client->to_send.message, sizeof(t_client_message));
	client->to_send.packet->len = sizeof(t_client_message);

	if ((nb_packets = SDLNet_UDP_Send(client->socket, -1,
			client->to_send.packet)) == 0)
	{
		printf("fail de send\n");
		return 0;
	}
	return (1);
}

void ft_check_for_data_back(t_client *client)
{
	t_server_message *received_message;

	if (SDLNet_UDP_Recv(client->socket, client->received.packet))
	{
		client->last_tick = SDL_GetTicks();
		received_message = (t_server_message *)client->received.packet->data;
		client->player_index = 	received_message->player_index;
	}
	else
	{
		if (SDL_GetTicks() - client->last_tick > TIMEOUT_THRESHOLD)
		{
			printf("no response from server for too long.\n");
			client->on = 0;
		}
	}
}

int	ft_init_client(t_client *client, char *server_ip, int remote_port)
{
	if (!(ft_open_port(&(client->socket), 0))) // take the first available port
		return (0);
	if (!ft_resolve_ip_address(&(client->server_ip), server_ip, remote_port))
		return (0);
	if (!ft_init_server_bundle(&(client->received), sizeof(t_server_message)))
		return (0);
	if (!ft_init_client_bundle(&(client->to_send), sizeof(t_client_message)))
		return (0);
	client->to_send.packet->address.port = client->server_ip.port;
	client->to_send.packet->address.host = client->server_ip.host;
	client->player_index = -1;
	client->on = 1;
	client->socket_set = SDLNet_AllocSocketSet(1);
	if (SDLNet_UDP_AddSocket(client->socket_set, client->socket) == -1)
		return (0);
	return (1);
}

int		ft_receive_connection_packet(t_client *client)
{
	t_server_message *received_message;

	if (SDLNet_UDP_Recv(client->socket, client->received.packet))
	{
		client->last_tick = SDL_GetTicks();
		received_message = (t_server_message *)client->received.packet->data;
		return ((client->player_index = received_message->player_index));
	}
	return (-1);
}

int			ft_connect_to_server(t_client *client)
{
	int		active_socket;
	int 	nb_tries;

	nb_tries = 0;
	while (nb_tries < CONNECTION_RETRIES)
	{
		ft_send_data(client);
		ft_printf("trying to connect to server...\n");
		if ((active_socket = SDLNet_CheckSockets(client->socket_set, 2000)))
		{
			if (ft_receive_connection_packet(client) == -1)
			{
				printf("server is full\n");
				return (0);
			}
			else
				return (1);
		}
		nb_tries++;
	}
	ft_printf("failed to connect to the server after %d retries\n",
		CONNECTION_RETRIES);
	return (0);
}

void	ft_process_client(char *serverName, char *port)
{
	t_client client;

	if (!ft_init_client(&client, serverName, atoi(port)))
		exit(1);
	if (!ft_connect_to_server(&client))
		client.on = 0;
	else
		ft_printf("successfully connected to server as player number #%d\n",
			client.player_index + 1);
	while (client.on)
	{
		ft_send_data(&client);
		ft_check_for_data_back(&client);
		SDL_Delay(1000 / TICKRATE);
	}
}
