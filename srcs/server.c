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

void	ft_reset_changes(t_changes *changes)
{
	changes->event_stack.nb_events = 0;
	changes->move_stack.nb_moves = 0;
}

void	ft_init_clients(t_client_manager cm[MAX_CLIENTS])
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		cm[i].isfree = 1;
		ft_reset_changes(&(cm[i].changes));
		i++;
	}
}

void	ft_init_player(t_player *player, int index)
{
	if (index == 0)
	{
		player->pos.x = BOARD_WIDTH / 3;
		player->pos.y = BOARD_HEIGHT / 3;
		player->angle = 0;
		player->color = 0xff0000;
	}
	else
	{
		player->pos.x = (2 * BOARD_WIDTH) / 3;
		player->pos.y = (2 * BOARD_HEIGHT) / 3;
		player->angle = M_PI;
		player->color = 0x0000ff;
	}
	player->dead = 1;
	player->angle = 0;
	player->radius = 100;
}

void	ft_init_game(t_game *game)
{
	int i;

	i = 0;
	while (i < MAX_CLIENTS)
	{
		ft_init_player(&(game->players[i]), i);
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
	ft_init_board(&(server->board));	
	server->framerate.previous = SDL_GetPerformanceCounter();
	server->nb_clients = 0;
	server->colored_stack.nb_colored = 0;
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
			server->game.players[i].index = i;
			server->cm[i].last_tick = SDL_GetTicks();
			server->cm[i].last_message_number = 0;
			server->nb_clients++;
			return (i);
		}
		i++;
	}
	return (-1);
}

t_vec2	ft_new_vec2(double x, double y)
{
	t_vec2 res;

	res.x = x;
	res.y = y;
	return (res);
}

t_vec2	ft_vec2_scalar(t_vec2 vec, float scalar)
{
	t_vec2 res;

	res.x = vec.x * scalar;
	res.y = vec.y * scalar;
	return (res);
}

t_vec2	ft_vec2_add(t_vec2 vec1, t_vec2 vec2)
{
	t_vec2 res;

	res.x = vec1.x + vec2.x;
	res.y = vec1.y + vec2.y;
	return res;
}

t_vec2	ft_vec2_dest(t_vec2 pos, double angle, float speed)
{
	t_vec2 res;

	res.x = pos.x + cos(angle) * speed;
	res.y = pos.y + sin(angle) * speed;
	return (res);
}

void	ft_update_angle(char keys[NB_KEYS], double *angle)
{
	if (keys[KEY_LEFT])
		*angle -= DEFAULT_MOBILITY;
	else if (keys[KEY_RIGHT])
		*angle += DEFAULT_MOBILITY;
}


int		ft_is_in_sphere(double radius, t_vec2 center, t_vec2 pos)
{
	return ((pos.x - center.x) * (pos.x - center.x) + (pos.y - center.y) * (pos.y - center.y) < radius * radius);
}

int		ft_collide_board_sphere(t_board board, double radius, t_vec2 center, t_vec2 except)
{
	int i;
	int j;

	i = center.y - radius;
	while (i < center.y + radius)
	{
		j  = (int)center.x;
		while ((j - center.x) * (j - center.x) + ((i - center.y) * (i - center.y)) < radius * radius - MARGIN)
		{
			if (board.map[i][j] && !ft_is_in_sphere(radius, except, ft_new_vec2(j, i)))
				return (1);
			j--;
		}
		j  = (int)center.x + 1;
		while ((j - center.x) * (j - center.x) + ((i - center.y) * (i - center.y)) < radius * radius - MARGIN)
		{
			if (board.map[i][j] && !ft_is_in_sphere(radius, except, ft_new_vec2(j, i)))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}


int		ft_iz_okay(t_board board, t_vec2 vec, t_vec2 from, double radius)
{
	/*
	if (board.map[(int)vec.y][(int)vec.x])
	{
		printf("olala pos not empty: (%d, %d)\n((%f, %f))\n",
		(int)vec.x, (int)vec.y, vec.x, vec.y);
	}
	*/
//	printf("is_in_sphere(): %d\n", ft_is_in_sphere(radius, from, vec));
//	printf("ft_collide_board_sphere(): %d\n", ft_collide_board_sphere(board, radius, vec, from));
	return (vec.x >= 0 && vec.x < board.current_dim.x
				&& vec.y >= 0 && vec.y < board.current_dim.y
					&& !ft_collide_board_sphere(board, radius, vec, from));
}

void	ft_print_vec2(t_vec2 vec)
{
	printf("(%.2f, %.2f)\n", vec.x, vec.y);
}

void	ft_print_ivec2(t_ivec2 vec)
{
	printf("(%d, %d)\n", vec.x, vec.y);
}


void	ft_apply_changes(t_server *server)
{
	int i;
	t_colored colored;

	i = 0;
	while (i < server->colored_stack.nb_colored)
	{
		colored = server->colored_stack.colored[i];
//		ft_print_colored(colored);
		server->board.map[colored.pos.y][colored.pos.x] = colored.player_index + 1;
		i++;
	}
	server->colored_stack.nb_colored = 0;
}

int		is_already_in_color_stack(t_colored_stack colored_stack, int x, int y)
{
	int i;

	i = 0;
	while (i < colored_stack.nb_colored)
	{
		if (colored_stack.colored[i].pos.x == x && colored_stack.colored[i].pos.y == y)
			return (1);
		i++;
	}
	return (0);
}

void	ft_add_color_to_stack(t_colored_stack *colored_stack, int x, int y, char player_index)
{
	if (colored_stack->nb_colored >= MAX_COLORED)
	{
		printf("too much color changes\n");
		exit(1);
	}
///	if (!is_already_in_color_stack(*colored_stack, x, y))
//	{
		colored_stack->colored[colored_stack->nb_colored].pos.x = x;
		colored_stack->colored[colored_stack->nb_colored].pos.y = y;
		colored_stack->colored[colored_stack->nb_colored].player_index = player_index;
		colored_stack->nb_colored++;
//	}
}

void	ft_stack_changes_color(t_server *server, t_colored_stack *colored_stack, t_vec2 iter, char player_index)
{
	double radius;
	int i;
	int j;

	radius = server->game.players[player_index].radius;
	
	i = iter.y - radius;
	while (i < iter.y + radius)
	{
		j  = (int)iter.x;
		while ((j - iter.x) * (j - iter.x) + ((i - iter.y) * (i - iter.y)) < radius * radius)
		{
			ft_add_color_to_stack(colored_stack, j, i, player_index);
			j--;
		}
		j  = (int)iter.x + 1;
		while ((j - iter.x) * (j - iter.x) + ((i - iter.y) * (i - iter.y)) < radius * radius)
		{
			ft_add_color_to_stack(colored_stack, j, i, player_index);
			j++;
		}
		i++;
	}
}

void	ft_stack_changes_move(t_player player, t_move_stack *move_stack, t_vec2 from, t_vec2 to)
{
	if (move_stack->nb_moves > MAX_MOVES)
	{
		printf("too much moves\n");
		exit(1);
	}
	move_stack->moves[move_stack->nb_moves].from = from;
	move_stack->moves[move_stack->nb_moves].to = to;
	move_stack->moves[move_stack->nb_moves].player_index = player.index;
	move_stack->moves[move_stack->nb_moves].angle = player.angle;
	move_stack->moves[move_stack->nb_moves].speed = player.speed;
	move_stack->moves[move_stack->nb_moves].radius = player.radius;
	move_stack->nb_moves++;
}

void	ft_process_engine(t_server *server, t_client_message *message)
{
	t_vec2 to;
	t_vec2 from;
	t_vec2 iter;

	t_vec2 res;
	int i;

	if (!server->game.players[message->player_index].dead)
	{
		ft_update_angle(message->keys, &(server->game.players[message->player_index].angle));
		from = server->game.players[message->player_index].pos;
		to = ft_vec2_dest(from, server->game.players[message->player_index].angle, SPEED);
		res = to;
		iter = from;
		while ((int)iter.x != (int)to.x || (int)iter.y != (int)to.y)
		{
			iter = ft_vec2_dest(iter, server->game.players[message->player_index].angle, ft_fmin(1, SPEED));
			if (ft_iz_okay(server->board, iter, from, server->game.players[message->player_index].radius))
			{
				ft_stack_changes_color(server, &(server->colored_stack), iter, message->player_index);
			}
			else
			{
				server->game.players[message->player_index].dead = 1;
				res = iter;
				printf("dead\n");
				break;
			}
		}
		i = 0;
		while (i < MAX_CLIENTS)
		{
			if (!server->cm[i].isfree)
				ft_stack_changes_move(server->game.players[message->player_index], &(server->cm[i].changes.move_stack), from, iter);
			i++;
		}
		server->game.players[message->player_index].pos = res;
		ft_apply_changes(server);
	}
}

size_t	memcpy_ret(void *dest, void *src, size_t size)
{
	memcpy(dest, src, size);
	return size;
}

int		ft_get_server_packet_size(t_server *server)
{
	char player_index = server->to_send.message->player_index;
	int i;
	int size;

	size = 0;
	size += sizeof(player_index);
	size += sizeof(server->to_send.message->message_number);
	size += sizeof(server->cm[player_index].changes.move_stack.nb_moves);
	size += sizeof(server->cm[player_index].changes.event_stack.nb_events);
	size += sizeof(server->nb_clients);
	i = 0;
	while (i < server->cm[player_index].changes.move_stack.nb_moves)
	{
		size += sizeof(server->cm[player_index].changes.move_stack.moves[i]);
		i++;
	}
	i = 0;
	while (i < server->cm[player_index].changes.event_stack.nb_events)
	{
		size += sizeof(server->cm[player_index].changes.event_stack.events[i]);
		i++;
	}
	i = 0;
	while (i < MAX_CLIENTS)
	{
		if (!server->cm[i].isfree)
			size += sizeof(server->game.players[i]);
		i++;
	}
	return (size);
}

int		ft_fill_packet_server(t_server *server)
{
	char player_index = server->to_send.message->player_index;
	int size;
	int i;
	
	size = 0;

	int	total_size;

	total_size = ft_get_server_packet_size(server);
	if (SDLNet_ResizePacket(server->to_send.packet, total_size) < total_size)
		ft_error("could not resize packet");

//	if(1)
//		printf("resized to:%d\n", total_size);

	Uint8 *data = server->to_send.packet->data;
	
	size += memcpy_ret(&(data[size]), &(player_index), sizeof(player_index));
	size += memcpy_ret(&(data[size]), &(server->to_send.message->message_number), sizeof(server->to_send.message->message_number));
	size += memcpy_ret(&(data[size]), &(server->cm[player_index].changes.move_stack.nb_moves), sizeof(server->cm[player_index].changes.move_stack.nb_moves));
	size += memcpy_ret(&(data[size]), &(server->cm[player_index].changes.event_stack.nb_events), sizeof(server->cm[player_index].changes.event_stack.nb_events));
	size += memcpy_ret(&(data[size]), &(server->nb_clients), sizeof(server->nb_clients));

	i = 0;
	while (i < server->cm[player_index].changes.move_stack.nb_moves)
	{
		size += memcpy_ret(&data[size], &(server->cm[player_index].changes.move_stack.moves[i]), sizeof(server->cm[player_index].changes.move_stack.moves[i]));
		i++;
	}
	i = 0;
	while (i < server->cm[player_index].changes.event_stack.nb_events)
	{
		size += memcpy_ret(&data[size], &(server->cm[player_index].changes.event_stack.events[i]), sizeof(server->cm[player_index].changes.event_stack.events[i]));
		i++;
	}
	i = 0;
	while (i < MAX_CLIENTS)
	{
		if (!server->cm[i].isfree)
		{
			size += memcpy_ret(&data[size], &(server->game.players[i]), sizeof(server->game.players[i]));
		}
		i++;
	}
	return (size);
}

int		ft_send_data_back(t_server *server)
{
	int		nb_packets;
	int		packet_size;

	server->to_send.packet->address.port = server->received.packet->address.port;
	server->to_send.packet->address.host = server->received.packet->address.host;

	packet_size = ft_fill_packet_server(server);
	server->to_send.packet->len = packet_size;

	if ((nb_packets = SDLNet_UDP_Send(server->socket, -1,
					server->to_send.packet)) == 0)
	{
		printf("fail de send\n");
		return (0);
	}
	else
		ft_reset_changes(&(server->cm[server->to_send.message->player_index].changes));
	server->to_send.message->message_number++;
	return (1);
}

void	ft_deconnect(t_server *server, int player_index)
{
	if (!server->cm[player_index].isfree)
	{
		server->cm[player_index].isfree = 1;
		ft_init_player(&(server->game.players[player_index]), player_index);
		if(--server->nb_clients == 0)
			ft_init_board(&(server->board));
	}
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
//		printf("%d\n", received_message->player_index);
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
		{
			printf("player %d timed out!\n", i);
			ft_deconnect(server, i);
		}
		i++;
	}
}

void	ft_process_server(char *port)
{
	t_server	server;
	int			activity;

	if (!ft_init_server(&server, ft_atoi(port)))
		exit(1);
//	server.framerate.ms_counter = SDL_GetTicks();
	while (server.on)
	{
		activity = SDLNet_CheckSockets(server.socket_set, 1000);
		if (activity <= 0)
			ft_printf("no activity...\n");
		else
			ft_check_for_data(&server);
		ft_update_time_out(&server);
//		ft_process_delta(&(server.framerate));
//		SDL_Delay(1000 / (TICKRATE * (server.nb_clients + 1)) - server.framerate.delta);
//		SDL_Delay(1);
//		SDL_Delay(1000 / (TICKRATE * (server.nb_clients + 1)));
	}
}
