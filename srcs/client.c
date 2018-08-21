/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/22 01:42:53 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

//Send data. 
int ft_send_data(t_client *client) //send from client to server
{
	int length;
	int nb_packets;
	/*
	length = ft_strlen(str);

	memset(message->data, '\0', BUFF_SIZE);
	memcpy(message->data, str, ft_strlen(str));
	packet->len = sizeof(t_message);
	memcpy(packet->data, message, sizeof(t_message));
*/
	printf("%u\n", client->to_send.packet->address.port);
	printf("%u\n", client->to_send.packet->address.host);
	client->to_send.packet->len = sizeof(t_message);
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
	if (SDLNet_UDP_Recv(client->socket, client->received.packet))
	{
	//	received_message = (t_message *)(received_packet->data);
	//	str = ft_strndup(received_message->data, ft_strlen(received_message->data));
		printf("on recoit du serveur: un truc\n");
	}
}


int ft_init_bundle_client(t_bundle *bundle, size_t size)
{
	if (!(bundle->packet = ft_create_packet(size)))
		return (0);
	if (!(bundle->message = malloc(size)))
		return(0);
	return (1);
}

int	ft_init_client(t_client *client, char *server_ip, int remote_port)
{
	if (!(ft_open_port(&(client->socket), 0)))
		return (0);
	if (!ft_resolve_ip_address(&(client->server_ip), server_ip, remote_port))
		return (0);
	if (!ft_init_bundle_client(&(client->received), sizeof(t_message)))
		return (0);
	if (!ft_init_bundle_client(&(client->to_send), sizeof(t_message)))
		return (0);
	client->to_send.packet->address.port = client->server_ip.port;
	client->to_send.packet->address.host = client->server_ip.host;
	return (1);
}

void	ft_process_client(char *serverName, char *port, char *message)
{
	t_client client;

	if (!ft_init_client(&client, serverName, atoi(port)))
		exit(1);
	while (1)
	{
		ft_send_data(&client);
		ft_check_for_data_back(&client);
	}
}
