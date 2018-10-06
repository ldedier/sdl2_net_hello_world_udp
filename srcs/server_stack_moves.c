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

void	ft_stack_changes_color(t_server *server, t_vec2 pos, t_player *player)
{
	t_vec2 from;
	t_vec2 iter;
	double distance;
	
	distance = 0;
	from = ft_vec2_dest(pos, player->angle + M_PI / 2, player->radius);
	while (distance < player->radius * 2)
	{
		iter = ft_vec2_dest(from, player->angle - M_PI / 2, distance);
		if (ft_is_on_board(server->board.current_dim, (int)iter.x, (int)iter.y))
			server->board.map[(int)iter.y][(int)iter.x].parsed = player->index;
		distance = ft_fmin(distance + 0.5, player->radius * 2);
	}
	iter = ft_vec2_dest(from, player->angle - M_PI / 2, player->radius * 2);
	if (ft_is_on_board(server->board.current_dim, (int)iter.x, (int)iter.y))
		server->board.map[(int)iter.y][(int)iter.x].parsed = player->index;
}

void	ft_fill_move(t_move *move, t_player player, char is_rotate)
{
	move->origin = player.pos;
	move->player_index = player.index;
	move->color = player.color;
	move->player_angle = player.angle;
	move->speed = player.speed;
	move->radius = player.radius;
	move->is_rotate = is_rotate;
}

void	ft_stack_move_rotate(t_player player, t_move_stack *move_stack, t_vec2 center, double angle, char dir, double angle_init)
{
	t_rotate_move *rmove;

	if (move_stack->nb_moves == MAX_MOVES)
	{
		printf("too much moves\n");
		exit(1);
	}
	ft_fill_move(&(move_stack->moves[move_stack->nb_moves]), player, 1);
	rmove = &(move_stack->moves[move_stack->nb_moves].move_union.rmove);
	rmove->angle = angle;
	rmove->angle_init = angle_init;
	rmove->dir = dir;
	rmove->mobility = player.mobility;
	move_stack->nb_moves++;
}

void	ft_stack_move_forward(t_player player, t_move_stack *move_stack, double distance)
{
	t_forward_move *fmove;

	if (move_stack->nb_moves == MAX_MOVES)
	{
		printf("too much moves\n");
		exit(1);
	}
	ft_fill_move(&(move_stack->moves[move_stack->nb_moves]), player, 0);
	fmove = &(move_stack->moves[move_stack->nb_moves].move_union.fmove);
	fmove->distance = distance;
	move_stack->nb_moves++;
}
