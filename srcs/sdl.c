/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:57:22 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/25 18:57:22 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

int     ft_init_sdl(char *name, t_sdl *sdl)
{
	sdl->screen.x = 0;
	sdl->screen.y = 0;
	sdl->screen.w = 1280;
	sdl->screen.h = 720;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return (0);
	if (!(sdl->window = SDL_CreateWindow(name, sdl->screen.x,
					sdl->screen.y, sdl->screen.w, sdl->screen.h, 0)))
		return (0);
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, -1, 0)))
		return (0);
	if (!(sdl->surface = SDL_CreateRGBSurface(0, sdl->screen.w,
					sdl->screen.h, 32, 0, 0, 0, 0)))
		return (0);
	if (SDL_RenderSetLogicalSize(sdl->renderer, sdl->screen.w,
				sdl->screen.h) < 0)
		return (0);
	if (SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255) < 0)
		return (0);
	return (1);
}
