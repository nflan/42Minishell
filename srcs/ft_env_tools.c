/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:04:01 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 22:16:50 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_envadd_back(t_env **alst, t_env *new)
{
	t_env	*tmp;

	tmp = NULL;
	tmp = *alst;
	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}

t_env	*ft_envnew(char *line)
{
	t_env	*new;
	char	*tmp;

	tmp = NULL;
	new = ft_calloc(sizeof(t_env), 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	if (ft_fill_envnew(new, line, 0, -1))
		return (free(new), NULL);
	if (!ft_strncmp(new->name, "SHLVL", 6) || !ft_strncmp(new->name, "PWD", 4))
	{
		if (!ft_strncmp(new->name, "SHLVL", 6))
			tmp = ft_itoa(ft_atoi(new->value) + 1);
		else if (!ft_strncmp(new->name, "PWD", 4))
			tmp = getcwd(tmp, 0);
		free(new->value);
		new->value = ft_strdup_free(tmp);
		if (!new->value)
			return (ft_free_env(new), NULL);
	}
	return (new);
}

t_env	*ft_without_env(int i)
{
	t_env	*new;

	new = ft_calloc(sizeof(t_env), 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	if (i == 0)
	{
		new->name = ft_strdup("PWD");
		new->value = getcwd(new->value, 0);
	}
	else if (i == 1)
	{
		new->name = ft_strdup("SHLVL");
		new->value = ft_strdup("1");
	}
	else if (i == 2)
	{
		new->name = ft_strdup("_");
		new->value = ft_strdup("./minishell");
	}
	if (!new->name || !new->value)
		return (NULL);
	return (new);
}

int	ft_fill_envnew(t_env *env, char *line, int i, int j)
{
	if (!line)
		return (1);
	while (line[i] && line[i] != '=' && line[i] != '+')
		i++;
	env->name = ft_calloc(sizeof(char), i + 1);
	if (!env->name)
		return (1);
	while (++j < i)
		env->name[j] = line[j];
	if (line[j] != '=' && line[j] != '+')
		return (0);
	if (line[j] == '+')
		j++;
	j++;
	i = 0;
	while (line[j + i])
		i++;
	env->value = ft_calloc(sizeof(char), i + 1);
	if (!env->value)
		return (free(env->name), 1);
	i = 0;
	while (line[j])
		env->value[i++] = line[j++];
	return (0);
}
