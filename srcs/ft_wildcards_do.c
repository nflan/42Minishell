/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcards_do.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:34:22 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 00:36:51 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_fill_old_args(t_big_token *b_tokens, char **tmp, int j, int list)
{
	static int	i = 0;

	if (!list)
	{
		i = 0;
		while (i < j)
		{
			tmp[i] = ft_strdup(b_tokens->cmd_args[i]);
			if (!tmp[i])
				return (ft_free_split(tmp), NULL);
			i++;
		}
	}
	else
	{
		while (b_tokens->cmd_args[i] && j < b_tokens->cmd_args_num)
		{
			i++;
			tmp[j] = ft_strdup(b_tokens->cmd_args[i]);
			if (!tmp[j])
				return (ft_free_split(tmp), NULL);
			j++;
		}
	}
	return (tmp);
}

int	ft_do_wildcards(t_big_token *b_tokens, int i, char *pwd)
{
	t_wildcards	*wd;
	int			itype[2];

	itype[0] = i;
	itype[1] = 8;
	wd = NULL;
	if (ft_get_wildcards(&wd, pwd))
		return (ft_free_wildcards(wd), 1);
	if (b_tokens->cmd_args[i][ft_strlen(b_tokens->cmd_args[i]) - 1] == '/')
		itype[1] = 4;
	if (ft_wd_nb_args(wd, b_tokens, i, itype[1]))
		if (ft_realloc_args(wd, b_tokens, itype, 0))
			return (ft_free_wildcards(wd), 1);
	ft_free_wildcards(wd);
	return (0);
}
