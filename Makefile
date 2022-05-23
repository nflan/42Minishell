# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nflan <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/05 15:05:20 by nflan             #+#    #+#              #
#    Updated: 2022/05/23 14:25:20 by nflan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

vpath %.h libft
vpath ft_%.c libft
vpath ft_%.o libft

sources = srcs/main.c srcs/ft_cd.c srcs/ft_export.c srcs/ft_tools.c srcs/ft_builtins.c\
		  srcs/ft_pipex_tools.c srcs/ft_pipex.c srcs/ft_global.c srcs/ft_tools2.c

INC = include/minishell.h

objets = ${sources:.c=.o}

LIBFT = libft/libft.a

NAME = minishell

DEBUG = -g3 -fsanitize=address

.c.o :
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

all:	${NAME}

$(NAME):	${objets} ${INC} ${HEADER} ${LIBFT}
	${CC} ${CFLAGS} ${objets} ${LIBFT} -I ${INC} -lreadline -o ${NAME}

-include libft/Makefile

ft_%.o : ft_%.c
	${CC} ${CFLAGS} -c $< -o $@

$(LIBFT):	${SRCS} ${OBJS} libft.h
	${MAKE} -C libft/

$(objets):	${INC}

clean:
	$(MAKE) clean -C libft/
	${RM} ${objets} 

fclean:
	$(MAKE) fclean -C libft/
	${RM} ${NAME}
	${RM} ${objets}

re: fclean all

.PHONY: all clean fclean re
