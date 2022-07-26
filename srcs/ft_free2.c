/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 17:46:15 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 16:44:43 by nflan            ###   ########.fr       */
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
	t_env	*tmp;

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

int	ft_mal_err(t_info *info, t_env *env, char *err)
{
	if (err)
		ft_putstr_fd(err, 2);
	if (info && env)
		ft_free_all(info, env);
	else if (info && !env)
		ft_free_all(info, NULL);
	return (1);
}
