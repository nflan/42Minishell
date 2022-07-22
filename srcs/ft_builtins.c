/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:22:55 by nflan             #+#    #+#             */
/*   Updated: 2022/07/22 12:09:24 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_mystic_exit(t_big_token *b, unsigned long long *ret)
{
	if (!b || !b->cmd_args[1])
		return (0);
	if (*ret > 9223372036854775807 || ft_digital(b->cmd_args[1]))
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
	ft_putstr_fd("exit\n", 1);
	if (ft_mystic_exit(b_tokens, &ret) != 1)
	{
		if (b_tokens)
			ft_free_all(info, info->env);
		rl_clear_history();
		exit(ret);
	}
	return (ret);
}

int	ft_env(t_info *info, t_big_token *b_tok)
{
	t_env	*print;
	char	*line;

	print = info->env;
	line = NULL;
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

int	ft_pwd(t_big_token *b_tok)
{
	char	*buf;
	char	*tofree;

	buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		return (1);
	tofree = buf;
	buf = ft_strjoin(buf, "\n");
	ft_putstr_fd(buf, b_tok->fdout);
	free(buf);
	free(tofree);
	return (0);
}

int	ft_unset_name(t_env **tmp, char *name)
{
	if (!tmp || !name)
		return (1);
	if (ft_wordigit(name))
		return (ft_exp_err(name, 2));
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

	tmp = info->env;
	ptr = NULL;
	if (!tmp || !b_tokens->cmd_args[0])
		return (1);
	if (!ft_strncmp(tmp->name, b_tokens->cmd_args[1], ft_strlen(tmp->name) + 1))
	{
		ptr = tmp;
		tmp = tmp->next;
		info->env = tmp;
	}
	else if (!ft_unset_name(&tmp, b_tokens->cmd_args[1]))
	{
		ptr = tmp->next;
		tmp->next = tmp->next->next;
	}
	else
		return (1);
	ptr->next = NULL;
	ft_free_env(ptr);
	return (0);
}
