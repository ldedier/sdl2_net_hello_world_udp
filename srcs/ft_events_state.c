/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_events_state.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 21:51:16 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/25 21:51:16 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

void	ft_process_keyboard(t_client *client, const Uint8* keys)
{
	client->to_send.message->keys[KEY_RIGHT] = keys[SDL_SCANCODE_RIGHT];
	client->to_send.message->keys[KEY_LEFT] = keys[SDL_SCANCODE_LEFT];
}

void	ft_process_mouse(t_client *client, Uint32 keys)
{

}
