/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/12 14:54:59 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/23 00:38:00 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_init_clients(t_client_manager cm[MAX_CLIENTS])
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		cm[i].isfree = 1;
		i++;
	}
}

void	ft_init_player(t_player *player)
{
	player->pos.x = 0;
	player->pos.y = 0;
	player->dead = 1;
	player->angle = 0;
}

void	ft_init_game(t_game *game)
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		ft_init_player(&(game->players[i]));
		i++;
	}
}

int ft_init_server(t_server *server, int port)
{
	if (!ft_open_port(&(server->socket), port))
		return (0);
	if (!ft_init_client_bundle(&(server->received), sizeof(t_client_message)))
		return (0);
	if (!ft_init_server_bundle(&(server->to_send), sizeof(t_server_message)))
		return (0);
	server->socket_set = SDLNet_AllocSocketSet(1);
	if (SDLNet_UDP_AddSocket(server->socket_set, server->socket) == -1)
		return (0);
	server->on = 1;
	server->to_send.message->message_number = 0;
	ft_init_clients(server->cm);
	ft_init_game(&(server->game));
	server->framerate.previous = SDL_GetPerformanceCounter();
	server->nb_clients = 0;
	return (1);
}

int		ft_get_client_index(t_server *server)
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		if (server->cm[i].isfree)
		{
			server->cm[i].isfree = 0;
			server->game.players[i].dead = 0;
			server->cm[i].last_tick = SDL_GetTicks();
			server->cm[i].last_message_number = 0;
			server->nb_clients++;
			return (i);
		}
		i++;
	}
	return (-1);
}

/*
void	ft_process_engine(t_server *server, t_client_message *message)
{
	server->game.players[message->player_index].pos.x +=
		(message->keys[KEY_RIGHT] - message->keys[KEY_LEFT]) * SPEED;
	server->game.players[message->player_index].pos.y +=
		(message->keys[KEY_DOWN] - message->keys[KEY_UP]) * SPEED;
}
*/

void	ft_process_engine(t_server *server, t_client_message *message)
{
	double *angle = &(server->game.players[message->player_index].angle);

	if (message->keys[KEY_UP])
		*angle += (message->keys[KEY_RIGHT] - message->keys[KEY_LEFT]) * (M_PI / 40.0);
	if (message->keys[KEY_DOWN])
		*angle -= (message->keys[KEY_RIGHT] - message->keys[KEY_LEFT]) * (M_PI / 40.0);
	server->game.players[message->player_index].pos.x -=
		(message->keys[KEY_UP] - message->keys[KEY_DOWN]) * cos(*angle) * SPEED;
	server->game.players[message->player_index].pos.y -=
		(message->keys[KEY_UP] - message->keys[KEY_DOWN]) * sin(*angle) * SPEED;
}

int		ft_send_data_back(t_server *server)
{
	int		nb_packets;
	
	server->to_send.message->game = server->game;
	server->to_send.packet->address.port = server->received.packet->address.port;
	server->to_send.packet->address.host = server->received.packet->address.host;
	
	memcpy(server->to_send.packet->data, server->to_send.message, sizeof(t_server_message));
	server->to_send.packet->len = sizeof(t_server_message);
	if ((nb_packets = SDLNet_UDP_Send(server->socket, -1,
					server->to_send.packet)) == 0)
	{
		printf("fail de send\n");
		return (0);
	}
	server->to_send.message->message_number++;
	return (1);
}

void	ft_deconnect(t_server *server, int player_index)
{
	server->cm[player_index].isfree = 1;
	ft_init_player(&(server->game.players[player_index]));
	server->nb_clients--;
}

int		ft_process_client_message(t_server *server, t_client_message *message)
{
	server->to_send.message->player_index = message->player_index;
	server->cm[message->player_index].last_tick = SDL_GetTicks();

	if (server->cm[message->player_index].last_message_number > message->message_number)
		printf("wrong packet order:\navant %u\napres %u\n\n", server->cm[message->player_index].last_message_number, message->message_number);
	else	
		server->cm[message->player_index].last_message_number = message->message_number;
	
	if (message->flag == REGULAR)
	{
		ft_process_engine(server, message);
		ft_send_data_back(server);
	}
	else if (message->flag == DECONNEXION)
		ft_deconnect(server, message->player_index);
	return (1);
}

void	ft_check_for_data(t_server *server)
{
	t_client_message	*received_message;

	if (SDLNet_UDP_Recv(server->socket, server->received.packet))
	{
		received_message = (t_client_message *)(server->received.packet->data);
		if (received_message->player_index == -1)
		{
			server->to_send.message->player_index = ft_get_client_index(server);
			ft_send_data_back(server);
		}
		else
			ft_process_client_message(server, received_message);
	}
}

void	ft_update_time_out(t_server *server)
{
	Uint32	ticks;
	int		i;

	i = 0;
	ticks = SDL_GetTicks();
	while (i < MAX_CLIENTS)
	{
		if (!server->cm[i].isfree && ticks - server->cm[i].last_tick > TIMEOUT_THRESHOLD)
			ft_deconnect(server, i);
		i++;
	}
}

void	ft_process_server(char *port)
{
	t_server	server;
	int			activity;

	if (!ft_init_server(&server, ft_atoi(port)))
		exit(1);
	server.framerate.ms_counter = SDL_GetTicks();
	while (server.on)
	{
		activity = SDLNet_CheckSockets(server.socket_set, 1000);
		if (activity <= 0)
			ft_printf("no activity...\n");
		else
			ft_check_for_data(&server);
		ft_update_time_out(&server);
		ft_process_delta(&(server.framerate));
//		SDL_Delay(1000 / (TICKRATE * (server.nb_clients + 1)) - server.framerate.delta);
		SDL_Delay(1000 / (TICKRATE * (server.nb_clients + 1)));
	}
}
