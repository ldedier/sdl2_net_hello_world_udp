/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/05 23:28:37 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/05 23:28:37 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

t_vec2	ft_new_vec2(double x, double y)
{
	t_vec2 res;

	res.x = x;
	res.y = y;
	return (res);
}

t_vec2	ft_vec2_scalar(t_vec2 vec, double scalar)
{
	t_vec2 res;

	res.x = vec.x * scalar;
	res.y = vec.y * scalar;
	return (res);
}

t_vec2	ft_vec2_add(t_vec2 vec1, t_vec2 vec2)
{
	t_vec2 res;

	res.x = vec1.x + vec2.x;
	res.y = vec1.y + vec2.y;
	return res;
}

t_vec2	ft_vec2_dest(t_vec2 pos, double angle, double speed)
{
	t_vec2 res;

	res.x = pos.x + cos(angle) * speed;
	res.y = pos.y + sin(angle) * speed;
	return (res);
}

t_ivec2 ft_vec2_to_ivec2(t_vec2 vec)
{
	t_ivec2 res;

	res.x = (int) vec.x;
	res.y = (int) vec.y;
	return res;
}
