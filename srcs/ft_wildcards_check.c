/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcards_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:31:10 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 23:04:07 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_wc(int *i, char **pwd)
{
	*i = -1;
	*pwd = NULL;
	*pwd = getcwd(*pwd, 0);
}

int	ft_add_wildcards(t_big_token *b_tokens)
{
	int		i;
	int		j;
	char	*pwd;

	init_wc(&i, &pwd);
	if (!pwd)
		return (0);
	if (b_tokens->cmd_args)
	{
		while (b_tokens->cmd_args[++i])
		{
			j = -1;
			while (b_tokens->cmd_args[i][++j])
			{
				if (b_tokens->cmd_args[i][j] == '*'
				&& !ft_postype(b_tokens->cmd_args[i], j))
				{
					if (ft_do_wildcards(b_tokens, i, pwd))
						return (free(pwd), 1);
					break ;
				}
			}
		}
	}
	return (free(pwd), 0);
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

int	ft_dk_util(char **str, char *dir, int *i, int j)
{
	while (**str)
	{
		if (**str == '*' || **str == '/')
		{
			while (**str == '/')
			{
				(*str)++;
				if (**str && **str != '/')
					return (1);
			}
			while (**str == '*')
				(*str)++;
			if (!**str)
				return (-1);
		}
		else if (!ft_keep(*str, dir, i, j))
			(*str)++;
		else
			return (1);
	}
	return (0);
}
