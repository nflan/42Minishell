/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:22:55 by nflan             #+#    #+#             */
/*   Updated: 2022/07/25 19:29:55 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_ex_val_1(int *i, char *cmd_args_1)
{
	if (ft_strlen(cmd_args_1) == 19)
	{
		while (LLMAX[*i] && cmd_args_1[*i] && cmd_args_1[*i] <= LLMAX[*i])
			(*i)++;
		if (LLMAX[*i] || cmd_args_1[*i])
			return (1);
	}
	else if (ft_strlen(cmd_args_1) > 19)
		return (1);
	return (0);
}

int	ft_ex_val_2(int *i, char *cmd_args_1)
{
	if (ft_strlen(cmd_args_1) == 20)
	{
		while (LLMIN[*i] && cmd_args_1[*i] && cmd_args_1[*i] <= LLMIN[*i])
			(*i)++;
		if (LLMIN[*i] || cmd_args_1[*i])
			return (1);
	}
	else if (ft_strlen(cmd_args_1) > 20)
		return (1);
	return (0);
}

int	ft_ex_val(t_big_token *b)
{
	int			i;

	i = 0;
	if (b->cmd_args[1][0] != '-' && ft_ex_val_1(&i, b->cmd_args[1]))
		return (1);
	else if (b->cmd_args[1][0] == '-' && ft_ex_val_2(&i, b->cmd_args[1]))
		return (1);
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
