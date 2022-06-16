# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/16 17:53:03 by omoudni           #+#    #+#              #
#    Updated: 2022/06/16 19:22:37 by omoudni          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

vpath %.h libft
vpath ft_%.c libft
vpath ft_%.o libft

SRC_DIR = srcs/
OBJ_DIR = bin/
INC_DIR = include/
LIB_DIR = libft/

SRCS = main.c ft_cd.c ft_export.c ft_tools.c ft_builtins.c\
		  ft_pipex_tools.c ft_pipex.c ft_global.c ft_tools2.c\
		  big_tokenizer_1.c big_tokenizer_2.c big_tokenizer_3.c\
		  big_tokenizer_4.c parser.c tokenizer_1.c tokenizer_2.c\
		  syntax_errorinizer_1.c syntax_errorinizer_2.c\
		  tree_manipulator.c ft_launch_cmd.c ft_echo.c main_O.c\
		  printer.c executer.c

OBJS = $(addprefix $(OBJ_DIR), $(SRCS:%.c=%.o))

MINISHELL = minishell

LIBPATH = -L$(LIB_DIR)
INCPATH = -I$(INC_DIR) -I$(LIB_DIR)
LIB = ft

all: $(MINISHELL)

LIBFT:
	make -C $(LIB_DIR)

$(MINISHELL): $(OBJS) LIBFT
	$(CC) $(OBJS) $(CFLAGS) $(LIBPATH) -l$(LIB) -lreadline -o $(MINISHELL)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(INCPATH) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	make clean -C $(LIB_DIR)
	rm -rf $(OBJ_DIR)

fclean:
	rm -rf $(OBJ_DIR)
	make fclean -C $(LIB_DIR)
	rm -f $(minishell)

re: fclean all

.PHONY: all clean fclean re libft
