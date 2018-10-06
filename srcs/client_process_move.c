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


char	get_line_intersection(t_segment sg1, t_segment sg2, t_vec2 *intersect)
{
	t_vec2 s1;
	t_vec2 s2;
	float s;
	float t;

	s1.x = sg1.b.x - sg1.a.x;
	s1.y = sg1.b.y - sg1.a.y;

	s2.x = sg2.b.x - sg2.a.x;
	s2.y = sg2.b.y - sg2.a.y;

	s = (-s1.y * (sg1.a.x - sg2.a.x) + s1.x * (sg1.a.y - sg2.a.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (sg1.a.y - sg2.a.y) - s2.y * (sg1.a.x - sg2.a.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		intersect->x = sg1.a.x + (t * s1.x);
		intersect->y = sg1.a.y + (t * s1.y);
		return 1;
	}
	return 0; // No collision
}

t_segment	ft_new_segment(double x1, double y1, double x2, double y2)
{
	t_segment res;

	res.a = ft_new_vec2(x1, y1);
	res.b = ft_new_vec2(x2, y2);
	return (res);
}



int		ft_aa_color(t_vec2 pos, double dir, t_vec2 center, int color)
{
	double line_dir = dir + M_PI / 2;
	t_aa aa;
	t_segment segment;
	t_ivec2 ivec;

	ivec = ft_vec2_to_ivec2(pos);
	segment.a = ft_vec2_dest(pos, line_dir, 2);
	segment.b = ft_vec2_dest(pos, line_dir + M_PI, 2);

	if ((aa.up = get_line_intersection(segment,
		ft_new_segment(ivec.x, ivec.y, ivec.x + 1, ivec.y),
		&aa.up_intersection)))
	{
	//	ft_print_vec2(aa.up_intersection);
	}
	if((aa.down = get_line_intersection(segment,
		ft_new_segment(ivec.x, ivec.y + 1, ivec.x + 1, ivec.y + 1),
		&aa.down_intersection)))
	{
	//	ft_print_vec2(aa.down_intersection);
	}
	if((aa.right = get_line_intersection(segment,
		ft_new_segment(ivec.x + 1, ivec.y, ivec.x + 1, ivec.y + 1),
		&aa.right_intersection)))
		{
	//	ft_print_vec2(aa.right_intersection);
	}
	if((aa.left = get_line_intersection(segment,
		ft_new_segment(ivec.x, ivec.y, ivec.x, ivec.y + 1),
		&aa.left_intersection)))
	{
		ft_print_vec2(aa.left_intersection);
	}	
	int nb_inter = aa.up +aa.left + aa.down + aa.right;
	if (nb_inter != 2 )
		printf("NB_INTER: %d\n", nb_inter);
	
//	t_vec2 third = ft_third_point(aa, ivec);

//	printf("up:%d\ndown:%d\nright:%d\nleft:%d\n\n", aa.up, aa.down, aa.right, aa.left);
	return 0xffff00;
}

void    ft_update_board(t_client *client, t_vec2 pos, t_move move)
{
	t_vec2 from;
	t_vec2 iter;
	double distance;
	t_color color;
	distance = 0;
	int first;
	first = 1;
	from = ft_vec2_dest(pos, move.player_angle + M_PI / 2, move.radius);
	while (distance < move.radius * 2)
	{
		color.color = move.color;
		iter = ft_vec2_dest(from, move.player_angle - M_PI / 2, distance);
		if (ft_is_on_board(client->board.current_dim, (int)iter.x, (int)iter.y)
			&& !client->board.map[(int)iter.y][(int)iter.x].color)
		{
			if (first)
			{
				client->board.map[(int)iter.y][(int)iter.x].color =
					ft_aa_color(iter, move.player_angle - M_PI / 2, pos, move.color);
			}
			else
				client->board.map[(int)iter.y][(int)iter.x].color = move.color;
		}
		first = 0;
		distance = ft_fmin(distance + 0.5, move.radius * 2);
	}
	iter = ft_vec2_dest(from, move.player_angle - M_PI / 2, move.radius * 2);
	if (ft_is_on_board(client->board.current_dim, (int)iter.x, (int)iter.y))
	{
		client->board.map[(int)iter.y][(int)iter.x].color =
		ft_aa_color(iter, move.player_angle + M_PI / 2, pos, move.color);
	//	client->board.map[(int)iter.y][(int)iter.x].color = move.color;
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
}
