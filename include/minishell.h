/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 15:10:15 by nflan             #+#    #+#             */
/*   Updated: 2022/05/13 15:17:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_cmd
{
	char			*line;
	int				fdin;
	int				fdout;
	int				index;
	int				type;// && = 1 / || = 2
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_tree
{
	t_cmd			*cmd;
	unsigned int	prof;// profondeur
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	*parent;
}	t_tree;

typedef struct s_info
{
	char	*rdline;
	int		nbdiff;
	t_tree	*tree;
}	t_info;

#endif
