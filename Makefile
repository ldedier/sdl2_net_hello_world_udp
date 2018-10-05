# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/26 20:58:27 by ldedier           #+#    #+#              #
#    Updated: 2018/08/27 20:06:50 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= udp

CC		= gcc -g 
PWD = \"$(shell pwd)\"

OK_COLOR = \x1b[32;01m
EOC = \033[0m

DEBUG ?= 0

SRCDIR   = srcs
OBJDIR   = objs
BINDIR   = .
INCLUDESDIR = includes

LIBFTDIR = libft
LIBFT_INCLUDEDIR = includes
LIBFT = $(LIBFTDIR)/libft.a

LIBMATDIR = libmat
LIBMAT_INCLUDEDIR = includes
LIBMAT = $(LIBMATDIR)/libmat.a

LIBSDL2DIR = ~/.brew/lib
LIBSDL2_INCLUDEDIR = ~/.brew/Cellar/sdl2/2.0.8/include/SDL2/
LIBSDL2TTF_INCLUDEDIR = ~/.brew/Cellar/sdl2_ttf/2.0.14/include/SDL2/
LIBSDL2NET_INCLUDEDIR = ~/.brew/Cellar/sdl2_net/2.0.1/include/SDL2/
LIBSDL2MIXER_INCLUDEDIR = ~/.brew/Cellar/sdl2_mixer/2.0.2_3/include/SDL2/
LIBSDL2IMAGE_INCLUDEDIR =  ~/.brew/Cellar/sdl2_image/2.0.3/include/SDL2/

LIBSDL2DIR_MB = /usr/local/lib
LIBSDL2_INCLUDEDIR_MB = /usr/local/Cellar/sdl2/2.0.8/include/SDL2/
LIBSDL2TTF_INCLUDEDIR_MB = /usr/local/Cellar/sdl2_ttf/2.0.14/include/SDL2/
LIBSDL2IMAGE_INCLUDEDIR_MB = /usr/local/Cellar/sdl2_image/2.0.3/include/SDL2/
LIBSDL2NET_INCLUDEDIR_MB = /usr/local/Cellar/sdl2_net/2.0.1/include/SDL2/
LIBSDL2MIXER_INCLUDEDIR_MB = /usr/local/Cellar/sdl2_mixer/2.0.2_3/include/SDL2/

SRCS_NO_PREFIX =	main.c client.c server.c network.c sdl.c ft_events.c\
					ft_events_state.c framerate.c curvefever.c client_init.c\
					client_process_move.c  client_connect.c client_interact.c\
					client_render.c utils.c client_process_data.c\
					server_init.c vec.c server_process_engine.c\
					server_stack_moves.c server_fill_packet.c\
					server_interact.c server_disconnect.c server_collide.c\
					server_process_board.c

INCLUDES_NO_PREFIX = net.h curvefever.h

SOURCES  = $(addprefix $(SRCDIR)/,      $(SRCS_NO_PREFIX))
OBJECTS  = $(addprefix $(OBJDIR)/,      $(SRCS_NO_PREFIX:%.c=%.o))
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))

INC = -I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDEDIR)\
	  -I $(LIBMATDIR)/$(LIBMAT_INCLUDEDIR)\

ifeq ($(MACBOOK), 1)
	INC +=	-I $(LIBSDL2_INCLUDEDIR_MB)\
			-I $(LIBSDL2TTF_INCLUDEDIR_MB)\
			-I $(LIBSDL2NET_INCLUDEDIR_MB)\
			-I $(LIBSDL2MIXER_INCLUDEDIR_MB)\
			-I $(LIBSDL2IMAGE_INCLUDEDIR_MB)
else
	INC +=	-I $(LIBSDL2_INCLUDEDIR)\
			-I $(LIBSDL2TTF_INCLUDEDIR)\
			-I $(LIBSDL2NET_INCLUDEDIR)\
			-I $(LIBSDL2MIXER_INCLUDEDIR)\
			-I $(LIBSDL2IMAGE_INCLUDEDIR)
endif


CFLAGS = -DPATH=$(PWD)  -Werror $(INC)

ifeq ($(DEBUG), 1)
	CFLAGS += -g -DDEBUG -fsanitize=address
else
	CFLAGS += -Ofast
endif

LFLAGS = -L $(LIBFTDIR) -lft -L $(LIBMATDIR) -lmat -fsanitize=address


ifeq ($(MACBOOK), 1)
	LFLAGS += -L $(LIBSDL2DIR_MB) 
else
	LFLAGS += -L $(LIBSDL2DIR)
endif

LFLAGS += -lsdl2 -lsdl2_ttf -lsdl2_net -lsdl2_mixer -lsdl2_image

opti:
	@make -j all

all:
	@make -C $(LIBFTDIR) all
	@make -C $(LIBMATDIR) all
	@make $(BINDIR)/$(NAME)

macbook:
	@make -j all MACBOOK=1

debug:
	@make -j all DEBUG=1

$(BINDIR)/$(NAME): $(OBJECTS) $(LIBFT) $(LIBMAT)
	$(CC) -o $@ $^ $(LFLAGS)
	@echo "$(OK_COLOR)$(NAME) linked with success !$(EOC)"

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@make clean -C $(LIBFTDIR)
	@make clean -C $(LIBMATDIR)
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFTDIR)
	@make fclean -C $(LIBMATDIR)

re: fclean opti

.PHONY: all clean fclean re
