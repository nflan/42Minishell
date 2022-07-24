/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 21:42:30 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 23:44:03 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	unset_tool(t_env **ptr, t_env **tmp, t_info **info, int type)
{
	if (type == 1)
	{
		*ptr = *tmp;
		*tmp = (*tmp)->next;
		(*info)->env = *tmp;
	}
	else if (type == 2)
	{
		*ptr = (*tmp)->next;
		(*tmp)->next = (*tmp)->next->next;
	}
	else if (type == 3)
	{
		(*ptr)->next = NULL;
		ft_free_env(*ptr);
	}
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
	t_env	*t;
	t_env	*ptr;
	int		i;
	int		check;
	int		err;

	i = 0;
	err = 0;
	if (!info->env || !b_tokens->cmd_args[0] || !b_tokens->cmd_args[1])
		return (1);
	while (b_tokens->cmd_args[++i])
	{
		ptr = NULL;
		t = info->env;
		check = ft_unset_name(&t, b_tokens->cmd_args[i]);
		if (!ft_strncmp(t->name, b_tokens->cmd_args[i], ft_strlen(t->name) + 1))
			unset_tool(&ptr, &t, &info, 1);
		else if (!check)
			unset_tool(&ptr, &t, &info, 2);
		else if (check == 2)
			err = 1;
		if (ptr)
			unset_tool(&ptr, &t, &info, 3);
	}
	return (err);
}
