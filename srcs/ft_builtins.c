/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 21:42:30 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 16:37:06 by nflan            ###   ########.fr       */
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
	char	*tmp;

	if (b_tok->cmd_args[1] && b_tok->cmd_args[1][0] == '-')
		return (ft_putstr_error("minishell: pwd with option\n"), 2);
	tmp = NULL;
	tmp = getcwd(tmp, 0);
	if (!tmp && ft_get_env_value(b_tok->info, "PWD"))
	{
		tmp = ft_get_env_value(b_tok->info, "PWD");
		ft_putstr_fd(tmp, b_tok->fdout);
		ft_putstr_fd("\n", b_tok->fdout);
		return (0);
	}
	else if (!tmp)
		return (ft_putstr_error("minishell: PWD error\n"));
	ft_putstr_fd(tmp, b_tok->fdout);
	ft_putstr_fd("\n", b_tok->fdout);
	return (free(tmp), 0);
}

int	ft_unset_name(t_env **tmp, char *name)
{
	if (!tmp || !name)
		return (1);
	if (ft_wordigit(name))
		return (ft_exp_err(name, 2), 2);
	if (!ft_strncmp((*tmp)->name, name, ft_strlen((*tmp)->name) + 1))
		return (3);
	while ((*tmp)->next)
	{
		if (!ft_strncmp(name, (*tmp)->next->name, ft_strlen(name) + 1))
			return (0);
		else
			*tmp = (*tmp)->next;
	}
	return (1);
}

int	ft_unset(t_info *info, t_big_token *b_tokens, int err)
{
	t_env	*t;
	t_env	*ptr;
	int		i;
	int		check;

	i = 0;
	if (!info->env || !b_tokens->cmd_args[0])
		return (1);
	if (b_tokens->cmd_args[1] && b_tokens->cmd_args[1][0] == '-')
		return (ft_putstr_error("minishell: unset with option\n"), 2);
	while (b_tokens->cmd_args[++i])
	{
		ptr = NULL;
		t = info->env;
		check = ft_unset_name(&t, b_tokens->cmd_args[i]);
		if (!check)
			unset_tool(&ptr, &t, &info, 2);
		else if (check == 2)
			err = 1;
		else if (check == 3)
			unset_tool(&ptr, &t, &info, 1);
		if (ptr)
			unset_tool(&ptr, &t, &info, 3);
	}
	return (err);
}
