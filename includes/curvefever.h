/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curvefever.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 20:41:12 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/03 20:41:12 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CURVEFEVER_H
# define CURVEFEVER_H

# define BOARD_WIDTH	1
# define BOARD_HEIGHT	9217 - 144

typedef struct          s_xy
{
	float               x;
	float               y;
}                       t_xy;

typedef struct          s_curve
{
	double				default_mobility;
	int					default_width;
	double				default_speed;
	double				default_dash_rate;
}                       t_curve;

typedef struct          s_player
{
	t_xy				pos;
	t_xy				dir;
//	int					color;
//	
	int					dead;
//	t_curve				curve;
//	double				speed;
	double				angle;
//	int					score;
}						t_player;

typedef struct			s_board
{
	char				map[BOARD_HEIGHT][BOARD_WIDTH];
}						t_board;

#endif
