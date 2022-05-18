/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:37:41 by nflan             #+#    #+#             */
/*   Updated: 2022/05/18 14:08:16 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_cd(t_info *info, char *dir)
{
	char	*buf;
	t_env	*env;

	buf = NULL;
	env = info->env;
	if (!dir || !ft_strncmp(dir, "", 1))
	{
		while (strncmp(env->name, "HOME", 5))
			env = env->next;
		buf = ft_strdup(env->name);
		if (!buf)
		{
			ft_putstr_fd("Error: directory not found\n", 2);
			return (1);
		}
		buf = ft_strtrim(buf, "HOME=");
		if (chdir(buf))
		{
			free(buf);
			ft_putstr_fd("Error: directory not found\n", 2);
			return (1);
		}
		free(buf);
		return (0);
	}
	buf = ft_strjoin(getcwd(buf, 0), "/");
	buf = ft_strjoin(buf, dir);
	if (chdir(buf))
	{
		free(buf);
		ft_putstr_fd("Error: directory not found\n", 2);
		return (1);
	}
	printf("buf = %s\n", buf);
	free(buf);
	return (0);
}
