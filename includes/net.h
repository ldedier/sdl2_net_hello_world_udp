/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/30 19:45:00 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/22 22:28:43 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NET_H
# define NET_H

# include "libft.h"
# include <stdlib.h>
# include <fcntl.h>
# include <SDL.h>
# include <SDL_ttf.h>
# include <SDL_net.h>
# include <SDL_mixer.h>

# define MAX_CLIENTS 32

# define DEFAULT 0
# define INIT_USERNAME 1
# define FROM_SERVER 2
# define CONNECTION_RETRIES_LIMIT 5
# define TIMEOUT_THRESHOLD 500
# define TICKRATE 120
# define SPEED 10

typedef enum			e_keys_enum
{
	KEY_DOWN,
	KEY_UP,
	KEY_RIGHT,
	KEY_LEFT,
	NB_KEYS
}						t_keys_enum;

typedef struct			s_xy
{
	int					x;
	int					y;
}						t_xy;

typedef struct			s_player
{
	t_xy				pos;
	t_xy				speed;
	int					dead;
}						t_player;

typedef struct			s_game
{
	t_player			players[MAX_CLIENTS];
}						t_game;

typedef struct			s_client_manager
{
	Uint32				last_tick;
	int					isfree;
}						t_client_manager;

typedef struct			s_server_message
{
	char				player_index;
	t_game				game;
	Uint32				message_number;
}						t_server_message;

typedef struct			s_keys
{
	char				left;
	char				right;
	char				up;
	char				down;
}						t_keys;

typedef struct			s_client_message
{
	char				player_index;
	char				keys[NB_KEYS];
	Uint32				message_number;
}						t_client_message;

typedef struct			s_server_bundle
{
	t_server_message	*message;
	UDPpacket			*packet;
}						t_server_bundle;

typedef struct			s_client_bundle
{
	t_client_message	*message;
	UDPpacket			*packet;
}						t_client_bundle;

typedef struct     		s_sdl
{
	SDL_Surface			*surface;
	SDL_Texture			*texture;
	SDL_Rect			screen;
	SDL_Window			*window;
	SDL_Renderer		*renderer;
}						t_sdl;

typedef struct			s_server
{
	t_client_bundle		received;
	t_server_bundle		to_send;
	UDPsocket			socket;
	t_client_manager	cm[MAX_CLIENTS];
	int					on;
	SDLNet_SocketSet	socket_set;
	int					nb_clients;
	t_game				game;
}						t_server;

typedef struct			s_client
{
	t_server_bundle		received;
	t_client_bundle		to_send;
	UDPsocket			socket;
	IPaddress			server_ip;
	int					on;
	SDLNet_SocketSet	socket_set;
	Uint32				last_tick;
	t_player			player;
	t_game				game;
	t_sdl				sdl;
}						t_client;

void					ft_process_client(char *serverName, char *port);
void					ft_process_server(char *port);
void					ft_error(char *str);
int						ft_init_sdl_net(void);
int						ft_open_port(UDPsocket *socket, int port);
int						ft_resolve_ip_address(IPaddress *address, char *ip, int port);
UDPpacket				*ft_create_packet(size_t packet_size);
int						ft_init_client_bundle(t_client_bundle *bundle, size_t size);
int						ft_init_server_bundle(t_server_bundle *bundle, size_t size);
int						ft_init_sdl(char *name, t_sdl *sdl);
void					ft_process_keydown(t_client *client, SDL_Keycode code);
void					ft_process_keyup(t_client *client, SDL_Keycode code);
void					ft_process_mousedown(t_client *client, SDL_Keycode code);
void					ft_process_mouseup(t_client *client, SDL_Keycode code);
void					ft_process_keyboard(t_client *client, const Uint8* keys);
void					ft_process_mouse(t_client *client, Uint32 keys);
#endif
