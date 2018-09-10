/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curvefever.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 19:52:32 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/09 19:52:32 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_init_board(t_board *board)
{
	int i;
	int j;

	board->init_dim.x = BOARD_WIDTH;
	board->init_dim.y = BOARD_HEIGHT;
	board->current_dim = board->init_dim;
	
	i = 0;
	while (i < BOARD_HEIGHT)
	{
		j = 0;
		while (j < BOARD_WIDTH)
		{
			board->map[i][j] = 0;
			j++;
		}
		i++;
	}
}
