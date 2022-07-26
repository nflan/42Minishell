/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 12:32:37 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 16:35:56 by nflan            ###   ########.fr       */
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
		if (line[i] == '\0' || line[i] == '=')
			return (ft_exp_err(line, 1));
		while (line[i] && line[i] != '=')
		{
			if (!ft_isdigit(line[i]) && !ft_isalpha(line[i]) && line[i] != '_')
			{
				if (line[i] == '+')
					if (line[i + 1] == '=')
						break ;
				return (ft_exp_err(line, 1));
			}
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
	return (ft_not_valid_id(line));
}

int	ft_export_new(t_env *env, t_env *tmp, char *line)
{
	t_env	*new;

	new = env;
	tmp = ft_envnew(line);
	if (!tmp)
		return (1);
	ft_envadd_back(&new, tmp);
	return (0);
}

int	ft_export_replace(t_env *env, char *line, int i)
{
	int	j;

	j = i + 1;
	if (line)
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
