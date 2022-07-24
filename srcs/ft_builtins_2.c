/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 21:41:14 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 22:30:06 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_env_err(t_big_token *b, int i)
{
	if (i == 1)
	{
		ft_putstr_fd_3("env: ‘", b->cmd_args[1],
			"': No such file or directory\n", 2);
		return (127);
	}
	else if (i == 2)
	{
		ft_putstr_fd_3("env: ‘", b->cmd_args[1], "': Permission denied\n", 2);
		return (126);
	}
	return (0);
}

int	ft_env_error(t_big_token *b)
{
	int	i;

	i = 0;
	if (access(b->cmd_args[1], F_OK))
		return (ft_env_err(b, 1));
	else
	{
		if (!access(b->cmd_args[1], X_OK | R_OK)
			&& b->cmd_args[1][ft_strlen(b->cmd_args[1]) - 1] == '/')
		{
			i = open(b->cmd_args[1], O_DIRECTORY);
			if (i > 0)
				return (close(i), ft_env_err(b, 2));
			else
				return (ft_env_err(b, 1));
		}
		else
			return (ft_env_err(b, 1));
	}
	return (0);
}

int	ft_env(t_info *info, t_big_token *b_tok)
{
	t_env	*print;
	char	*line;

	print = info->env;
	line = NULL;
	if (print && !b_tok->cmd_args[1])
	{
		while (print)
		{
			if (print->value)
			{
				line = ft_strjoiiin(print->name, "=", print->value);
				line = ft_strjoin_free(line, "\n", 1);
				ft_putstr_fd(line, b_tok->fdout);
				free(line);
			}
			print = print->next;
		}
	}
	else if (b_tok->cmd_args[1])
		return (ft_env_error(b_tok));
	return (0);
}

int	ft_pwd(t_big_token *b_tok)
{
	ft_putstr_fd(ft_get_env_value(b_tok->info, "PWD"), b_tok->fdout);
	ft_putstr_fd("\n", b_tok->fdout);
	return (0);
}

int	ft_unset_name(t_env **tmp, char *name)
{
	if (!tmp || !name)
		return (1);
	if (ft_wordigit(name))
		return (ft_exp_err(name, 2), 2);
	while ((*tmp)->next)
	{
		if (!ft_strncmp(name, (*tmp)->next->name, ft_strlen(name) + 1))
			return (0);
		else
			*tmp = (*tmp)->next;
	}
	return (1);
}
