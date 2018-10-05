/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_fill_packet.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 00:07:32 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/06 00:07:32 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

size_t	memcpy_ret(void *dest, void *src, size_t size)
{
	memcpy(dest, src, size);
	return size;
}

int		ft_get_server_packet_size(t_server *server)
{
	char player_index = server->to_send.message->player_index;
	int i;
	int size;

	size = 0;
	size += sizeof(player_index);
	size += sizeof(server->to_send.message->message_number);
	size += sizeof(server->cm[player_index].changes.move_stack.nb_moves);
	size += sizeof(server->cm[player_index].changes.event_stack.nb_events);
	size += sizeof(server->nb_clients);
	i = 0;
	while (i < server->cm[player_index].changes.move_stack.nb_moves)
	{
		size += sizeof(server->cm[player_index].changes.move_stack.moves[i]);
		i++;
	}
	i = 0;
	while (i < server->cm[player_index].changes.event_stack.nb_events)
	{
		size += sizeof(server->cm[player_index].changes.event_stack.events[i]);
		i++;
	}
	i = 0;
	while (i < MAX_CLIENTS)
	{
		if (!server->cm[i].isfree)
			size += sizeof(server->game.players[i]);
		i++;
	}
	return (size);
}

int		ft_fill_packet_server(t_server *server)
{
	char player_index = server->to_send.message->player_index;
	int size;
	int i;
	
	size = 0;

	int	total_size;

	total_size = ft_get_server_packet_size(server);
	if (SDLNet_ResizePacket(server->to_send.packet, total_size) < total_size)
		ft_error("could not resize packet");

	//	if(1)
	//		printf("resized to:%d\n", total_size);
	Uint8 *data = server->to_send.packet->data;
	
	size += memcpy_ret(&(data[size]), &(player_index), sizeof(player_index));
	size += memcpy_ret(&(data[size]), &(server->to_send.message->message_number), sizeof(server->to_send.message->message_number));
	size += memcpy_ret(&(data[size]), &(server->cm[player_index].changes.move_stack.nb_moves), sizeof(server->cm[player_index].changes.move_stack.nb_moves));
	size += memcpy_ret(&(data[size]), &(server->cm[player_index].changes.event_stack.nb_events), sizeof(server->cm[player_index].changes.event_stack.nb_events));
	size += memcpy_ret(&(data[size]), &(server->nb_clients), sizeof(server->nb_clients));

	i = 0;
	while (i < server->cm[player_index].changes.move_stack.nb_moves)
	{
		size += memcpy_ret(&data[size], &(server->cm[player_index].changes.move_stack.moves[i]), sizeof(server->cm[player_index].changes.move_stack.moves[i]));
		i++;
	}
	i = 0;
	while (i < server->cm[player_index].changes.event_stack.nb_events)
	{
		size += memcpy_ret(&data[size], &(server->cm[player_index].changes.event_stack.events[i]), sizeof(server->cm[player_index].changes.event_stack.events[i]));
		i++;
	}
	i = 0;
	while (i < MAX_CLIENTS)
	{
		if (!server->cm[i].isfree)
		{
			size += memcpy_ret(&data[size], &(server->game.players[i]), sizeof(server->game.players[i]));
		}
		i++;
	}
	return (size);
}
