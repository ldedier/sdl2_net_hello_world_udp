/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/05 23:24:48 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/05 23:24:48 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_reset_changes(t_changes *changes)
{
	changes->event_stack.nb_events = 0;
	changes->move_stack.nb_moves = 0;
}

void	ft_init_clients(t_client_manager cm[MAX_CLIENTS + 1])
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS + 1)
	{
		cm[i].isfree = 1;
		ft_reset_changes(&(cm[i].changes));
		i++;
	}
}

void	ft_init_player(t_player *player, int index)
{
	if (index == 1)
	{
		player->pos.x = BOARD_WIDTH / 3;
		player->pos.y = BOARD_HEIGHT / 3;
		player->angle = 0;
		player->color = 0xff0000;
	}
	else
	{
		player->pos.x = (2 * BOARD_WIDTH) / 3;
		player->pos.y = (2 * BOARD_HEIGHT) / 3;
		player->angle = M_PI;
		player->color = 0x0000ff;
	}
	player->dead = 1;
	player->speed = SPEED;
	player->radius = RADIUS;
	player->mobility = MOBILITY;
	player->dash = DASH;
	player->vulnerability = 1;
}

void	ft_init_game(t_game *game)
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS + 1)
	{
		ft_init_player(&(game->players[i]), i);
		i++;
	}
}

int ft_init_server(t_server *server, int port)
{
	if (!ft_open_port(&(server->socket), port))
		return (0);
	if (!ft_init_client_bundle(&(server->received), sizeof(t_client_message)))
		return (0);
	if (!ft_init_server_bundle(&(server->to_send), sizeof(t_server_message)))
		return (0);
	server->socket_set = SDLNet_AllocSocketSet(1);
	if (SDLNet_UDP_AddSocket(server->socket_set, server->socket) == -1)
		return (0);
	server->on = 1;
	server->to_send.message->message_number = 0;
	ft_init_clients(server->cm);
	ft_init_game(&(server->game));
	ft_init_board(&(server->board));	
	server->framerate.previous = SDL_GetPerformanceCounter();
	server->nb_clients = 0;
	return (1);
}
