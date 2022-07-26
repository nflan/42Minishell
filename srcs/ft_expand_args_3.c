/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_args_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 18:31:14 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 12:06:27 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	resplit_tool_3(char ***tmp, int len1, int len2)
{
	*tmp = ft_calloc(sizeof(char *), len1 + len2);
	if (!*tmp)
		return (1);
	return (0);
}

int	ft_resplit(t_big_token *b, int *i, char *str)
{
	char	**tmp;
	int		jl[2];

	if (resplit_tool_3(&tmp, ft_tablen(b->cmd_args), ft_splitlen(str)))
		return (1);
	resplit_tool_2(&jl, *i, NULL, 1);
	while (jl[0] < *i)
	{
		if (resplit_tool(&(jl), tmp, b->cmd_args[(jl[0])], 1))
			return (ft_free_split(tmp), 1);
	}
	*i = *i + ft_splitlen(str) - 1;
	while ((jl[0]) <= *i)
	{
		if (resplit_tool(&(jl), tmp, str, 2))
			return (ft_free_split(tmp), 1);
	}
	while (b->cmd_args[(jl[1])])
	{
		if (resplit_tool(&(jl), tmp, b->cmd_args[(jl[1])], 3))
			return (ft_free_split(tmp), 1);
	}
	resplit_tool_2(&jl, *i, b, 2);
	b->cmd_args = tmp;
	return (b->cmd_args = tmp, 0);
}

int	ft_expand_args(t_big_token *b, t_info *info)
{
	int	i;

	i = -1;
	if (b)
	{
		if (!ft_check_expand(info->tokens, b->ind_tok_start, b->length))
			return (0);
		while (b->cmd_args && b->cmd_args[++i])
		{
			b->cmd_args[i] = ft_expand_l(b->cmd_args[i], info, 0);
			if (!b->cmd_args[i])
				return (1);
			if (b->cmd_args[i][0] != '\0'
					&& ft_strncmp(b->cmd_args[0], "export", 7))
				if (ft_resplit(b, &i, b->cmd_args[i]))
					return (1);
		}
	}
	return (0);
}
