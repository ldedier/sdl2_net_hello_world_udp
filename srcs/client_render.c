/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 17:35:12 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/06 17:35:12 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_render_player(t_client *client, t_player player)
{
	int *pixels = (int *)client->sdl.surface->pixels;
	int surface_width =  client->sdl.surface->w;

	t_vec2 iter = player.pos;
	double radius = player.radius;
	int i;
	int j;

	i = ft_max(0, iter.y - radius);
	while (i < ft_min(client->board.current_dim.y, iter.y + radius))
	{
		j  = (int)iter.x;
		while ((j - iter.x) * (j - iter.x) + ((i - iter.y) * (i - iter.y)) < radius * radius && j >= 0)
		{
			pixels[i * surface_width + j] = player.color;
			j--;
		}
		j  = (int)iter.x;
		while ((j - iter.x) * (j - iter.x) + ((i - iter.y) * (i - iter.y)) < radius * radius && j < client->board.current_dim.x)
		{
			pixels[i * surface_width + j] = player.color;
			j++;
		}
		i++;
	}

}

void	ft_render_players(t_client *client)
{
	t_game  game;
	int i;
	i = 0;
	while (i < client->response.nb_players)
	{
//		if (!client->response.players_data[i].dead)
			ft_render_player(client, client->response.players_data[i]);
		i++;
	}
}

void	ft_render_board(t_client *client)
{
	int i;
	int j;
	int index;

	int *pixels = (int *)client->sdl.surface->pixels;
	int surface_width =  client->sdl.surface->w;
	i = 0;
	while (i < BOARD_HEIGHT)
	{
		j = 0;
		while (j < BOARD_WIDTH)
		{
			if ((index = client->board.map[i][j].color))
			{
				pixels[i * surface_width + j] = client->board.map[i][j].color;
			}
			j++;
		}
		i++;
	}
}

void    ft_render(t_client *client)
{
	SDL_SetRenderDrawColor(client->sdl.renderer, 0, 0, 0, 255);
	SDL_RenderClear(client->sdl.renderer);
	ft_render_board(client);
	ft_render_players(client);

	client->sdl.texture = SDL_CreateTextureFromSurface(client->sdl.renderer, client->sdl.surface);
	SDL_RenderCopy(client->sdl.renderer, client->sdl.texture, NULL, NULL);
	SDL_FillRect(client->sdl.surface, NULL, 0);
	SDL_DestroyTexture(client->sdl.texture);
	SDL_RenderPresent(client->sdl.renderer);

	client->framerate.fps_counter++;
}
