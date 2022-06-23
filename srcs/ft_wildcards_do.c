/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcards_do.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:34:22 by nflan             #+#    #+#             */
/*   Updated: 2022/06/23 18:36:46 by nflan            ###   ########.fr       */
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
	//		printf("i = %d && j = %d && cmd_args[%d] = %s\n", i, j, i, b_tokens->cmd_args[i]);
			tmp[j] = ft_strdup(b_tokens->cmd_args[i]);
			if (!tmp[j])
				return (ft_free_split(tmp), NULL);
			j++;
		}
	}
	return (tmp);
}

int	ft_realloc_args(t_wildcards *wd, t_big_token *b_tokens, int i, int type)
{
	char	**tmp;
	int		count;
	int		j;

	j = 0;
	count = ft_wd_nb_args(wd, b_tokens, i, type);
	printf("count = %d\n", count);
	if (!count)
		return (0);
	tmp = ft_calloc(sizeof(char *), b_tokens->cmd_args_num + count);
	if (!tmp)
		return (ft_putstr_error("Malloc error in realloc args: "));
	tmp = ft_fill_old_args(b_tokens, tmp, i, 0);
	if (!tmp)
		return (ft_putstr_error("ft_fill_old_args error: "));
	while (wd && j < count)
	{
		if (wd && !ft_do_keep(b_tokens->cmd_args[i], wd, type))
		{
			if (wd->dir->d_type == 4 && b_tokens->cmd_args[i][ft_strlen(b_tokens->cmd_args[i]) - 1] == '/')
				tmp[i + j] = ft_strjoin(wd->dir->d_name, "/");
			else
				tmp[i + j] = ft_strdup(wd->dir->d_name);
			if (!tmp[i + j])
				return (ft_free_split(tmp), 1);
			j++;
		}
		wd = wd->next;
	}
	b_tokens->cmd_args_num += count - 1;
	tmp = ft_fill_old_args(b_tokens, tmp, j + i, 1);
	if (!tmp)
		return (ft_putstr_error("ft_fill_old_args error: "));
	ft_free_split(b_tokens->cmd_args);
	b_tokens->cmd_args = tmp;
	return (0);
}

int	ft_do_wildcards(t_big_token *b_tokens, int i)
{
	t_wildcards	*wd;
	int			type;

	type = 8;
	wd = NULL;
	if (ft_get_wildcards(&wd))
		return (ft_free_wildcards(wd), 1);
	ft_print_wildcards(wd);
	if (b_tokens->cmd_args[i][ft_strlen(b_tokens->cmd_args[i]) - 1] == '/')
		type = 4;
	if (ft_realloc_args(wd, b_tokens, i, type))
		return (ft_free_wildcards(wd), 1);
	ft_free_wildcards(wd);
	return (0);
}
