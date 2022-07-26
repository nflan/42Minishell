/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 21:28:26 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 15:24:05 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_init_pipex(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;
	int			i;
	int			status;

	tmp_b = b_tokens;
	i = 0;
	while (tmp_b && ++i)
		tmp_b = tmp_b->sibling;
	info->pid = ft_calloc(sizeof(pid_t), i);
	if (!info->pid)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	tmp_b = b_tokens;
	if (pipe(info->pdes) == -1)
		exit (ft_mal_err(info, info->env, "Pipe error\n"));
	ft_exec_pipex(info, b_tokens, info->pid);
	i = -1;
	while (++i < info->nb_cmd)
		waitpid(info->pid[i], &status, 0);
	ft_manage_sig(info, 2, status);
	if (info->pid)
		free(info->pid);
	info->pid = NULL;
	return (info->status);
}
