/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 15:29:38 by nflan             #+#    #+#             */
/*   Updated: 2022/07/21 09:20:09 by nflan            ###   ########.fr       */
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

int	ft_path(t_info *info, t_big_token *b_tokens, int err)
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
		if (access(tofree, F_OK) == 0)
			err = 126;
		free(tofree);
		i++;
		tofree = ft_strjoiiin(path[i], "/", b_tokens->cmd_args[0]);
		if (!tofree)
			return (ft_free_split(path), ft_putstr_error("Malloc error path\n"));
	}
	if (!path[i])
		return (ft_free_split(path), free(tofree), err);
	else
		return (ft_free_split(path), ft_change_cmd(b_tokens, tofree));
}

int	ft_is_cmd(t_big_token *b_tokens)
{
	if (b_tokens->cmd_args[0][0] == '.' && b_tokens->cmd_args[0][1] == '/'
			&& ft_strlen(b_tokens->cmd_args[0]) == 2)
		return (2);
	if (b_tokens->cmd_args[0][0] == '.' && b_tokens->cmd_args[0][1] == '/')
		return (0);
	if (b_tokens->cmd_args[0][0] == '/' || b_tokens->cmd_args[0][1] == '/')
		return (0);
	return (1);
}

int	ft_command(t_info *info, t_big_token *b_tokens)
{
	if (b_tokens->cmd_args[0][0] == '\0')
		return (127);
	if (!b_tokens->cmd_args || !ft_strlen(b_tokens->cmd_args[0]))
		return (1);
	if (ft_is_cmd(b_tokens) == 2)
		return (-4);
	if (!ft_is_cmd(b_tokens) || !ft_get_env_value(info, "PATH"))
	{
		if (access(b_tokens->cmd_args[0], F_OK) == 0)
			if (access(b_tokens->cmd_args[0], X_OK) != 0)
				return (126);
		return (0);
	}
	else if (ft_get_env_value(info, "PATH"))
		return (ft_path(info, b_tokens, 127));
	else if (access(b_tokens->cmd_args[0], F_OK == 0))
		return (0);
	else
		return (127);
	return (0);
}
