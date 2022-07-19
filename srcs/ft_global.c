/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 15:29:38 by nflan             #+#    #+#             */
/*   Updated: 2022/06/27 15:56:50 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_change_cmd(t_big_token *b_tokens, char *tofree)
{
	free(b_tokens->cmd_args[0]);
	b_tokens->cmd_args[0] = ft_strdup_free(tofree);
	if (!b_tokens->cmd_args[0])
		return (1);
	return (0);
}

int	ft_path(t_info *info, t_big_token *b_tokens)
{
	char	*tofree;
	char	**path;
	int		i;

	i = 0;
	path = ft_split(ft_get_env_value(info, "PATH"), ':');
	tofree = ft_strjoiiin(path[i], "/", b_tokens->cmd_args[0]);
	if (!tofree)
		return (ft_free_split(path), ft_putstr_error("Malloc error ft_path\n"));
	while (path[i] && access(tofree, X_OK | R_OK) != 0)
	{
		//demander conseil a Brice car le /bin/ls pas droit mais ptete le /usr/bin/ls si
		free(tofree);
		tofree = ft_strjoiiin(path[i], "/", b_tokens->cmd_args[0]);
		if (!tofree)
			return (ft_free_split(path), ft_putstr_error("Malloc error path\n"));
		i++;
	}
	if (!path[i])
		return (ft_free_split(path), free(tofree), 127);
	else
		return (ft_free_split(path), ft_change_cmd(b_tokens, tofree));
}

int	ft_is_cmd(t_big_token *b_tokens)
{
	if (b_tokens->cmd_args[0][0] == '.' && b_tokens->cmd_args[0][1] == '/')
		return (0);
	if (b_tokens->cmd_args[0][0] == '/' || b_tokens->cmd_args[0][1] == '/')
		return (0);
	return (1);
}

int	ft_command(t_info *info, t_big_token *b_tokens)
{
	if (!b_tokens->cmd_args)
		return (1);
	if (!ft_is_cmd(b_tokens))
	{
		if (access(b_tokens->cmd_args[0], F_OK) == 0)
		{
			if (access(b_tokens->cmd_args[0], X_OK) != 0)
				return (126);
			else
				return (0);
		}
		else
			return (-4);
	}
	else if (ft_get_env_value(info, "PATH")
		&& b_tokens->cmd_args[0][1] != '.' && b_tokens->cmd_args[0][1] != '/')
		return (ft_path(info, b_tokens));
	else
		return (127);
	return (0);
}
