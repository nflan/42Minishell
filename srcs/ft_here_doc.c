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

int	ft_write_here(t_fd *fd, char **str, int i, int red)
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
		{
			*str = ft_expand_l(*str, fd->info, 1);
			if (!*str)
				return (1);
		}
		write(fd->fd, *str, ft_strlen(*str));
		write(fd->fd, "\n", 1);
	}
	return (0);
}

int	ft_fill_here(t_fd *fd, int red)
{
	char	*buf;
	char	*to_free;

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
			if (ft_write_here(fd, &buf, 2, red))
				return (free(to_free), free(buf), 1);
		free(buf);
		buf = NULL;
	}
	if (buf)
		free(buf);
	return (0);
}

int	ft_here(t_fd *fd, int red, t_info *info)
{
	pid_t	pid;

	pid = -1;
	pid = fork();
	if ((int) pid == -1)
		return (ft_putstr_error("Child error\n"));
	signal(SIGINT, SIG_IGN);
	if ((int) pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		ft_fill_here(fd, red);
		if (info->tokens)
			while (info->tokens)
				info->tokens = info->tokens->prev;
		ft_exit_cmd(info, NULL, 1);
	}
	waitpid((int)pid, &pid, 0);
	signal(SIGINT, &ft_signal);
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
