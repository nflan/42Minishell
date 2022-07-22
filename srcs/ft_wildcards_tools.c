/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcards_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 18:33:02 by nflan             #+#    #+#             */
/*   Updated: 2022/07/21 13:22:46 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_wdadd_back(t_wildcards **alst, t_wildcards *new)
{
	t_wildcards	*tmp;

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

int	ft_wildcardsnew(t_wildcards **wd, struct dirent *dir, DIR *fd)
{
	t_wildcards	*new;

	new = NULL;
	new = ft_calloc(sizeof(t_wildcards), 1);
	if (!new)
		return (ft_putstr_error("Malloc error in get_wildcards: "));
	new->fd = fd;
	new->dir = dir;
	ft_wdadd_back(wd, new);
	return (0);
}

int	ft_strlen_before_ast(char *str)
{
	int	i;

	i = 0;
	if (str)
		while (str[i] && str[i] != '*')
			i++;
	return (i);
}

int	ft_manage_type(char *str, char *dir, int d_type, int type)
{
	int	test;

	test = 0;
	if ((str[0] == '.' && dir[0] != '.') || (str[0] != '.' && dir[0] == '.'))
	{
		if (str[ft_strlen(str)] != '*')
			return (1);
		test++;
	}
	if (!test && str[ft_strlen(str) - 1] != '*')
		if (type == 4)
			if (d_type != type)
				return (1);
	return (0);
}

int	ft_get_wildcards(t_wildcards **wd)
{
	DIR				*dir;
	struct dirent	*send;
	char			*tofree;

	dir = NULL;
	send = NULL;
	tofree = NULL;
	tofree = getcwd(tofree, 0);
	if (!tofree)
		return (1);
	dir = opendir(tofree);
	if (!dir)
		return (free(tofree), perror("minishell"), 1);
	send = readdir(dir);
	if (!send)
		return (free(tofree), closedir(dir), perror("minishell"), 1);
	while (send)
	{
		if (send)
			if (ft_wildcardsnew(wd, send, dir))
				return (closedir(dir), free(tofree), 1);
		send = readdir(dir);
	}
	free(tofree);
	return (0);
}
