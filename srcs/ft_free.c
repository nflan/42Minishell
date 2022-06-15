/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 21:19:16 by nflan             #+#    #+#             */
/*   Updated: 2022/06/15 21:50:10 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_all(t_info *info, t_env *env)
{
	if (info)
	{
		if (info->rdline)
			free(info->rdline);
		info->rdline = NULL;
	//	if (info->pdes[0] != -1)
	//		close(info->pdes[0]);
	//	if (info->pdes[1] != -1)
	//		close(info->pdes[1]);
		if (info->tokens)
		{
			ft_free_tokens(info->tokens);
			info->tokens = NULL;
		}
		if (info->parse)
		{
			ft_free_b_tokens(info->parse);
			info->parse = NULL;
		}
	}
	if (env)
		ft_free_env(env);
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
}

void	ft_free_cmd(t_big_token *b_tokens)
{
	if (b_tokens->red_in)
		free(b_tokens->red_in);
	if (b_tokens->red_out)
		free(b_tokens->red_out);
	if (b_tokens->delimitator)
		ft_free_split(b_tokens->delimitator);
	if (b_tokens->infile)
		ft_free_split(b_tokens->infile);
	if (b_tokens->outfile)
		ft_free_split(b_tokens->outfile);
	if (b_tokens->fdin)
		free(b_tokens->fdin);
	if (b_tokens->fdout)
		free(b_tokens->fdout);
	if (b_tokens->err_in)
		free(b_tokens->err_in);
	if (b_tokens->err_out)
		free(b_tokens->err_out);
	if (b_tokens->cmd_args)
		ft_free_split(b_tokens->cmd_args);
	if (b_tokens->envp)
		ft_free_split(b_tokens->envp);
	b_tokens = NULL;
}

void	ft_free_tokens(t_token *tokens)
{
	if (!tokens)
		return ;
	if (tokens->next)
		ft_free_tokens(tokens->next);
	if (tokens->value)
	{
		free(tokens->value);
		tokens->value = NULL;
	}
	free(tokens);
	tokens = NULL;
}

void	ft_free_env(t_env *env)
{
	t_env *tmp;

	tmp = NULL;
	if (!env)
		return ;
	while (env)
	{
		tmp = env;
		env = tmp->next;
		if (tmp->name)
			free(tmp->name);
		tmp->name = NULL;
			if (tmp->value)
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}
