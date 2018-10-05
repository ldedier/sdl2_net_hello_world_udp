/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_collide.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 00:20:37 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/06 00:20:37 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

int		ft_is_on_board(t_board board, int x, int y)
{
	return (x >= 0 && x < board.current_dim.x
			&& y >= 0 && y < board.current_dim.y);
}

int		ft_collide_board_diameter(t_board board, t_player player, t_vec2 pos, t_vec2 except)
{
	t_vec2 from;
	t_vec2 iter;
	double distance;
	t_ivec2 check;
	int i;

	distance = 0;
	i = 0;
	from = ft_vec2_dest(pos, player.angle + M_PI / 2, player.radius);
	while (distance < player.radius * 2)
	{
		iter = ft_vec2_dest(from, player.angle - M_PI / 2, distance);
		check = ft_vec2_to_ivec2(iter);
		if (!ft_is_on_board(board, check.x, check.y) || (board.map[check.y][check.x].player_index && board.map[check.y][check.x].parsed != player.index))
			return (1);
		distance = ft_fmin(distance + 0.5, player.radius * 2);
		i++;
	}
	iter = ft_vec2_dest(from, player.angle - M_PI / 2, player.radius * 2);
	check = ft_vec2_to_ivec2(iter);
	if (!ft_is_on_board(board, check.x, check.y) || (board.map[check.y][check.x].player_index && board.map[check.y][check.x].parsed != player.index))
		return (1);
	return (0);
}

int		ft_iz_okay(t_board board, t_vec2 vec, t_vec2 from, t_player player)
{
	return (vec.x >= 0 && vec.x < board.current_dim.x
			&& vec.y >= 0 && vec.y < board.current_dim.y
			&& !ft_collide_board_diameter(board, player, vec, from));
}
