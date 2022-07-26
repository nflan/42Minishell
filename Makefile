# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/17 17:27:46 by nflan             #+#    #+#              #
#    Updated: 2022/07/26 16:54:13 by nflan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


vpath %.h libft
vpath ft_%.c libft
vpath ft_%.o libft

SRC_DIR = srcs/
OBJ_DIR = bin/
INC_DIR = include/
LIB_DIR = libft/

sources = main.c ft_cd.c ft_export.c ft_tools.c ft_cd_2.c\
		  ft_here_doc.c ft_pipex.c ft_global.c ft_tools2.c\
		  big_tokenizer_1.c big_tokenizer_2.c big_tokenizer_3.c parser.c\
		  big_tokenizer_3_tool.c tokenizer_1.c tokenizer_1_tool.c tokenizer_3.c\
		  tokenizer_2.c ft_exec.c syntax_errorinizer_1.c syntax_errorinizer_2.c\
		  syntax_errorinizer_3.c ft_launch_built.c ft_launch_cmd.c\
		  ft_launch_tools.c ft_echo.c main_O.c ft_executer.c\
		  ft_executer_tools.c ft_free.c ft_free2.c ft_expand_check.c\
		  ft_fd_open.c ft_fd_close.c  ft_wildcards_check.c ft_wildcards_tools.c\
		  ft_wildcards_do.c ft_signal.c ft_wildcards_do2.c init_tok_type_tab.c\
		  ft_start.c ft_env_tools.c ft_env_tools2.c big_tokenizer_5.c\
		  big_tokenizer_6.c ft_expand_tools.c ft_expand_line.c\
		  ft_expand_args_1.c ft_expand_args_2.c ft_expand_args_3.c\
		  ft_fd_tools.c big_tokenizer_4-1.c handle_dir.c handle_par_dir.c\
		  handle_par.c big_tokenizer_8.c big_tokenizer_9.c ft_init_pipex.c\
		  big_tokenizer_4_tool.c syntax_errorinizer_4.c ft_export_tools.c\
		  ft_wildcards_check_bis.c ft_export_bis.c ft_fd_tools_bis.c\
		  ft_cd_tools.c ft_global_bis.c ft_builtins.c ft_exit.c ft_env.c\
		  ft_pipex_2.c

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
