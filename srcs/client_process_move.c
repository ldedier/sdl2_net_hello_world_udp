/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_process_move.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 17:47:06 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/06 17:47:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_init_parse_client_board(t_client_board *board)
{
	int i;
	int j;

	i = 0;
	while (i < board->current_dim.y)
	{
		j = 0;
		while(j < board->current_dim.x)
		{
			board->map[i][j].parsed = 0;
			j++;
		}
		i++;
	}
}

void    ft_update_board(t_client *client, t_vec2 pos, t_move move)
{
	t_vec2 from;
	t_vec2 iter;
	double distance;
	distance = 0;
	int first;
	first = 1;
	from = ft_vec2_dest(pos, move.player_angle + M_PI / 2, move.radius);

	iter = ft_vec2_dest(from, move.player_angle - M_PI / 2, move.radius * 2);
	if (ft_is_on_board(client->board.current_dim, (int)iter.x, (int)iter.y)
		&& !client->board.map[(int)iter.y][(int)iter.x].color)
	{
		client->board.map[(int)iter.y][(int)iter.x].color =
			ft_aa_color(iter, move.player_angle - M_PI / 2, pos, move.color);
		//	client->board.map[(int)iter.y][(int)iter.x].color = move.color;
	}
	while (distance < move.radius * 2)
	{
		iter = ft_vec2_dest(from, move.player_angle - M_PI / 2, distance);
		if (ft_is_on_board(client->board.current_dim, (int)iter.x, (int)iter.y)
				&& !client->board.map[(int)iter.y][(int)iter.x].color)
		{
			if (first)
			{
				client->board.map[(int)iter.y][(int)iter.x].color =
					ft_aa_color(iter, move.player_angle + M_PI / 2, pos, move.color);
			}
			else
				client->board.map[(int)iter.y][(int)iter.x].color = move.color;
		}
		first = 0;
		distance = ft_fmin(distance + 0.25, move.radius * 2);
	}
}

void	ft_process_move_rotate(t_client *client, t_move move)
{
	t_rotate_move rmove = move.move_union.rmove;
	t_vec2 iter;

	t_vec2 center;
	center = ft_vec2_dest(move.origin, rmove.angle_init + rmove.dir * (M_PI / 2),  move.radius + rmove.mobility);
	double angle_to = move.speed / (move.radius + rmove.mobility);
	double angle_init = rmove.angle_init;
	double angle_iter = 0;
	double counter_angle = rmove.angle_init + rmove.dir * (M_PI / 2) + M_PI;
	while (angle_iter < rmove.angle)
	{
		iter = ft_vec2_dest(center, counter_angle + (angle_iter * rmove.dir), move.radius + rmove.mobility);
		move.player_angle = rmove.angle_init + angle_iter * rmove.dir;
		ft_update_board(client, iter, move);
		angle_iter = ft_fmin(rmove.angle, angle_iter +  M_PI / (128 * ((move.radius + rmove.mobility) / 10)));
	}
	iter = ft_vec2_dest(center, counter_angle + (rmove.angle * rmove.dir), move.radius + rmove.mobility);
	ft_update_board(client, iter, move);

	//	client->board.map[(int)rmove.center.y][(int)rmove.center.x].player_index = move.player_index;
}

void	ft_process_move_forward(t_client *client, t_move move)
{
	t_forward_move fmove = move.move_union.fmove;
	t_vec2 iter = move.origin;
	double distance;

	distance = 0;
	while (distance < fmove.distance)
	{
		iter = ft_vec2_dest(move.origin, move.player_angle, distance);
		ft_update_board(client, iter, move);
		distance = ft_fmin(fmove.distance, distance + 1);
	}
	iter = ft_vec2_dest(move.origin, move.player_angle, fmove.distance);
	ft_update_board(client, iter, move);
}

void	ft_process_move(t_client *client, t_move move)
{
	if (move.is_rotate)
		ft_process_move_rotate(client, move);
	else
		ft_process_move_forward(client, move);
//	ft_init_parse_client_board(&(client->board));
}
