/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/12 14:54:59 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/29 19:20:01 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_apply_color_changes(t_server *server, char player_index)
{
	int i;
	int j;

	i = 0;
	while (i < server->board.init_dim.y)
	{
		j = 0;
		while (j < server->board.init_dim.x)
		{
			if (server->board.map[i][j].parsed == player_index)
			{
				server->board.map[i][j].parsed = 0;
				server->board.map[i][j].player_index = player_index;
			}
			j++;
		}
		i++;
	}
}

t_vec2	ft_get_rotation_center(t_player *player, int dir)
{
	return ft_vec2_dest(player->pos, player->angle + dir * (M_PI / 2),
			player->radius + player->mobility);
}

void	ft_process_engine_rotate(t_server *server, t_player *player, int dir)
{
	t_vec2 to;
	t_vec2 from;
	t_vec2 iter;
	t_vec2 res;
	int i;
	t_vec2 center;

	center = ft_get_rotation_center(player, dir);
	from = player->pos;
	iter = from;
	t_vec2 previous = from;
	double angle_init = player->angle;
	double angle_to = player->speed / (player->radius + player->mobility);
	double angle_iter;
	angle_iter = 0;
	double counter_angle = player->angle + dir * (M_PI / 2) + M_PI;

	res = ft_vec2_dest(center, counter_angle + (angle_to * dir), player->radius + player->mobility);
	if (player->vulnerability)
	{
		while (angle_iter < angle_to)
		{
			iter = ft_vec2_dest(center, counter_angle + (angle_iter * dir), player->radius + player->mobility);
			player->angle = angle_init + angle_iter * dir;
			if (ft_iz_okay(server->board, iter, *player))
				ft_stack_changes_color(server, iter, player);
			else
			{
				player->dead = 1;
				res = iter;
				printf("dead\n");
				break;
			}
			angle_iter = ft_fmin(angle_to, angle_iter +  M_PI / 256);
		}
		if (!player->dead)
		{
			iter = ft_vec2_dest(center, counter_angle + (angle_to * dir), player->radius + player->mobility);
			player->angle = angle_init + angle_to * dir;
			if (ft_iz_okay(server->board, iter, *player))
				ft_stack_changes_color(server, iter, player);
			else
			{
				player->dead = 1;
				res = iter;
				printf("dead\n");
			}
		}
		i = 1;
		while (i < MAX_CLIENTS + 1)
		{
			if (!server->cm[i].isfree)
				ft_stack_move_rotate(*player, &(server->cm[i].changes.move_stack), center, angle_iter, dir, angle_init);
			i++;
		}
	ft_apply_color_changes(server, player->index);
	ft_stack_changes_color(server, iter, player);
	}
	player->angle = angle_init + angle_to * dir;
	player->pos = res;
}

void	ft_process_engine_forward(t_server *server, t_player *player)
{
	t_vec2 to;
	t_vec2 from;
	t_vec2 iter;
	t_vec2 res;
	int i;

	from = player->pos;
	to = ft_vec2_dest(from, player->angle, player->speed);
	res = to;
	iter = from;
	double distance;

	if (player->vulnerability)
	{
		distance = 0;
		while (distance < player->speed)
		{
			iter = ft_vec2_dest(from, player->angle, distance);
			if (ft_iz_okay(server->board, iter, *player))
			{
				ft_stack_changes_color(server, iter, player);
			}
			else
			{
				player->dead = 1;
				res = iter;
				printf("dead\n");
				break;
			}
			distance = ft_fmin(player->speed, distance + 1);
		}
		if (!player->dead)
		{
			iter = ft_vec2_dest(from, player->angle, player->speed);
			if (ft_iz_okay(server->board, iter, *player))
				ft_stack_changes_color(server, iter, player);
			else
			{
				player->dead = 1;
				res = iter;
				printf("dead\n");
			}
		}
		i = 1;
		while (i < MAX_CLIENTS + 1)
		{
			if (!server->cm[i].isfree)
				ft_stack_move_forward(*player, &(server->cm[i].changes.move_stack), distance);
			i++;
		}
		ft_apply_color_changes(server, player->index);
		ft_stack_changes_color(server, iter, player);
	//	if (ft_is_on_board(server->board, (int)iter.x), (int)iter.y)
	}
	player->pos = res;
}
