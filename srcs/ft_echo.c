/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 17:15:09 by nflan             #+#    #+#             */
/*   Updated: 2022/06/06 17:37:09 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_option_echo(t_cmd *cmd, int i)
{
	int	option;
	int	y;

	option = 1;
	if (!cmd->cmd_p[i])
		return (1);
	while (cmd->cmd_p[i])
	{
		y = -1;
		if ((cmd->cmd_p[i][++y] == '-' && cmd->cmd_p[i][++y] == 'n'))
		{
			while (cmd->cmd_p[i][y] && cmd->cmd_p[i][y] == 'n')
				y++;
			if (!cmd->cmd_p[i][y])
				option++;
		}
		i++;
	}
	return (option);
}

int	ft_handle_ret(t_cmd *cmd, char *ret, int i)
{
	char	*tofree;
	int		y;

	tofree = cmd->cmd_p[i];
	y = 0;
	if (!tofree)
		return (free(ret), 1);
	while (tofree[y])
	{
		if (tofree[y + 1] && tofree[y] == '$' && tofree[y + 1] == '?')
		{
			cmd->cmd_p[i] = ft_substr(tofree, 0, y);
			cmd->cmd_p[i] = ft_strjoiiin_free(cmd->cmd_p[i], ret, tofree + y + 2, 1);
		}
		y++;
	}
	free(ret);
	free(tofree);
	return (0);
}

int	ft_echo_none(t_cmd *cmd, int i)
{
	if (i == 1)
		ft_putstr_fd("\n", cmd->fdout);
	return (0);
}

char	*ft_create_echo(t_info *info, t_cmd *cmd, char *tmp, int i)
{
	while (cmd->cmd_p[i])
	{
		if (ft_strnstr(cmd->cmd_p[i], "$?", ft_strlen(cmd->cmd_p[i])))
			if (ft_handle_ret(cmd, ft_itoa(info->status), i))
				return (NULL);
		if (!tmp)
			tmp = ft_strdup(cmd->cmd_p[i]);
		else
			tmp = ft_strjoiiin_free(tmp, " ", cmd->cmd_p[i], 1);
		if (!tmp)
			return (NULL);
		i++;
	}
	return (tmp);
}

int	ft_echo(t_info *info, t_cmd *cmd)
{
	char	*tmp;
	int		i;
	int		option;

	tmp = NULL;
	option = 0;
	i = ft_option_echo(cmd, 1);
	if (i > 1)
		option = 1;
	if (!cmd->cmd_p[i])
		return (ft_echo_none(cmd, i));
	if (cmd->cmd_p[i])
		tmp = ft_create_echo(info, cmd, tmp, i);
	if (!tmp)
		return (1);
	if (!option)
		tmp = ft_strjoin_free(tmp, "\n", 1);
	ft_putstr_fd(tmp, cmd->fdout);
	free(tmp);
	return (0);
}
