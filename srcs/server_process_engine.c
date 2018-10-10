/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_process_engine.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 00:27:31 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/10 19:21:36 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_process_dash(t_server *server, t_player *player)
{
	if (player->vulnerability && rand() % 1000 > player->dash)
	{
		player->vulnerability = 0;
		player->vulnerability_countdown = 20;
	}
	else if (player->vulnerability_countdown > 0)
	{
//		printf("vulnerabilty: %f\n", player->vulnerability_countdown);
		player->vulnerability_countdown -= (player->speed);
		if (player->vulnerability_countdown <= 0)
			player->vulnerability = 1;
	}
}

void	ft_process_keys_server(t_server *server, t_client_message *message)
{
	if (message->keys[KEY_LEFT])
		ft_process_engine_rotate(server, &(server->game.players[message->player_index]), -1);
	else if (message->keys[KEY_RIGHT])
		ft_process_engine_rotate(server, &(server->game.players[message->player_index]), 1);
	else
		ft_process_engine_forward(server, &(server->game.players[message->player_index]));
}

void	ft_process_engine(t_server *server, t_client_message *message)
{
	if (!server->game.players[message->player_index].dead && server->nb_clients > 1)
	{
		ft_process_dash(server, &(server->game.players[message->player_index]));
		ft_process_keys_server(server, message);
	}
}
