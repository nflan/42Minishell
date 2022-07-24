/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcards_check_bis.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 15:42:51 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 15:46:54 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_do_keep(char *str, t_wildcards *wd, int type, int i)
{
	int		j;
	char	*dir;
	int		dk_util;

	dir = wd->dir->d_name;
	j = ft_strlen_before_ast(str);
	if (!str || !dir
		|| ft_manage_type(str, wd->dir->d_name, wd->dir->d_type, type))
		return (1);
	if (*str == '*')
		i++;
	dk_util = ft_dk_util(&str, dir, &i, j);
	if (dk_util == 1)
		return (1);
	else if (dk_util == -1)
		return (0);
	if ((!*str && dir[i]) || (*str && !dir[i]))
		return (1);
	return (0);
}

int	ft_wd_nb_args(t_wildcards *wd, t_big_token *b_tokens, int i, int type)
{
	int			count;

	count = 0;
	if (wd)
	{
		while (wd)
		{
			if (!ft_do_keep(b_tokens->cmd_args[i], wd, type, 0))
				count++;
			wd = wd->next;
		}
	}
	return (count);
}
