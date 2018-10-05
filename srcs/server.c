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

void	ft_process_server(char *port)
{
	t_server	server;
	int			activity;

	srand(time(NULL));
	if (!ft_init_server(&server, ft_atoi(port)))
		exit(1);
	while (server.on)
	{
		activity = SDLNet_CheckSockets(server.socket_set, 1000);
		if (activity <= 0)
			ft_printf("no activity...\n");
		else
			ft_check_for_data(&server);
		ft_update_time_out(&server);
	}
}
