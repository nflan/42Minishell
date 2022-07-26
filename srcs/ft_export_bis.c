/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 15:52:52 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 12:19:03 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_if_eg(t_info *info, t_env *tmp, char *line, int i)
{
	if (line[i - 1] == '+')
	{
		while (tmp && ft_strncmp(tmp->name, line, i) != -43)
				tmp = tmp->next;
		if (tmp)
		{
			if (ft_export_concat(tmp, line, i))
				exit (ft_mal_err(info, info->env, "Malloc error\n"));
			return (0);
		}
	}
	else
		while (tmp && ft_strncmp(tmp->name, line, i + 1) != -61)
			tmp = tmp->next;
	if (!tmp)
	{
		if (ft_export_new(info->env, tmp, line))
			exit (ft_mal_err(info, info->env, "Malloc error\n"));
	}
	else
		if (ft_export_replace(tmp, line, i))
			exit (ft_mal_err(info, info->env, "Malloc error\n"));
	return (0);
}

int	ft_ifnot_eg(t_info *info, t_env *tmp, char *line, int i)
{
	while (tmp && ft_strncmp(tmp->name, line, i + 1))
		tmp = tmp->next;
	if (!tmp)
		if (ft_export_new(info->env, tmp, line))
			exit (ft_mal_err(info, info->env, "Malloc error\n"));
	return (0);
}

int	ft_exporting(t_info *info, t_big_token *b_tok, t_env *tmp, int (*ji)[2])
{
	while (b_tok->cmd_args[*ji[0]][*ji[1]]
		&& b_tok->cmd_args[*ji[0]][*ji[1]] != '=')
		(*ji[1])++;
	if (b_tok->cmd_args[*ji[0]][*ji[1]] == '=')
		ft_if_eg(info, tmp, b_tok->cmd_args[*ji[0]], *ji[1]);
	else
		ft_ifnot_eg(info, tmp, b_tok->cmd_args[*ji[0]], *ji[1]);
	return (0);
}

int	ft_do_export(t_info *info, t_big_token *b_tok, t_env *tmp, int i)
{
	int	ji[2];
	int	a;
	int	err;

	ji[0] = 0;
	ji[1] = 1;
	err = 0;
	if (b_tok->cmd_args[1] && b_tok->cmd_args[1][0] == '-')
		return (ft_putstr_error("minishell: export with option\n"), 2);
	while (b_tok->cmd_args[++(ji[0])])
	{
		a = ft_check_export(b_tok->cmd_args[ji[0]]);
		i = 0;
		if (!a)
			ft_exporting(info, b_tok, tmp, &ji);
		else if (a == 1)
			err = 1;
	}
	return (err);
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
		return (ft_do_export(info, b_tokens, tmp, i));
	return (0);
}
