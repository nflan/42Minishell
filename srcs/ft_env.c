/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 21:41:14 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 19:20:02 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_env_err(t_big_token *b, int arg, int i)
{
	if (i == 1)
	{
		ft_putstr_fd_3("env: ‘", b->cmd_args[arg],
			"': No such file or directory\n", 2);
		return (127);
	}
	else if (i == 2)
	{
		ft_putstr_fd_3("env: ‘", b->cmd_args[arg], "': Permission denied\n", 2);
		return (126);
	}
	return (0);
}

int	ft_env_error(t_big_token *b, int arg)
{
	int	i;

	i = 0;
	if (access(b->cmd_args[arg], F_OK))
		return (ft_env_err(b, arg, 1));
	else
	{
		if (!access(b->cmd_args[arg], X_OK | R_OK)
			&& b->cmd_args[arg][ft_strlen(b->cmd_args[arg]) - 1] == '/')
		{
			i = open(b->cmd_args[arg], O_DIRECTORY);
			if (i > 0)
				return (close(i), ft_env_err(b, arg, 2), 2);
			else
				return (ft_env_err(b, arg, 1), 1);
		}
		else
			return (ft_env_err(b, arg, 1));
	}
	return (0);
}

int	ft_env_args(t_big_token *b_tok, int *i)
{
	while (b_tok->cmd_args[*i] && !ft_strncmp(b_tok->cmd_args[*i], "env", 4))
		(*i)++;
	if (b_tok->cmd_args[*i])
		return (1);
	return (0);
}

int	ft_env(t_info *info, t_big_token *b_tok, int i)
{
	t_env	*print;
	char	*line;

	print = info->env;
	line = NULL;
	if (b_tok->cmd_args[1] && b_tok->cmd_args[1][0] == '-')
		return (ft_putstr_error("minishell: env with option\n"), 2);
	if (b_tok->cmd_args[1])
		if (ft_env_args(b_tok, &i))
			return (ft_env_error(b_tok, i));
	if (print)
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
	return (0);
}
