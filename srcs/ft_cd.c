/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:37:41 by nflan             #+#    #+#             */
/*   Updated: 2022/05/18 16:18:07 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_cd(t_info *info, char *dir)
{
	t_env	*env;

	env = info->env;
	if (!dir || !ft_strncmp(dir, "", 1))
	{
		while (env && strncmp(env->name, "HOME", 5))
			env = env->next;
		if (!env)
			return (ft_putstr_error("minishell: cd: HOME not set\n"));
		else if (chdir(env->value))
			return (ft_perror("minishell: cd: ", dir));
	}
	else
		if (chdir(dir))
			return (ft_perror("minishell: cd: ", dir));
	return (0);
}
