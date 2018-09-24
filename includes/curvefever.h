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

# define BOARD_WIDTH		1280
# define BOARD_HEIGHT		720
# define SPEED				1
# define MAX_COLORED		100
# define MAX_FLAGS			15
# define DEFAULT_MOBILITY	M_PI / 64

typedef struct          s_vec2
{
	float               x;
	float               y;
}                       t_vec2;

typedef struct          s_ivec2
{
	int					x;
	int					y;
}                       t_ivec2;

typedef struct          s_curve
{
	double				default_mobility;
	int					default_width;
	double				default_speed;
	double				default_dash_rate;
}                       t_curve;

typedef struct          s_player
{
	t_vec2				pos;
	int					color;	
	int					dead;
//	t_curve				curve;
//	double				speed;
	double				angle;
//	int					score;
}						t_player;

typedef struct			s_argb
{
	char				b;
	char				g;
	char				r;
	char				a;
}						t_argb;

typedef union			u_color
{
	int					color;
	t_argb				argb;
}						t_color;
		
typedef struct			s_colored
{
	t_ivec2				pos;
	char				player_index;
}						t_colored;

typedef struct			s_changes
{
	t_colored			colored[MAX_COLORED];
	int					nb_colored;
	int					nb_events;
	char				events[MAX_FLAGS];
}						t_changes;

typedef struct			s_server_changes
{
	t_colored			colored[MAX_COLORED];
	int					nb_colored;
}						t_server_changes;

typedef struct			s_board
{
	char				map[BOARD_HEIGHT][BOARD_WIDTH];
	t_ivec2				current_dim;
	t_ivec2				init_dim;
}						t_board;

typedef struct			s_client_response
{
	t_colored			*colored_data;
	char				*events_data;
	t_player			*players_data;
	int					nb_players;
	int					nb_colored;
	int					nb_events;
}						t_client_response;

void					ft_init_board(t_board *board);
void					ft_print_colored(t_colored color);
void					ft_print_vec2(t_vec2 vec2);
void					ft_print_ivec2(t_ivec2 vec2);
#endif
