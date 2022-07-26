/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcards_do2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:42 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 00:36:04 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_create_begin(t_big_token *b_tokens, int count, char ***tmp, int i)
{
	tmp[0] = ft_calloc(sizeof(char *), b_tokens->cmd_args_num + count);
	if (!tmp[0])
		return (1);
	tmp[0] = ft_fill_old_args(b_tokens, tmp[0], i, 0);
	if (!tmp[0])
		return (1);
	return (0);
}

int	ft_end_realloc(t_big_token *b_toks, char **tmp)
{
	ft_free_split(b_toks->cmd_args);
	b_toks->cmd_args = tmp;
	return (0);
}

int	ft_check_dir_wd(t_wildcards *wd, t_big_token *b, int *it)
{
	if (wd->dir->d_type == 4
		&& b->cmd_args[it[0]][ft_strlen(b->cmd_args[it[0]]) - 1] == '/')
		return (1);
	return (0);
}

int	ft_realloc_args(t_wildcards *wd, t_big_token *b, int *it, int j)
{
	char	**tmp;
	int		count;

	count = ft_wd_nb_args(wd, b, it[0], it[1]);
	if (ft_create_begin(b, count, &tmp, it[0]))
		return (1);
	while (wd && j < count)
	{
		if (wd && !ft_do_keep(b->cmd_args[it[0]], wd, it[1], 0))
		{
			if (ft_check_dir_wd(wd, b, it))
				tmp[it[0] + j] = ft_strjoin(wd->dir->d_name, "/");
			else
				tmp[it[0] + j] = ft_strdup(wd->dir->d_name);
			if (!tmp[it[0] + j])
				return (ft_free_split(tmp), 1);
			j++;
		}
		wd = wd->next;
	}
	b->cmd_args_num += count - 1;
	tmp = ft_fill_old_args(b, tmp, j + it[0], 1);
	if (!tmp)
		return (1);
	return (ft_end_realloc(b, tmp), 0);
}
