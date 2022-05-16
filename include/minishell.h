/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 15:10:15 by nflan             #+#    #+#             */
/*   Updated: 2022/05/16 13:14:39 by nflan            ###   ########.fr       */
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
	char			*cmd;
	int				fdin;
	int				fdout;
//	int				type;// && = 1 / || = 2 -> j'envoie dans X ou Y fonction au cas ou
//	unsigned int	prof;// profondeur
	struct s_tree	*tree;
}	t_cmd;

typedef struct s_tree
{
	t_cmd			*cmd;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef struct s_info
{
	char	*rdline;
	int		status;
	t_tree	*tree;
}	t_info;

#endif
