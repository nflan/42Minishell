/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 10:01:41 by nflan             #+#    #+#             */
/*   Updated: 2022/05/18 11:10:16 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_print(char *buf, char c, int i)
{
	int	p;

	p = i;
	if (buf)
	{
		if (ft_strchr(buf, c))
		{
			while (p--)
				write(1, "pipe ", 5);
			write(1, "heredoc > ", 10);
		}
	}
	else
	{
		while (p--)
			write(1, "pipe ", 5);
		write(1, "heredoc > ", 10);
	}
}

unsigned int	ft_test_line(char *str)
{
	int	i;

	i = -1;
	if (str)
		while (str[++i])
			if (str[i] == '\n')
				return (0);
	return (1);
}

char	*ft_fill_str(char *str, char *buf)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	j = -1;
	tmp = NULL;
	tmp = ft_calloc(sizeof(char), (ft_strlen(str) + ft_strlen(buf) + 1));
	if (!tmp)
		return (NULL);
	if (str)
		while (str[++i])
			tmp[i] = str[i];
	else
		i = 0;
	if (buf)
		while (buf[++j])
			tmp[i + j] = buf[j];
	free(str);
	return (tmp);
}

char	*get_line(char *str, int fd)
{
	char	*buf;
	int		ret;

	buf = NULL;
	ret = 1;
	while (ret != 0)
	{
		buf = ft_calloc(sizeof(char), 2);
		if (!buf)
			return (NULL);
		ret = read(fd, buf, 1);
		buf[ret] = 0;
		if ((ret == 0 || !*buf) && !str)
		{
			free(buf);
			return (NULL);
		}
		str = ft_fill_str(str, buf);
		free(buf);
		if (!str)
			return (NULL);
		if (!ft_test_line(str))
			ret = 0;
	}
	return (str);
}

char	*get_next_line(int fd)
{
	char	*line;

	if (fd < 0)
		return (NULL);
	line = NULL;
	line = get_line(line, fd);
	if (!line)
		return (NULL);
	return (line);
}
