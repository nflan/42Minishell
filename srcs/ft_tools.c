/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 16:10:52 by nflan             #+#    #+#             */
/*   Updated: 2022/05/23 17:06:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_putstr_error(char *error)
{
	if (error)
		while (*error)
			write(2, (void *)error++, 1);
	return (1);
}

int	ft_putstr_frror(char *error, char *str, int i)
{
	char	*tofree;
	char	*tofree2;

	tofree = error;
	tofree2 = str;
	if (str && error)
		error = ft_strjoin(error, str);
	ft_putstr_error(error);
	if (i && i != 2)
		free(tofree);
	if (i && i != 1)
		free(tofree2);
	free(error);
	return (1);
}

int	ft_perror(char *error, char *str)
{
	if (str)
		error = ft_strjoin(error, str);
	perror(error);
	if (str)
		free(error);
	return (1);
}

int	ft_perror_free(char *error, char *str, int i)
{
	char	*tofree;
	char	*tofree2;

	tofree = error;
	tofree2 = str;
	if (str && error)
		error = ft_strjoin(error, str);
	perror(error);
	if (i != 2)
		free(tofree);
	if (i != 1)
		free(tofree2);
	free(error);
	return (1);
}

char	*ft_get_env_value(t_info *info, char *name)
{
	t_env	*env;

	env = NULL;
	if (!info->env || !name)
		return (NULL);
	env = info->env;
	while (env && ft_strncmp(env->name, name, ft_strlen(name)))
		env = env->next;
	if (!env)
		return (NULL);
	return (env->value);
}
