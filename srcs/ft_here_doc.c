/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 15:13:19 by nflan             #+#    #+#             */
/*   Updated: 2022/06/27 11:24:43 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_write_here(t_fd *fd, char **str, int i, int red)
{
	if (i == 1)
	{	
		ft_putstr_fd("That's not a good way to do things but EOF (wanted ", 2);
		ft_putstr_fd(*str, 2);
		ft_putstr_fd(")\n", 2);
	}
	else if (i == 2)
	{
		if (red == 2)
			expand(str, fd->info);
		write(fd->fd, *str, ft_strlen(*str));
		write(fd->fd, "\n", 1);
	}
}

int	ft_here(t_fd *fd, int red)
{
	char	*buf;
	char	*to_free;

	buf = NULL;
	to_free = ft_strjoiiin("heredoc \"", fd->delimitator, "\" > ");
	if (!to_free)
		return (1);
	while (1)
	{
		buf = readline(to_free);
		if (!buf)
			ft_write_here(fd, &fd->delimitator, 1, red);
		if (!buf || !ft_strncmp(buf, fd->delimitator,
				ft_strlen(fd->delimitator) + 1))
			break ;
		else
			ft_write_here(fd, &buf, 2, red);
		free(buf);
		buf = NULL;
	}
	if (buf)
		free(buf);
	close(fd->fd);
	fd->fd = 0;
	return (free(to_free), 0);
}

char	**ft_env_to_tab(t_env *env)
{
	t_env	*tmp;
	char	**tab;
	int		i;

	tab = NULL;
	tmp = env;
	i = 0;
	if (!env)
		return (NULL);
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	tab = ft_calloc(sizeof(char *), i + 1);
	i = 0;
	tmp = env;
	while (tmp)
	{
		tab[i] = ft_strjoiiin(tmp->name, "=", tmp->value);
		i++;
		tmp = tmp->next;
	}
	return (tab);
}
