/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 15:52:52 by omoudni           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/07/24 20:12:40 by nflan            ###   ########.fr       */
=======
/*   Updated: 2022/07/24 20:18:06 by omoudni          ###   ########.fr       */
>>>>>>> o
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_export_concat(t_env *env, char *line, int i)
{
	env->value = ft_strjoin_free(env->value, line + i + 1, 1);
	if (!env->value)
		return (1);
	return (0);
}

int	ft_if_eg(t_info *info, t_env *tmp, char *line, int i)
{
	if (line[i - 1] == '+')
	{
		while (tmp && ft_strncmp(tmp->name, line, i) != -43)
				tmp = tmp->next;
		if (tmp)
		{
			if (ft_export_concat(tmp, line, i))
				return (1);
			return (0);
		}
	}
	else
		while (tmp && ft_strncmp(tmp->name, line, i + 1) != -61)
			tmp = tmp->next;
	if (!tmp)
	{
		if (ft_export_new(info->env, tmp, line))
			return (1);
	}
	else
		if (ft_export_replace(tmp, line, i))
			return (1);
	return (0);
}

int	ft_ifnot_eg(t_info *info, t_env *tmp, char *line, int i)
{
	while (tmp && ft_strncmp(tmp->name, line, i + 1))
		tmp = tmp->next;
	if (!tmp)
		if (ft_export_new(info->env, tmp, line))
			return (1);
	return (0);
}

int	ft_do_export(t_info *info, t_big_token *b_tok, t_env *tmp, int i)
{
	int	j;
	int	a;

	a = 0;
	j = 0;
	while (b_tok->cmd_args[++j])
	{
		i = 0;
		if (!ft_check_export(b_tok->cmd_args[j]))
		{
			while (b_tok->cmd_args[j][i] && b_tok->cmd_args[j][i] != '=')
				i++;
			if (b_tok->cmd_args[j][i] == '=')
			{
				if (ft_if_eg(info, tmp, b_tok->cmd_args[j], i))
					return (1);
			}
			else
				if (ft_ifnot_eg(info, tmp, b_tok->cmd_args[j], i))
					return (1);
		}
		else
			a = 1;
	}
	return (a);
}

int	ft_export(t_info *info, t_big_token *b_tokens)
{
	t_env	*tmp;
	int		i;

	tmp = info->env;
	i = 0;
	if (!tmp)
		return (1);
	if (!b_tokens->cmd_args[1])
		ft_export_solo(tmp, b_tokens);
	else
		if (ft_do_export(info, b_tokens, tmp, i))
			return (1);
	return (0);
}
