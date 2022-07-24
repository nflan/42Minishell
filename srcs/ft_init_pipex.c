/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 21:28:26 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 22:18:45 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_init_pipex(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;
	int			i;

	tmp_b = b_tokens;
	i = 0;
	while (tmp_b && ++i)
		tmp_b = tmp_b->sibling;
	info->pid = ft_calloc(sizeof(pid_t), i);
	if (!info->pid)
		return (1);
	tmp_b = b_tokens;
	if (pipe(info->pdes) == -1)
		return (free(info->pid), ft_error(5, info, NULL));
	ft_exec_pipex(info, b_tokens, info->pid);
	i = -1;
	while (++i < info->nb_cmd)
		waitpid(info->pid[i], &info->pid[i], 0);
	ft_manage_sig(info, 2, info->pid[i]);
	if (info->pid)
		free(info->pid);
	info->pid = NULL;
	return (info->status);
}
