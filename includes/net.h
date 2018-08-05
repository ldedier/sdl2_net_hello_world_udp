/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/30 19:45:00 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/06 00:32:59 by ldedier          ###   ########.fr       */
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
# include <termios.h>

# define MAX_SOCKETS 4
# define MAX_CLIENTS 3
# define BUFF_SIZE 4096
# define AUTHOR_BUFF_SIZE 50

# define DEFAULT 0
# define INIT_USERNAME 1
# define FROM_SERVER 2

typedef struct		s_client_socket
{
	UDPsocket		socket;
	char			author[AUTHOR_BUFF_SIZE];
	int				isfree;
}					t_client_socket;

typedef struct		s_message
{
	char			author[AUTHOR_BUFF_SIZE];
	char			content[BUFF_SIZE];
	char			from_server;
	char			flags;
}					t_message;

void	ft_process_client(char *serverName, char *local, char *remote,  char *name, char *message);
void	ft_process_server(char *port);
void	ft_error(char *str);
#endif
