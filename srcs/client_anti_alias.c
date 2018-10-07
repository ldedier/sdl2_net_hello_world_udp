/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_anti_alias.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 14:45:05 by ldedier           #+#    #+#             */
/*   Updated: 2018/10/07 14:45:05 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

char	get_line_intersection(t_segment sg1, t_segment sg2, t_vec2 *intersect)
{
	t_vec2 s1;
	t_vec2 s2;
	float s;
	float t;

	s1.x = sg1.b.x - sg1.a.x;
	s1.y = sg1.b.y - sg1.a.y;

	s2.x = sg2.b.x - sg2.a.x;
	s2.y = sg2.b.y - sg2.a.y;

	s = (-s1.y * (sg1.a.x - sg2.a.x) + s1.x * (sg1.a.y - sg2.a.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (sg1.a.y - sg2.a.y) - s2.y * (sg1.a.x - sg2.a.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		intersect->x = sg1.a.x + (t * s1.x);
		intersect->y = sg1.a.y + (t * s1.y);
		return 1;
	}
	return 0; // No collision
}

t_color		ft_int_to_color(int color)
{
	t_color c;

	c.col = color;
	c.r = (color >> 16) & 0xFF;
	c.g = (color >> 8) & 0xFF;
	c.b = (color) & 0xFF;
	c.col = color;
	return (c);
}

int			ft_color_to_int(t_color color)
{
	return (color.r << 16 | (color.g << 8) | (color.b));
}

int			ft_color_shade(int color, double shade)
{
	t_color col;

	col = ft_int_to_color(color);
	col.r *= shade;
	col.g *= shade;
	col.b *= shade;
	return (ft_color_to_int(col));
}

t_segment	ft_new_segment(double x1, double y1, double x2, double y2)
{
	t_segment res;

	res.a = ft_new_vec2(x1, y1);
	res.b = ft_new_vec2(x2, y2);
	return (res);
}

t_vec2		ft_third_point(t_aa aa, t_ivec2 pixel)
{
	if (aa.up && aa.left)
		return ft_new_vec2(pixel.x, pixel.y);
	else if (aa.up && aa.right)
		return ft_new_vec2(pixel.x + 1, pixel.y);
	else if (aa.down && aa.left)
		return ft_new_vec2(pixel.x, pixel.y + 1);
	else // aa.down && aa.right
		return ft_new_vec2(pixel.x + 1, pixel.y + 1);
}

double	ft_dist(t_vec2 vec1, t_vec2 vec2)
{
	return sqrt(((vec1.x - vec2.x) * (vec1.x - vec2.x)) +
			((vec1.y - vec2.y) * (vec1.y - vec2.y)));
}

double	ft_get_area(t_aa *aa, t_vec2 center)
{
	int to_add;
	int closest_vertex;
	double small_dist;
	double long_dist;

	if (aa->up && aa->down)
	{
		to_add = (center.x > aa->intersections[0].x) > 0 ? 1 : 0;
		aa->others[0] = ft_new_vec2((int)aa->intersections[0].x + to_add,
			aa->intersections[0].y);
		aa->others[1] = ft_new_vec2((int)aa->intersections[1].x + to_add,
			aa->intersections[1].y);
		if (ft_fabs(aa->intersections[0].x - aa->others[0].x) <
			ft_fabs(aa->intersections[1].x - aa->others[1].x))
		{
			closest_vertex = 0;
			small_dist = ft_fabs(aa->intersections[0].x - aa->others[0].x); 
			long_dist = ft_fabs(aa->intersections[1].x - aa->others[1].x);
		}
		else
		{
			closest_vertex = 1;
			small_dist = ft_fabs(aa->intersections[1].x - aa->others[1].x);
			long_dist = ft_fabs(aa->intersections[0].x - aa->others[0].x); 
		}
		return ((small_dist * 1) + ((long_dist - small_dist) * 1) / 2);
	}
	else
	{
		to_add = (center.y > aa->intersections[0].y) > 0 ? 1 : 0;
		aa->others[0] = ft_new_vec2(aa->intersections[0].x,
			(int)aa->intersections[0].y + to_add);
		aa->others[1] = ft_new_vec2(aa->intersections[1].x,
			(int)aa->intersections[1].y + to_add);

		if (ft_fabs(aa->intersections[0].y - aa->others[0].y) <
			ft_fabs(aa->intersections[1].y - aa->others[1].y))
		{
			closest_vertex = 0;
			small_dist = ft_fabs(aa->intersections[0].y - aa->others[0].y); 
			long_dist = ft_fabs(aa->intersections[1].y - aa->others[1].y);
		}
		else
		{
			closest_vertex = 1;
			small_dist = ft_fabs(aa->intersections[1].y - aa->others[1].y);
			long_dist = ft_fabs(aa->intersections[0].y - aa->others[0].y); 
		}
		return ((small_dist * 1) + ((long_dist - small_dist) * 1) / 2);
	}
}

double		ft_aa_area(t_vec2 pos, double dir, t_vec2 center)
{
	double line_dir = dir + M_PI / 2;
	t_aa aa;
	t_segment segment;
	t_ivec2 ivec;
	double area;

	aa.nb_intersections = 0;
	ivec = ft_vec2_to_ivec2(pos);
	segment.a = ft_vec2_dest(pos, line_dir, 2);
	segment.b = ft_vec2_dest(pos, line_dir + M_PI, 2);
	t_vec2 intersection;

	if ((aa.up = get_line_intersection(segment,
					ft_new_segment(ivec.x, ivec.y, ivec.x + 1, ivec.y),
					&intersection)))
		aa.intersections[aa.nb_intersections++] = intersection;
	if ((aa.down = get_line_intersection(segment,
					ft_new_segment(ivec.x, ivec.y + 1, ivec.x + 1, ivec.y + 1),
					&intersection)))
		aa.intersections[aa.nb_intersections++] = intersection;
	if ((aa.right = get_line_intersection(segment,
					ft_new_segment(ivec.x + 1, ivec.y, ivec.x + 1, ivec.y + 1),
					&intersection)))
		aa.intersections[aa.nb_intersections++] = intersection;
	if ((aa.left = get_line_intersection(segment,
					ft_new_segment(ivec.x, ivec.y, ivec.x, ivec.y + 1),
					&intersection)))
	aa.intersections[aa.nb_intersections++] = intersection;
	if ((aa.up && aa.down) || (aa.left && aa.right))
		return  ft_get_area(&aa, center);
	else
	{
		t_vec2 third = ft_third_point(aa, ivec);
		int inside =
			(ft_fabs(third.x - center.x) < ft_fabs(pos.x - center.x)) ? 1 : 0;
		area = 
			ft_dist(third, aa.intersections[0]) *
			ft_dist(third, aa.intersections[1]) / 2;
		return ((inside == 1) ? area : 1 - area);
	}
}
