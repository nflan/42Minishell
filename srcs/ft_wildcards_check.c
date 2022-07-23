/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcards_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:31:10 by nflan             #+#    #+#             */
/*   Updated: 2022/07/23 16:36:43 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_add_wildcards(t_big_token *b_tokens)
{
	int	i;
	int	j;

	i = 0;
	if (b_tokens->cmd_args)
	{
		while (b_tokens->cmd_args[i])
		{
			j = 0;
			while (b_tokens->cmd_args[i][j])
			{
				if (b_tokens->cmd_args[i][j] == '*'
				&& !ft_postype(b_tokens->cmd_args[i], j))
				{
					if (ft_do_wildcards(b_tokens, i))
						return (1);
					break ;
				}
				j++;
			}
			i++;
		}
	}
	return (0);
}

int	ft_keep(char *str, char *dir, int *i, int j)
{
	if (*str == '*')
		return (0);
	if (*i < j)
	{
		if (*str != dir[*i])
			return (1);
		*i += 1;
	}
	else
	{
		while (dir[*i] && *str != dir[*i])
			*i += 1;
		if (!dir[*i] && *str)
			return (1);
		if (dir[*i] && dir[*i] == *str)
			*i += 1;
	}
	return (0);
}

int	ft_do_keep(char *str, t_wildcards *wd, int type, int i)
{
	int		j;
	char	*dir;

	dir = wd->dir->d_name;
	j = ft_strlen_before_ast(str);
	if (!str || !dir
		|| ft_manage_type(str, wd->dir->d_name, wd->dir->d_type, type))
		return (1);
	if (*str == '*')
		i++;
	while (*str)
	{
		if (*str == '*' || *str == '/')
		{
			while (*str == '/')
			{
				str++;
				if (*str && *str != '/')
					return (1);
			}
			while (*str == '*')
				str++;
			if (!*str)
				return (0);
		}
		else if (!ft_keep(str, dir, &i, j))
			str++;
		else
			return (1);
	}
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
