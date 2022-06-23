/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 15:13:19 by nflan             #+#    #+#             */
/*   Updated: 2022/06/20 16:06:48 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_here(t_fd *fd, int ret)
{
	char	*buf;

	buf = NULL;
	while (ret > 0)
	{
		ft_print(fd, buf, '\n');
		buf = get_next_line(0);
		if (!buf)
			return (1);
		if (ft_strlen(buf) - 1 == ft_strlen(fd->delimitator) && !ft_strncmp(buf, fd->delimitator, ft_strlen(fd->delimitator)))
			ret = 0;
		else
			write(fd->fd, buf, ft_strlen(buf));
		free(buf);
		buf = NULL;
	}
	close(fd->fd);
	fd->fd = 0;
	return (0);
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
