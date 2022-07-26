/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_tools2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:05:09 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 22:18:13 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_create_without_env(t_env **new, t_env *ptr)
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		ptr = ft_without_env(i);
		if (!ptr && !i)
			return (1);
		if (!ptr)
			return (ft_free_env(*new), 1);
		ft_envadd_back(new, ptr);
	}
	return (0);
}

int	ft_init_env(t_info *info, char **envp)
{
	t_env	*ptr;
	t_env	*new;
	int		i;

	ptr = NULL;
	new = NULL;
	i = -1;
	if (envp[0])
	{
		while (envp[++i])
		{
			ptr = ft_envnew(envp[i]);
			if (!ptr && !i)
				return (1);
			else if (!ptr && new)
				return (ft_free_env(new), 1);
			ft_envadd_back(&new, ptr);
		}
	}
	else if (ft_create_without_env(&new, ptr))
		return (1);
	info->env = new;
	return (0);
}
