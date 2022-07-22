/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 17:30:47 by nflan             #+#    #+#             */
/*   Updated: 2022/07/21 20:06:42 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_error_2(int i, t_info *info, t_big_token *b_tokens)
{
	if (i == 4)
	{
		perror("Exec error");
		ft_close_fd(b_tokens);
		ft_free_all(info, info->env);
		rl_clear_history();
	}
	else if (i == 6)
		perror("Malloc error");
	else if (i == 7)
		perror("Here_doc error");
}

int	ft_error(int i, t_info *info, t_big_token *b_tokens)
{
	info->status = 1;
	if (i == 1)
	{
		dup2(b_tokens->fdin, info->pdes[0]);
		perror("FD error");
	}
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
	else if (i >= 4)
		ft_error_2(i, info, b_tokens);
	return (info->status);
}

void	ft_write(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(1, &str[i], 1);
}

int	ft_first_error(char *av1)
{
	ft_write("minishell: ");
	ft_write(av1);
	ft_write(": No such file or directory");
	return (1);
}
