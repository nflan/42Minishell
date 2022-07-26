/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 15:29:38 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 16:41:32 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_change_cmd(t_big_token *b_tokens, char *tofree)
{
	free(b_tokens->cmd_args[0]);
	b_tokens->cmd_args[0] = ft_strdup_free(tofree);
	if (!b_tokens->cmd_args[0])
		return (2);
	return (0);
}

int	ft_path(t_info *info, t_big_token *b_tokens, int err)
{
	char	*tofree;
	char	**path;
	int		i;

	i = 0;
	path = ft_split(ft_get_env_value(info, "PATH"), ':');
	tofree = ft_strjoiiin(path[i], "/", b_tokens->cmd_args[0]);
	if (!tofree)
		return (ft_free_split(path), ft_putstr_error("Malloc error\n"), 2);
	while (path[i] && access(tofree, X_OK | R_OK) != 0)
	{
		if (access(tofree, F_OK) == 0)
			err = 126;
		free(tofree);
		i++;
		tofree = ft_strjoiiin(path[i], "/", b_tokens->cmd_args[0]);
		if (!tofree)
			return (ft_free_split(path), ft_putstr_error("Malloc error\n"), 2);
	}
	if (!path[i])
		return (ft_free_split(path), free(tofree), err);
	else
		return (ft_free_split(path), ft_change_cmd(b_tokens, tofree));
}

int	ft_cmd_nopath(t_big_token *b_tokens)
{
	int	i;

	i = 0;
	if (!access(b_tokens->cmd_args[0], F_OK))
	{
		if (access(b_tokens->cmd_args[0], X_OK))
			return (0);
		if (!access(b_tokens->cmd_args[0], X_OK | R_OK))
		{
			i = open(b_tokens->cmd_args[0], O_DIRECTORY);
			if (i > 0)
				return (close(i), 2);
			return (0);
		}
	}
	return (0);
}

int	ft_is_cmd(t_big_token *b_tokens, t_info *info)
{
	int	i;

	i = -1;
	if (!ft_get_env_value(info, "PATH"))
		return (ft_cmd_nopath(b_tokens));
	while (b_tokens->cmd_args[0][++i])
	{
		if (b_tokens->cmd_args[0][i] == '/')
		{
			while (b_tokens->cmd_args[0][i] == '/')
				i++;
			if (!b_tokens->cmd_args[0][i])
				return (2);
			else
				return (0);
		}
	}
	return (1);
}

int	ft_commanding(t_info *info, t_big_token *b_tokens)
{
	int	err;
	int	err2;

	err = ft_command(info, b_tokens);
	if (err == 2)
		exit (ft_mal_err(info, info->env, NULL));
	err2 = ft_change__(info->env, b_tokens);
	if (err2 == 2)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	else if (err2)
		return (info->status = 1, 1);
	ft_free_split(b_tokens->envp);
	b_tokens->envp = ft_env_to_tab(info->env, NULL);
	if (!b_tokens->envp)
		exit (ft_mal_err(info, info->env, "Malloc error\n"));
	return (err);
}
