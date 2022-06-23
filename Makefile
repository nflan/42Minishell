# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/17 17:27:46 by nflan             #+#    #+#              #
#    Updated: 2022/06/23 17:52:12 by nflan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

vpath %.h libft
vpath ft_%.c libft
vpath ft_%.o libft

SRC_DIR = srcs/
OBJ_DIR = bin/
INC_DIR = include/
LIB_DIR = libft/

sources = main.c ft_cd.c ft_export.c ft_tools.c ft_builtins.c\
		  ft_here_doc.c ft_pipex.c ft_global.c ft_tools2.c\
		  big_tokenizer_1.c big_tokenizer_2.c big_tokenizer_3.c\
		  big_tokenizer_4.c parser.c tokenizer_1.c tokenizer_2.c\
		  syntax_errorinizer_1.c syntax_errorinizer_2.c\
		  tree_manipulator.c ft_launch_cmd.c ft_echo.c main_O.c\
		  printer.c executer.c ft_free.c ft_free2.c dollar_expander.c\
		  ft_wildcards_check.c ft_wildcards_tools.c ft_wildcards_do.c ft_signal.c

INC = include/minishell.h

objets = $(addprefix $(OBJ_DIR), $(sources:%.c=%.o))

LIBFT = libft/libft.a

NAME = minishell

LIBPATH = -L $(LIB_DIR)
INCPATH = -I $(INC_DIR) -I $(LIB_DIR)

.c.o :
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

all:	${NAME}

$(NAME):	${objets} ${INC} ${HEADER} ${LIBFT}
	${CC} ${CFLAGS} ${objets} ${LIBFT} -I ${INC} -lreadline -o ${NAME}

-include libft/Makefile

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):	${SRCS} ${OBJS} libft.h
	${MAKE} -C libft/

$(objets):	${INC}

clean:
	$(MAKE) clean -C $(LIB_DIR)
	${RM} ${OBJ_DIR}

fclean:
	$(MAKE) fclean -C $(LIB_DIR)
	${RM} ${OBJ_DIR}
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
