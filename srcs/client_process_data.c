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

void	ft_process_data_board(t_client *client)
{
	int i;
	t_move move;
	i = 0;

	while (i < client->response.nb_moves)
	{
		move = client->response.move_data[i];
		ft_process_move(client, move);
		i++;
	}
}

void	ft_process_data_back(t_client *client)
{
	ft_process_data_board(client);
}
