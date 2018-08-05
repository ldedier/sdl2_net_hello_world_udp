/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/26 21:09:19 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/05 19:50:00 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_error(char *str)
{
	ft_dprintf(2, "error: %s\n", str);
	exit(1);
}

int main(int argc, char **argv)
{
	if (SDLNet_Init() == -1)
		ft_error("could not init sdl2net");
	if (argc == 1)
		ft_error("invalid mode");
	if (!ft_strcmp(argv[1], "client"))
		ft_process_client(argv[2], argv[3], argv[4], argv[5]);
	else
		ft_error("invalid mode");
	return (0);
}
