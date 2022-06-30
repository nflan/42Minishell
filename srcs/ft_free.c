/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 21:19:16 by nflan             #+#    #+#             */
/*   Updated: 2022/06/24 16:34:07 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_wildcards(t_wildcards *wd)
{
	if (wd)
	{
		if (wd->next)
			ft_free_wildcards(wd->next);
		else
			closedir(wd->fd);
		if (wd)
			free(wd);
	}
}

void	ft_free_all(t_info *info, t_env *env)
{
	if (info)
	{
		if (info->rdline)
			free(info->rdline);
		info->rdline = NULL;
		if (info->tokens)
			ft_free_tokens(info->tokens);
		info->tokens = NULL;
		if (info->old_tokens)
			ft_free_tokens(info->old_tokens);
		info->old_tokens = NULL;
		if (info->parse)
			ft_free_b_tokens(info->parse);
		info->parse = NULL;
		if (info->pid)
			free(info->pid);
		info->pid = NULL;
	}
	if (env)
	{
		ft_free_env(env);
		if (info->home)
			free(info->home);
		info->home = NULL;
	}
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
	if (b_tokens->fd_out)
		ft_free_fd(b_tokens->fd_out);
	if (b_tokens->fd_in)
		ft_free_fd(b_tokens->fd_in);
	b_tokens = NULL;
}
