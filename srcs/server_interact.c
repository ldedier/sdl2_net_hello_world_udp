/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_interact.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 00:02:48 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/06 00:02:48 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

int		ft_get_client_index(t_server *server)
{
	int i;

	i = 1;
	while (i < MAX_CLIENTS + 1)
	{
		if (server->cm[i].isfree)
		{
			server->cm[i].isfree = 0;
			server->game.players[i].dead = 0;
			server->game.players[i].index = i;
			server->cm[i].last_tick = SDL_GetTicks();
			server->cm[i].last_message_number = 0;
			server->nb_clients++;
			return (i);
		}
		i++;
	}
	return (-1);
}

int		ft_send_data_back(t_server *server)
{
	int		nb_packets;
	int		packet_size;

	server->to_send.packet->address.port = server->received.packet->address.port;
	server->to_send.packet->address.host = server->received.packet->address.host;

	packet_size = ft_fill_packet_server(server);
	server->to_send.packet->len = packet_size;

	if ((nb_packets = SDLNet_UDP_Send(server->socket, -1,
					server->to_send.packet)) == 0)
	{
		printf("fail de send\n");
		return (0);
	}
	else
		ft_reset_changes(&(server->cm[server->to_send.message->player_index].changes));
	server->to_send.message->message_number++;
	return (1);
}

int		ft_process_client_message(t_server *server, t_client_message *message)
{
	server->to_send.message->player_index = message->player_index;
	server->cm[message->player_index].last_tick = SDL_GetTicks();

	if (server->cm[message->player_index].last_message_number > message->message_number)
		printf("wrong packet order:\navant %u\napres %u\n\n", server->cm[message->player_index].last_message_number, message->message_number);
	else	
		server->cm[message->player_index].last_message_number = message->message_number;
	
	if (message->flag == REGULAR)
	{
		ft_process_engine(server, message);
		ft_send_data_back(server);
	}
	else if (message->flag == DECONNEXION)
		ft_deconnect(server, message->player_index);
	return (1);
}

void	ft_check_for_data(t_server *server)
{
	t_client_message	*received_message;

	if (SDLNet_UDP_Recv(server->socket, server->received.packet))
	{
		received_message = (t_client_message *)(server->received.packet->data);
//		printf("%d\n", received_message->player_index);
		if (received_message->player_index == -1)
		{
			server->to_send.message->player_index = ft_get_client_index(server);
			ft_send_data_back(server);
		}
		else
			ft_process_client_message(server, received_message);
	}
}
