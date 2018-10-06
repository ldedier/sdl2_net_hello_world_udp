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
	//	ft_load_textures(&(client->sdl)); // to protect
	ft_init_keys(client->to_send.message->keys);
	ft_init_client_board(&(client->board));
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
