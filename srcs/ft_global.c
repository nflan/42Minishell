/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 15:29:38 by nflan             #+#    #+#             */
/*   Updated: 2022/05/23 15:53:02 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_cmd_path(t_info *info, t_cmd *cmd)
{
	char	*tofree;
	char	**path;
	int		i;

	i = 0;
	path = ft_split(ft_get_env_value(info, "PATH"), ':');
	tofree = ft_strjoiiin(path[i], "/", cmd->cmd_p[0]);
	if (!tofree)
		return (1);
	while (path[i] && access(tofree, X_OK) != 0)
	{
		free(tofree);
		tofree = ft_strjoiiin(path[i], "/", cmd->cmd_p[0]);
		if (!tofree)
			return (1);
		i++;
	}
	if (path[i])
	{
		free(cmd->cmd_p[0]);
		cmd->cmd_p[0] = ft_strdup_free(tofree);
	}
	if (!path[i] || !cmd->cmd_p[0])
		return (ft_free_split(path), free(tofree), 1);
	ft_free_split(path);
	return (0);
}

int	ft_command(t_info *info, t_cmd *cmd)
{
	if (!cmd->cmd)
		return (1);
	cmd->cmd_p = ft_split(cmd->cmd, ' ');
	if (!cmd->cmd_p)
		return (1);
	if (access(cmd->cmd_p[0], X_OK) == 0)
		return (0);
	else if (ft_get_env_value(info, "PATH") && cmd->cmd_p[0][1] != '.' && cmd->cmd_p[0][1] != '/')
	{
		if (ft_cmd_path(info, cmd))
			return (1);
	}
	else
		return (1);
	return (0);
}
