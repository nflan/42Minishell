/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:30:47 by nflan             #+#    #+#             */
/*   Updated: 2022/06/15 17:44:40 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_error_2(t_info *info, t_big_token *b_tokens)
{
	dup2(b_tokens->fdin[b_tokens->rd_inouthd[0]], info->pdes[0]);
	(void) info;
	perror("FD error");
}

int	ft_error(int i, t_info *info, t_big_token *b_tokens)
{
	info->status = 1;
	if (i == 1)
		ft_error_2(info, b_tokens);
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
	{
		perror("Exec error");
		ft_free_all(info, info->env);
	}
	else if (i == 6)
		perror("Malloc error");
	else if (i == 7)
		perror("Here_doc error");
	return (info->status);
}
