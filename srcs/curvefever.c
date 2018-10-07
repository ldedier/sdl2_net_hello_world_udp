/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curvefever.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 19:52:32 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/28 09:42:23 by ldedier          ###   ########.fr       */
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
	
	board->map = malloc(BOARD_HEIGHT * sizeof(t_tile *));
	i = 0;
	while (i < BOARD_HEIGHT)
	{
		board->map[i] = malloc(BOARD_WIDTH * sizeof(t_tile));
		j = 0;
		while (j < BOARD_WIDTH)
		{
			board->map[i][j].parsed = 0;
			board->map[i][j].player_index = 0;
			j++;
		}
		i++;
	}
}

void	ft_init_client_board(t_client_board *board)
{
	int i;
	int j;

	board->init_dim.x = BOARD_WIDTH;
	board->init_dim.y = BOARD_HEIGHT;
	board->current_dim = board->init_dim;
	
	board->map = malloc(BOARD_HEIGHT * sizeof(t_client_tile *));
	i = 0;
	while (i < BOARD_HEIGHT)
	{
		board->map[i] = malloc(BOARD_WIDTH * sizeof(t_client_tile));
		j = 0;
		while (j < BOARD_WIDTH)
		{
			board->map[i][j].color = 0;
			board->map[i][j].area = 0;
			board->map[i][j].parsed = 0;
			j++;
		}
		i++;
	}
}

void	ft_clear_board(t_board *board)
{
	int i;
	int j;

	i = 0;
	while (i < board->init_dim.y)
	{
		j = 0;
		while (j < board->init_dim.x)
		{
			board->map[i][j].parsed = 0;
			board->map[i][j].player_index = 0;
			j++;
		}
		i++;
	}
}
