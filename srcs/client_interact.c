/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/29 21:31:54 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

int ft_send_data(t_client *client, int flag) //send from client to server
{
	int length;
	int nb_packets;

	client->to_send.message->player_index = client->received.message->player_index;
	client->to_send.message->flag = flag;
	memcpy(client->to_send.packet->data, client->to_send.message, sizeof(t_client_message));
	client->to_send.packet->len = sizeof(t_client_message);

	if ((nb_packets = SDLNet_UDP_Send(client->socket, -1,
					client->to_send.packet)) == 0)
	{
		printf("fail de send\n");
		return 0;
	}
	client->to_send.message->message_number++;
	return (1);
}

void	ft_receive_data_back(t_client *client)
{
	Uint8 *int_8_data = client->received.packet->data;
	Uint32 *int_32_data = (Uint32 *)(&int_8_data[1]);

	client->response.nb_moves = int_32_data[1];
	client->response.nb_events = int_32_data[2];
	client->response.nb_players = int_32_data[3];

	size_t header_size =
		sizeof(client->received.message->player_index) +
		sizeof(client->last_message_number) +
		sizeof(client->response.nb_moves) +
		sizeof(client->response.nb_events) +
		sizeof(client->response.nb_players);

	int		events_index = header_size + client->response.nb_moves * sizeof(t_move);

	client->response.move_data = (t_move *)(&int_8_data[header_size]);
	client->response.events_data = (char *)(&int_8_data[events_index]);
	client->response.players_data = (t_player *)(&int_8_data[events_index + client->response.nb_events * sizeof(char)]);

//	printf("nb_moves %d\n", int_32_data[1]); // nb_moves
//	printf("nb_events %d\n", int_32_data[2]); // nb_events
//	printf("nb_players %d\n", int_32_data[3]); // nb_players
	client->last_message_number = int_32_data[0];
}

void	ft_check_for_data_back(t_client *client)
{
	UDPpacket packet;
	int activity;

	if ((activity = SDLNet_CheckSockets(client->socket_set, TIMEOUT_THRESHOLD)))
	{
		SDLNet_UDP_Recv(client->socket, client->received.packet);
		ft_receive_data_back(client);
		ft_process_data_back(client);
		if (client->received.message->message_number < client->last_message_number)
			printf("wrong packet order:\navant %u\napres %u\n\n", client->last_message_number, client->received.message->message_number);
		else	
			client->last_message_number = client->received.message->message_number;
		ft_render(client);
	}
	else
	{
		printf("no response from server for too long.\n");
		client->on = 0;
	}
}
