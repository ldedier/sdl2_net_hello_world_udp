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

void	ft_process_keys(t_client *client)
{
	SDL_Event event;

	//ft_bzero(client->to_send.message->keys, NB_KEYS);
	while (SDL_PollEvent(&event))
	{
		if ((event.type == SDL_QUIT) || (event.type == SDL_KEYDOWN &&
					event.key.keysym.sym == SDLK_ESCAPE))
			client->on = 0;
		else if (event.type == SDL_KEYDOWN && !event.key.repeat)
			ft_process_keydown(client, event.key.keysym.sym);
		else if (event.type == SDL_KEYUP)
			ft_process_keyup(client, event.key.keysym.sym);
		else if (event.type == SDL_MOUSEBUTTONDOWN && !event.key.repeat)
			ft_process_mousedown(client, event.key.keysym.sym);
		else if (event.type == SDL_MOUSEBUTTONUP)
			ft_process_mouseup(client, event.key.keysym.sym);
	}
	ft_process_keyboard(client, SDL_GetKeyboardState(NULL));
	//	ft_process_mouse(client, SDL_GetMouseState(NULL, NULL));
}

void	ft_process_client(char *serverName, char *port)
{
	t_client client;
	if (!ft_init_client(&client, serverName, atoi(port)))
		exit(1);
	if (!ft_connect_to_server(&client))
		exit(1);
	else
		ft_printf("successfully connected to server as player number #%d\n",
			client.received.message->player_index);
	client.framerate.ms_counter = SDL_GetTicks();
	while (client.on)
	{
		ft_process_delta_first(&(client.framerate));
		ft_process_keys(&client);
		ft_send_data(&client, REGULAR);
		ft_check_for_data_back(&client);
		//ft_render(&client);
		ft_print_fps(&(client.framerate));
		ft_process_delta(&(client.framerate));
		SDL_Delay(ft_fmax(0, (1000 / TICKRATE) - (client.framerate.delta)));
		//	SDL_Delay((1000 / TICKRATE));
	}
	ft_send_data(&client, DECONNEXION);
}
