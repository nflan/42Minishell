/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:22:55 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 18:14:00 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_ex_val(t_big_token *b)
{
	char		llmax[20] = "9223372036854775807";
	char		llmin[20] = "-9223372036854775808";
	int			i;

	i = 0;
	if (b->cmd_args[1][0] != '-')
	{
		if (ft_strlen(b->cmd_args[1]) == 19)
		{
			while (llmax[i] && b->cmd_args[1][i] && b->cmd_args[1][i] <= llmax[i])
				i++;
			if (llmax[i] || b->cmd_args[1][i])
				return (1);
		}
		else if (ft_strlen(b->cmd_args[1]) > 19)
			return (1);
	}
	else if (b->cmd_args[1][0] == '-')
	{
		if (ft_strlen(b->cmd_args[1]) == 20)
		{
			while (llmin[i] && b->cmd_args[1][i] && b->cmd_args[1][i] <= llmin[i])
				i++;
			if (llmin[i] || b->cmd_args[1][i])
				return (1);
		}
		else if (ft_strlen(b->cmd_args[1]) > 20)
			return (1);
	}
	return (0);
}

int	ft_mystic_exit(t_big_token *b, unsigned long long *ret)
{
	if (!b || !b->cmd_args[1])
		return (0);
	if (ft_ex_val(b) || ft_digital(b->cmd_args[1]))
	{
		ft_putstr_fd_3("minishell: exit: ", b->cmd_args[1],
			": numeric argument required\n", 2);
		*ret = 2;
		return (2);
	}
	else if (b->cmd_args[2])
	{
		ft_putstr_fd_3("minishell: exit: ", NULL, "too many arguments\n", 2);
		*ret = 1;
		return (1);
	}
	return (0);
}

int	ft_exit(t_info *info, t_big_token *b_tokens)
{
	unsigned long long	ret;

	ret = info->status;
	if (b_tokens && b_tokens->cmd_args[1])
		ret = ft_atoi(b_tokens->cmd_args[1]);
	ft_putstr_fd("exit\n", 2);
	if (ft_mystic_exit(b_tokens, &ret) != 1)
	{
		if (b_tokens)
			ft_free_all(info, info->env);
		rl_clear_history();
		exit(ret);
	}
	return (ret);
}

int	ft_env_err(t_big_token *b, int	i)
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
		if (!access(b->cmd_args[1], X_OK | R_OK) && b->cmd_args[1][ft_strlen(b->cmd_args[1]) - 1] == '/')
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

int	ft_unset(t_info *info, t_big_token *b_tokens)
{
	t_env	*tmp;
	t_env	*ptr;
	int		i;
	int		check;
	int		err;

	i = 1;
	err = 0;
	if (!info->env || !b_tokens->cmd_args[0] || !b_tokens->cmd_args[1])
		return (1);
	while (b_tokens->cmd_args[i])
	{
		ptr = NULL;
		tmp = info->env;
		check = ft_unset_name(&tmp, b_tokens->cmd_args[i]);
		if (!ft_strncmp(tmp->name, b_tokens->cmd_args[i], ft_strlen(tmp->name) + 1))
		{
			ptr = tmp;
			tmp = tmp->next;
			info->env = tmp;
		}
		else if (!check)
		{
			ptr = tmp->next;
			tmp->next = tmp->next->next;
		}
		else if (check == 2)
			err = 1;
		i++;
		if (ptr)
		{
			ptr->next = NULL;
			ft_free_env(ptr);
		}
	}
	return (err);
}
