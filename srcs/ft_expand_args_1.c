/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_args_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:10:36 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 00:43:56 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ra_sub(char **tmpj, char *cmd_arg)
{
	*tmpj = ft_strdup(cmd_arg);
	if (!*tmpj)
		return (1);
	return (0);
}

int	ft_reduce_args(t_big_token *b, size_t i)
{
	char	**tmp;
	size_t	j;

	j = 0;
	tmp = NULL;
	if (i)
	{
		tmp = ft_calloc(sizeof(char *), ft_tablen(b->cmd_args));
		if (!tmp)
			return (1);
		while (j < i)
		{
			if (ra_sub(&(tmp[j]), b->cmd_args[j]))
				return (ft_free_split(tmp), 1);
			j++;
		}
		while (j < ft_tablen(b->cmd_args) - 1)
		{
			if (ra_sub(&(tmp[j]), b->cmd_args[j + 1]))
				return (ft_free_split(tmp), 1);
			j++;
		}
	}
	ft_free_split(b->cmd_args);
	return (b->cmd_args = tmp, 0);
}

int	nqa_tool(t_big_token *b_tokens)
{
	int	i;

	i = 0;
	while (b_tokens->cmd_args && b_tokens->cmd_args[i])
	{
		if (!ft_strlen(b_tokens->cmd_args[i]))
		{
			if (ft_reduce_args(b_tokens, i))
				return (1);
		}
		else
		{
			if (ft_strlen(b_tokens->cmd_args[i])
				!= ft_strlen_nq(b_tokens->cmd_args[i])
				&& ft_strlen(b_tokens->cmd_args[i]))
			{
				b_tokens->cmd_args[i] = ft_noquote_line(b_tokens->cmd_args[i]);
				if (!b_tokens->cmd_args[i])
					return (1);
			}
			i++;
		}
	}
	return (0);
}

int	ft_noquote_args(t_big_token *b_tokens)
{
	if (b_tokens->cmd_args)
	{
		if (nqa_tool(b_tokens))
			return (1);
	}
	if (!b_tokens->cmd_args)
	{
		ft_free_split(b_tokens->cmd_args);
		return (2);
	}
	return (0);
}

char	*ft_expanded_value(t_info *info, char *tmp)
{
	char	*exp;

	exp = NULL;
	if (tmp && !ft_strncmp(tmp, "?", 2))
		exp = ft_itoa(info->status);
	else if (tmp)
		exp = ft_strdup(ft_get_env_value(info, tmp));
	if (!exp)
		return (free(tmp), NULL);
	return (free(tmp), exp);
}
