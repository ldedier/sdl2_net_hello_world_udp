/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/05 22:48:41 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/05 22:48:41 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

int		ft_receive_connection_packet(t_client *client)
{
	if (SDLNet_UDP_Recv(client->socket, client->received.packet))
	{
		client->last_tick = SDL_GetTicks();

		Uint8 *int_8_data = client->received.packet->data;
		Uint32 *int_32_data = (Uint32 *)(&int_8_data[1]);
		//	Uint32 *int_32i_data = (Uint32 *)client->received.packet->data;

		printf("%d\n", int_8_data[0]); // player_index
		printf("%d\n", int_32_data[0]); // message_number
		printf("%d\n", int_32_data[1]); // nb_moves
		printf("%d\n", int_32_data[2]); // nb_events
		printf("%d\n", int_32_data[3]); // nb_players	

		client->last_message_number = int_32_data[0];
		printf("PLAYER INDEX %d\n", int_8_data[0]);
		return (client->received.message->player_index = int_8_data[0]);
	}
	return (-1);
}

int		ft_connect_to_server(t_client *client)
{
	int		active_socket;
	int 	nb_tries;

	nb_tries = 0;
	while (nb_tries < CONNECTION_RETRIES_LIMIT)
	{
		ft_send_data(client, REGULAR);
		ft_printf("trying to connect to server...\n");
		if ((active_socket = SDLNet_CheckSockets(client->socket_set, 1500)))
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
			CONNECTION_RETRIES_LIMIT);
	return (0);
}
