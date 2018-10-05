/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_disconnect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 00:16:17 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/06 00:16:17 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_deconnect(t_server *server, char player_index)
{
	if (!server->cm[player_index].isfree)
	{
		server->cm[player_index].isfree = 1;
		ft_init_player(&(server->game.players[player_index]), player_index);
		if (--server->nb_clients == 0)
			ft_clear_board(&(server->board));
	}
}

void	ft_update_time_out(t_server *server)
{
	Uint32	ticks;
	int		i;

	i = 0;
	ticks = SDL_GetTicks();
	while (i < MAX_CLIENTS)
	{
		if (!server->cm[i].isfree && ticks - server->cm[i].last_tick > TIMEOUT_THRESHOLD)
		{
			printf("player %d timed out!\n", i);
			ft_deconnect(server, i);
		}
		i++;
	}
}
