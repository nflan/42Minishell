/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 21:42:30 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 22:34:21 by omoudni          ###   ########.fr       */
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
