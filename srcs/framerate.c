/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framerate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/26 15:15:58 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/26 15:15:58 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_process_delta_first(t_framerate *framerate)
{
	framerate->previous = SDL_GetPerformanceCounter();
}

void	ft_process_delta(t_framerate *framerate)
{
	framerate->current = SDL_GetPerformanceCounter();
	framerate->delta = framerate->current - framerate->previous;
	framerate->delta /= 1000000000;
}

void	ft_print_fps(t_framerate *framerate)
{
	if (SDL_GetTicks() - framerate->ms_counter > 1000)
	{
		framerate->ms_counter = SDL_GetTicks();
		printf("fps : %d\n", framerate->fps_counter);
		framerate->fps_counter = 0;
	}
}
