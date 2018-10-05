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


void    ft_print_vec2(t_vec2 vec)
{
	printf("(%.2f, %.2f)\n", vec.x, vec.y);
}

void    ft_print_ivec2(t_ivec2 vec)
{
	printf("(%d, %d)\n", vec.x, vec.y);
}

void	ft_print_player(t_player player)
{
	printf("color:%d\n", player.color);
}

/*
   void	ft_print_move(t_move move)
   {
   printf("index: %d\n", move.player_index);
   printf("angle: %f\n", move.angle);
   printf("radius: %f\n", move.radius);
   ft_print_vec2(move.from);
   ft_print_vec2(move.to);
   }
 */
