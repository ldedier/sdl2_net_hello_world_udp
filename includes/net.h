/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/30 19:45:00 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/22 01:27:06 by ldedier          ###   ########.fr       */
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

# define MAX_SOCKETS 4
# define MAX_CLIENTS 3

# define DEFAULT 0
# define INIT_USERNAME 1
# define FROM_SERVER 2

typedef struct			s_client_socket
{
	UDPsocket			socket;
	int					isfree;
}						t_client_socket;

typedef struct			s_message
{
	char				mdr;
}						t_message;


typedef struct			s_bundle
{
	t_message			*message;
	UDPpacket			*packet;
}						t_bundle;

typedef struct			s_server
{
	t_bundle			received;
	t_bundle			to_send;
	UDPsocket			socket;
	t_client_socket		client_sockets[MAX_CLIENTS];
	int					on;
}						t_server;

typedef struct			s_client
{
	t_bundle			received;
	t_bundle			to_send;
	UDPsocket			socket;
	IPaddress			server_ip;
}						t_client;

void				ft_process_client(char *serverName, char *port, char *message);
void				ft_process_server(char *port);
void				ft_error(char *str);
int					ft_init_sdl_net(void);
int					ft_open_port(UDPsocket *socket, int port);
int					ft_resolve_ip_address(IPaddress *address, char *ip, int port);
UDPpacket 			*ft_create_packet(size_t packet_size);
#endif
