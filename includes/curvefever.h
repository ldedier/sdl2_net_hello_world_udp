/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curvefever.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 20:41:12 by ldedier           #+#    #+#             */
/*   Updated: 2018/09/29 21:51:48 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CURVEFEVER_H
# define CURVEFEVER_H

# define BOARD_WIDTH		1280
# define BOARD_HEIGHT		720
# define MAX_MOVES			20
# define MAX_FLAGS			15
# define DEFAULT_MOBILITY	M_PI / 128
# define EPSILON			2

# define MOBILITY			30
# define SPEED				1.5
# define RADIUS				5
# define DASH				993

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
//	t_curve				curve;
	double				speed;
	double				angle;
	double				radius;
	double				mobility;
	double				dash;
	char				vulnerability;
	double				vulnerability_countdown;
//	int					score;
}						t_player;

/*
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
*/

typedef struct			s_color
{
	unsigned char		a;
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	int					col;
}						t_color;



typedef struct			s_forward_move
{
	double				distance;
}						t_forward_move;

typedef struct			s_rotate_move
{
	double				angle;
	double				angle_init;
	double				mobility;
	char				dir;
}						t_rotate_move;

typedef union			u_move_union
{
	t_forward_move		fmove;
	t_rotate_move		rmove;
}						t_move_union;

typedef struct			s_move
{
	t_move_union		move_union;
	char				is_rotate;
	t_vec2				origin;
	double				radius;
	double				speed;
	double				player_angle;
	char				player_index;
	int					color;
}						t_move;

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

typedef struct			s_tile
{
	char				player_index : 4;
	char				parsed : 4;
}						t_tile;

typedef struct			s_board
{
	t_tile				**map;
	t_ivec2				current_dim;
	t_ivec2				init_dim;
}						t_board;

typedef struct			s_client_tile
{
	int					color;
	double				area;
	char				parsed;
}						t_client_tile;

typedef struct			s_client_board
{
	t_client_tile		**map;
	t_ivec2				current_dim;
	t_ivec2				init_dim;
}						t_client_board;

typedef struct			s_segment
{
	t_vec2				a;
	t_vec2				b;
}						t_segment;

typedef struct			s_aa
{
	t_segment			segment;
	char				up;
	char				down;
	char				right;
	char				left;
	t_vec2				intersections[4];
	char				nb_intersections;
	t_vec2				others[2];
}						t_aa;

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
void					ft_init_client_board(t_client_board *board);
void					ft_clear_board(t_board *board);
int						ft_is_on_board(t_ivec2 dim, int x, int y);
void					ft_print_vec2(t_vec2 vec2);
void					ft_print_ivec2(t_ivec2 vec2);
t_vec2					ft_vec2_dest(t_vec2 pos, double angle, double speed);
t_ivec2                 ft_vec2_to_ivec2(t_vec2 iter);
t_vec2 					ft_new_vec2(double x, double y);
double					ft_aa_area(t_vec2 iter, double angle, t_vec2 pos);
int						ft_color_shade(int color, double shade);
#endif
