/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   net.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/30 19:45:00 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/02 00:13:09 by ldedier          ###   ########.fr       */
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
# define PORT 3001


void	ft_process_client(void);
void	ft_process_server(void);
void	ft_error(char *str);
#endif
