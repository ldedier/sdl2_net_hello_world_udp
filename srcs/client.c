/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 23:56:28 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/27 20:07:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"


void	ft_init_keys(char keys[NB_KEYS])
{
	int i;

	i = 0;
	while (i < NB_KEYS)
		keys[i++] = 0;
}

SDL_Texture	*ft_load_texture(t_sdl* sdl, char *filename)
{
	SDL_Texture	*res;

	if (!(res = IMG_LoadTexture(sdl->renderer, filename)))
	{
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_error("could not load the file");
	}
	return (res);
}

void	ft_load_textures(t_sdl* sdl)
{
	sdl->textures[0] = ft_load_texture(sdl, PATH"/resources/car.png");
	sdl->textures[1] = ft_load_texture(sdl, PATH"/resources/car2.png");
}

int	ft_init_client(t_client *client, char *server_ip, int remote_port)
{
	if (!(ft_open_port(&(client->socket), 0))) // take the first available port
		return (0);
	if (!ft_resolve_ip_address(&(client->server_ip), server_ip, remote_port))
		return (0);
	if (!ft_init_server_bundle(&(client->received), sizeof(t_server_message)))
		return (0);
	if (!ft_init_client_bundle(&(client->to_send), sizeof(t_client_message)))
		return (0);
	if (!ft_init_sdl("UDP 2 RUE", &(client->sdl)))
		return (0);
	ft_load_textures(&(client->sdl)); // to protect
	ft_init_keys(client->to_send.message->keys);
	ft_init_board(&(client->board));
	client->to_send.packet->address.port = client->server_ip.port;
	client->to_send.packet->address.host = client->server_ip.host;
	client->received.message->player_index = -1;
	client->on = 1;
	client->framerate.previous = SDL_GetPerformanceCounter();
	client->to_send.message->message_number = 0;
	client->socket_set = SDLNet_AllocSocketSet(1);
	if (SDLNet_UDP_AddSocket(client->socket_set, client->socket) == -1)
		return (0);
	return (1);
}

int		ft_receive_connection_packet(t_client *client)
{
	if (SDLNet_UDP_Recv(client->socket, client->received.packet))
	{
		client->last_tick = SDL_GetTicks();
	
		Uint8 *int_8_data = client->received.packet->data;
		Uint32 *int_32_data = (Uint32 *)(&int_8_data[1]);
	//	Uint32 *int_32i_data = (Uint32 *)client->received.packet->data;
	
		printf("%d\n", int_8_data[0]); // player_index
		printf("%d\n", int_32_data[0]); // message_number
		printf("%d\n", int_32_data[1]); // nb_colors
		printf("%d\n", int_32_data[2]); // nb_events
		printf("%d\n", int_32_data[3]); // nb_players	
	
		client->last_message_number = int_32_data[0];
		printf("PLAYER INDEX %d\n", int_8_data[0]);
		return (client->received.message->player_index = int_8_data[0]);
	}
	return (-1);
}

int ft_send_data(t_client *client, int flag) //send from client to server
{
	int length;
	int nb_packets;

	client->to_send.message->player_index = client->received.message->player_index;
	client->to_send.message->flag = flag;
	memcpy(client->to_send.packet->data, client->to_send.message, sizeof(t_client_message));
	client->to_send.packet->len = sizeof(t_client_message);

	if ((nb_packets = SDLNet_UDP_Send(client->socket, -1,
					client->to_send.packet)) == 0)
	{
		printf("fail de send\n");
		return 0;
	}
	client->to_send.message->message_number++;
	return (1);
}

int		ft_connect_to_server(t_client *client)
{
	int		active_socket;
	int 	nb_tries;

	nb_tries = 0;
	while (nb_tries < CONNECTION_RETRIES_LIMIT)
	{
		ft_send_data(client, REGULAR);
		ft_printf("trying to connect to server...\n");
		if ((active_socket = SDLNet_CheckSockets(client->socket_set, 1500)))
		{
			if (ft_receive_connection_packet(client) == -1)
			{
				printf("server is full\n");
				return (0);
			}
			else
				return (1);
		}
		nb_tries++;
	}
	ft_printf("failed to connect to the server after %d retries\n",
			CONNECTION_RETRIES_LIMIT);
	return (0);
}

void	ft_render_players(t_client *client)
{
	t_game  game;
	int i;
	SDL_Rect rect;
	SDL_Point center;

	rect.w = 300;
	rect.h = 150;
	
	i = 0;

	while (i < MAX_CLIENTS)
	{
		if (!(game.players[i].dead))
		{
			rect.x = game.players[i].pos.x;
			rect.y = game.players[i].pos.y;

			center.x = rect.w / 2;
			center.y = rect.h / 2;

			if (i % 2)
				SDL_SetRenderDrawColor(client->sdl.renderer, 0, 0, 255, 255);
			else
				SDL_SetRenderDrawColor(client->sdl.renderer, 255, 0, 0, 255);
			SDL_RenderCopyEx(client->sdl.renderer, client->sdl.textures[0], NULL, &rect, game.players[i].angle * (180.0 / M_PI), &center, SDL_FLIP_NONE);
		//	SDL_RenderFillRect(client->sdl.renderer, &rect);
		}
		i++;
	}
}

/*
void	ft_render_board(t_client *client)
{
	int i;
	int j;
	int index;

	t_color c;
	i = 0;
	while (i < BOARD_HEIGHT)
	{
		j = 0;
		while (j < BOARD_WIDTH)
		{
			if((index = client->board.map[i][j]))
			{
				c.color = client->response.players_data[index - 1].color;
		//		printf("%.6x\n", c.color);
				SDL_SetRenderDrawColor(client->sdl.renderer, c.argb.r, c.argb.g, c.argb.b, 255);
				SDL_RenderDrawPoint(client->sdl.renderer, j, i);
			}
			j++;
		}
		i++;
	}
}
*/

void	ft_render_board(t_client *client)
{
	int i;
	int j;
	int index;

	int *pixels = (int *)client->sdl.surface->pixels;
	int surface_width =  client->sdl.surface->w;
	t_color c;
	i = 0;
	while (i < BOARD_HEIGHT)
	{
		j = 0;
		while (j < BOARD_WIDTH)
		{
			if ((index = client->board.map[i][j]))
			{
				pixels[i * surface_width + j] = client->response.players_data[index - 1].color;
			}
			j++;
		}
		i++;
	}
	client->sdl.texture = SDL_CreateTextureFromSurface(client->sdl.renderer, client->sdl.surface);
	SDL_RenderCopy(client->sdl.renderer, client->sdl.texture, NULL, NULL);
	SDL_DestroyTexture(client->sdl.texture);
}

void    ft_render(t_client *client)
{
	SDL_SetRenderDrawColor(client->sdl.renderer, 0, 0, 0, 255);
	SDL_RenderClear(client->sdl.renderer);
	ft_render_board(client);
//	ft_render_players(client);
	SDL_RenderPresent(client->sdl.renderer);
	client->framerate.fps_counter++;
}

void	ft_print_colored(t_colored colored)
{
	printf("(%d, %d), %d\n", colored.pos.x, colored.pos.y, colored.player_index);
}

void	ft_print_player(t_player player)
{
	printf("color:%d\n", player.color);
}

void	ft_receive_data_back(t_client *client)
{
	Uint8 *int_8_data = client->received.packet->data;
	Uint32 *int_32_data = (Uint32 *)(&int_8_data[1]);

	client->response.nb_colored = int_32_data[1];
	client->response.nb_events = int_32_data[2];
	client->response.nb_players = int_32_data[3];
	
	size_t header_size = 
		sizeof(client->received.message->player_index) +
		sizeof(client->last_message_number) +
		sizeof(client->response.nb_colored) +
		sizeof(client->response.nb_events) +
		sizeof(client->response.nb_players);

	int		events_index = header_size + client->response.nb_colored * sizeof(t_colored);
	
	client->response.colored_data = (t_colored *)(&int_8_data[header_size]);
	client->response.events_data = (char *)(&int_8_data[events_index]);
	client->response.players_data = (t_player *)(&int_8_data[events_index + client->response.nb_events * sizeof(char)]);

	int i = 0;
	i = 0;

//	while(i < 1)
//		ft_print_player(client->response.players_data[i++]);

//	printf("nb_colors %d\n", int_32_data[1]); // nb_colors
//	printf("nb_events %d\n", int_32_data[2]); // nb_events
//	printf("nb_players %d\n", int_32_data[3]); // nb_players
	client->last_message_number = int_32_data[0];
}

void	ft_process_data_board(t_client *client)
{
	int i;
	t_colored colored;
	i = 0;

	while (i < client->response.nb_colored)
	{
		colored = client->response.colored_data[i];
		client->board.map[colored.pos.y][colored.pos.x] = colored.player_index + 1;
		i++;
	}
}

void	ft_process_data_back(t_client *client)
{
	ft_process_data_board(client);
}

void	ft_check_for_data_back(t_client *client)
{
	UDPpacket packet;
	int activity;

	if ((activity = SDLNet_CheckSockets(client->socket_set, TIMEOUT_THRESHOLD)))
	{
		SDLNet_UDP_Recv(client->socket, client->received.packet);
		ft_receive_data_back(client);
		ft_process_data_back(client);
		if (client->received.message->message_number < client->last_message_number)
			printf("wrong packet order:\navant %u\napres %u\n\n", client->last_message_number, client->received.message->message_number);
		else	
			client->last_message_number = client->received.message->message_number;
		ft_render(client);
	}
	else
	{
		printf("no response from server for too long.\n");
		client->on = 0;
	}
}

void	ft_process_keys(t_client *client)
{
	SDL_Event event;

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
	ft_process_mouse(client, SDL_GetMouseState(NULL, NULL));
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
				client.received.message->player_index + 1);
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
