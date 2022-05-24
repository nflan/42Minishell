/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:30:47 by nflan             #+#    #+#             */
/*   Updated: 2022/05/24 11:44:18 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_error_2(t_info *info, t_cmd *cmd)
{
	dup2(cmd->fdin, info->pdes[0]);
	(void) info;
	perror("FD error");
}

int	ft_error(int i, t_info *info, t_cmd *cmd)
{
	info->status = 1;
	if (i == 1)
		ft_error_2(info, cmd);
	else if (i == 2 || i == 5)
	{
		if (i == 2)
			perror("Child error");
		else if (i == 5)
			perror("Pipe error");
		ft_free_all(info, info->env);
		exit(1);
	}
	else if (i == 3)
	{
		info->status = 127;
		perror("Command not found");
	}
	else if (i == 4)
		perror("Exec error");
	else if (i == 6)
		perror("Malloc error");
	else if (i == 7)
		perror("Here_doc error");
	return (info->status);
}
