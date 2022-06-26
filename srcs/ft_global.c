/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 15:29:38 by nflan             #+#    #+#             */
/*   Updated: 2022/06/23 17:50:24 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	while (path[i] && access(tofree, X_OK) != 0)
	{
		free(tofree);
		tofree = ft_strjoiiin(path[i], "/", b_tokens->cmd_args[0]);
		if (!tofree)
			return (ft_free_split(path), ft_putstr_error("Malloc error path\n"));
		i++;
	}
	if (path[i])
	{
		free(b_tokens->cmd_args[0]);
		b_tokens->cmd_args[0] = ft_strdup_free(tofree);
	}
	if (!path[i] || !b_tokens->cmd_args[0])
		return (ft_free_split(path), free(tofree), 1);
	return (ft_free_split(path), 0);
}

int	ft_command(t_info *info, t_big_token *b_tokens)
{
	if (!b_tokens->cmd_args)
		return (1);
	if (access(b_tokens->cmd_args[0], X_OK) == 0)
		return (0);
	else if (ft_get_env_value(info, "PATH")
		&& b_tokens->cmd_args[0][1] != '.' && b_tokens->cmd_args[0][1] != '/')
	{
		if (ft_path(info, b_tokens))
			return (1);
	}
	else
		return (1);
	return (0);
}
