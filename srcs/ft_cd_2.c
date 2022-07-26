/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:37:55 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 12:30:34 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_is_tilde_or_home(char *home, char *dir)
{
	if (dir && !ft_strncmp(dir, "~", 2))
		return (0);
	if (!dir)
		return (1);
	if (dir[0] == '~' && (dir[1] == '/' || dir[1] == '+' || dir[1] == '-'))
		return (0);
	if (!home && !dir)
		return (1);
	return (2);
}

char	*ft_cd_tilde(t_info *info, char *home, char *dir)
{
	char	*new_dir;

	new_dir = NULL;
	if (!dir)
		return (NULL);
	if (dir[1] == '/')
		new_dir = ft_strjoin(home, dir + 1);
	else if (dir[1] == '+' || dir[1] == '-')
	{
		if (dir[1] == '+')
		{
			new_dir = ft_strdup(ft_get_env_value(info, "PWD"));
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		}
		else
		{
			new_dir = ft_strdup(ft_get_env_value(info, "OLDPWD"));
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		}
	}
	return (new_dir);
}

int	ft_back(t_env *env, char *op, char *home)
{
	if (env)
		while (env && ft_strncmp(env->name, "OLDPWD", 7))
			env = env->next;
	if (env)
	{
		free(env->value);
		env->value = ft_strdup(op);
		if (!env->value)
			return (free(op), 2);
	}
	return (ft_perror("minishell: cd: ", home));
}
