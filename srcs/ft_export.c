/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 12:32:37 by nflan             #+#    #+#             */
/*   Updated: 2022/07/20 17:11:36 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exp_err(char *line, int i)
{
	if (i == 1)
	ft_putstr_fd_3("minishell: export: `", line,
			"': not a valid identifier\n", 2);
	if (i == 2)
	ft_putstr_fd_3("minishell: unset: `", line,
			"': not a valid identifier\n", 2);
	return (1);
}

int	ft_not_valid_id(char *line)
{
	int	i;

	i = 0;
	if (line)
	{
		if (line[i] == '\0')
			return (ft_exp_err(line, 1));
		while (line[i] && line[i] != '=')
		{
			if (!ft_isdigit(line[i]) && !ft_isalpha(line[i]))
				return (ft_exp_err(line, 1));
			i++;
		}
	}
	return (0);
}

int	ft_check_export(char *line)
{
	if (!line)
		return (1);
	if (ft_isdigit(line[0]))
		return (ft_exp_err(line, 1));
	if (ft_not_valid_id(line))
		return (1);
	return (0);
}

int	ft_export_new(t_env *env, t_env *tmp, char *line)
{
	tmp = ft_envnew(line);
	if (!tmp)
		return (1);
	ft_envadd_back(&env, tmp);
	return (0);
}

int	ft_export_replace(t_env *env, char *line, int i)
{
	int	j;

	j = i + 1;
	if (line[j])
		while (line[j])
			j++;
	free(env->value);
	env->value = NULL;
	if (j > i + 1)
		env->value = ft_substr(line, i + 1, j);
	if (!env->value)
		env->value = ft_strdup("");
	if (!env->value)
		return (1);
	return (0);
}

int	ft_if_eg(t_info *info, t_env *tmp, char *line, int i)
{
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

	j = 1;
	while (b_tok->cmd_args[j])
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
		j++;
	}
	return (0);
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
