/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/30 19:45:00 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/03 19:53:12 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NET_H
# define NET_H

# include "libft.h"
# include "curvefever.h"
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include <SDL.h>
# include <SDL_ttf.h>
# include <SDL_net.h>
# include <SDL_image.h>
# include <SDL_mixer.h>
# include <time.h>

//# define MAX_CLIENTS 460
# define MAX_CLIENTS 4
# define MAX_TEXTURES 10

# define REGULAR 0
# define DECONNEXION 1

# define MAX_UDP_PACKET_SIZE 64

# define CONNECTION_RETRIES_LIMIT 5
# define TIMEOUT_THRESHOLD 5000
# define TICKRATE 60

typedef enum			e_keys_enum
{
	KEY_RIGHT,
	KEY_LEFT,
	NB_KEYS
}						t_keys_enum;

typedef struct			s_game
{
	t_player			players[MAX_CLIENTS];
}						t_game;

typedef struct			s_client_manager
{
	Uint32				last_tick;
	Uint32				last_message_number;
	int					isfree;
	t_changes			changes;
}						t_client_manager;

typedef struct			s_server_message
{
	t_game				game;
	t_changes			changes;
	char				player_index;
	Uint32				message_number;
}						t_server_message;

typedef struct			s_client_message
{
	Uint32				message_number;
	char				keys[NB_KEYS];
	char				player_index;
	char				flag;
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
	SDL_Texture         *textures[MAX_TEXTURES];
}						t_sdl;

typedef struct			s_framerate
{
	Uint64				current;
	Uint64				previous;
	double				delta;
	int					fps_counter;
	Uint32				ms_counter;
}						t_framerate;

typedef struct			s_packet_vector
{
	UDPpacket			**packets;
	int					nb_packets;
}						t_packet_vector;

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
	t_board				board;
	t_changes			changes;
	t_framerate			framerate;
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
	Uint32				last_message_number;
//	t_player			player;
	t_game				game;
	t_sdl				sdl;
	t_board				board;
	t_changes			changes;
	t_framerate			framerate;
	int					nb_clients;
	t_client_response	response;
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
void					ft_process_delta_first(t_framerate *framerate);
void					ft_process_delta(t_framerate *framerate);
void					ft_print_fps(t_framerate *framerate);
t_packet_vector			ft_create_packet_vector(size_t data_size);
void					ft_memcpy_to_packet_vector(void *source, UDPpacket **packet_vector, size_t size);
void					ft_memcpy_from_packet_vector(void *dest, UDPpacket **packet_vector);
void					ft_address_packets(UDPpacket **packet_vector, IPaddress address);
#endif
