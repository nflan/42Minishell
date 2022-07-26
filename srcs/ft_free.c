/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 21:19:16 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 16:44:30 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_all2(t_info *info, t_env *env)
{
	ft_free_env(env);
	if (info->home)
		free(info->home);
	info->home = NULL;
	if (info->rdline)
		free(info->rdline);
	info->rdline = NULL;
}

void	ft_free_all(t_info *info, t_env *env)
{
	if (info)
	{
		if (info->tokens)
			ft_free_tokens(info->tokens);
		info->tokens = NULL;
		if (info->parse)
			ft_free_b_tokens(info->parse);
		info->parse = NULL;
		if (info->pid)
			free(info->pid);
		if (info->pdes[0] && info->pdes[0] > 2)
			ft_close_pdes(info->pdes[1], 1);
		if (info->pdes[1] && info->pdes[1] > 2)
			ft_close_pdes(info->pdes[0], 0);
		if (info->tmp[0] && info->tmp[0] > 2)
			ft_close_pdes(info->tmp[0], 0);
		if (info->tmp[1] && info->tmp[1] > 2)
			ft_close_pdes(info->tmp[1], 1);
		info->pid = NULL;
	}
	if (env)
		ft_free_all2(info, env);
}

void	ft_free_b_tokens(t_big_token *b_tokens)
{
	t_big_token	*tmp_b;
	t_big_token	*tmp;

	tmp_b = b_tokens;
	tmp = NULL;
	if (!b_tokens)
		return ;
	while (tmp_b)
	{
		if (tmp_b->child)
			ft_free_b_tokens(tmp_b->child);
		tmp = tmp_b;
		tmp_b = tmp_b->sibling;
		if (tmp)
			ft_free_cmd(tmp);
		free(tmp);
		tmp = NULL;
	}
	b_tokens = NULL;
}

void	ft_free_fd(t_fd *fd)
{
	if (fd)
	{
		if (fd->next)
			ft_free_fd(fd->next);
		if (fd->file)
			free(fd->file);
		if (fd->delimitator)
			free(fd->delimitator);
		free(fd);
		fd = NULL;
	}
}

void	ft_free_cmd(t_big_token *b_tokens)
{
	if (b_tokens->cmd_args)
		ft_free_split(b_tokens->cmd_args);
	if (b_tokens->envp)
		ft_free_split(b_tokens->envp);
	if (b_tokens->fd)
	{
		ft_close_fd(b_tokens);
		ft_free_fd(b_tokens->fd);
	}
	b_tokens = NULL;
}
