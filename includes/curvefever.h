/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curvefever.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 20:41:12 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/27 20:09:48 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CURVEFEVER_H
# define CURVEFEVER_H

# define BOARD_WIDTH		1280
# define BOARD_HEIGHT		720
# define MAX_MOVES			10
# define MAX_COLORED		100000
# define MAX_FLAGS			15
# define DEFAULT_MOBILITY	M_PI / 32
# define MARGIN				0
# define EPSILON			0.1

# define INIT_SPEED			1
# define SPEED				1
# define INIT_RADIUS		1
# define RADIUS				5

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
	int					default_radius;
	double				default_speed;
	double				default_dash_rate;
}                       t_curve;

typedef struct          s_player
{
	t_vec2				pos;
	int					color;	
	int					dead;
	char				index;
	t_curve				curve;
	double				speed;
	double				angle;
	double				radius;
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

typedef struct			s_move
{
	t_vec2				from;
	t_vec2				to;
	double				speed;
	double				angle;
	double				radius;
	//double			invis...
	char				player_index;
}						t_move;



typedef struct			s_colored
{
	t_ivec2				pos;
	char				player_index;
}						t_colored;

typedef struct			s_colored_stack
{
	t_colored			colored[MAX_COLORED];
	int					nb_colored;
}						t_colored_stack;

typedef struct			s_move_stack
{
	t_move				moves[MAX_MOVES];
	int					nb_moves;
}						t_move_stack;

typedef struct			s_event_stack
{
	char				events[MAX_FLAGS];
	int					nb_events;
}						t_event_stack;

typedef struct			s_changes
{
	t_move_stack		move_stack;
	t_event_stack		event_stack;
}						t_changes;

typedef struct			s_board
{
	char				map[BOARD_HEIGHT][BOARD_WIDTH];
	t_ivec2				current_dim;
	t_ivec2				init_dim;
}						t_board;

typedef struct			s_client_response
{
	t_move				*move_data;
	char				*events_data;
	t_player			*players_data;
	int					nb_players;
	int					nb_moves;
	int					nb_events;
}						t_client_response;

void					ft_init_board(t_board *board);
void					ft_print_vec2(t_vec2 vec2);
void					ft_print_ivec2(t_ivec2 vec2);
t_vec2					ft_vec2_dest(t_vec2 pos, double angle, float speed);
#endif
