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

//Send data. 
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
		received_message = (t_server_message *)client->received.packet->data;
		client->player_index = 	received_message->player_index;
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
	return (1);
}

void	ft_process_client(char *serverName, char *port)
{
	t_client client;

	if (!ft_init_client(&client, serverName, atoi(port)))
		exit(1);
	
	ft_send_data(&client);
	while (client.on)
	{
		ft_check_for_data_back(&client);
		printf("%d\n", client.player_index);
	}
}
