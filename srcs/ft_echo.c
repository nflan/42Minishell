/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 16:59:17 by nflan             #+#    #+#             */
/*   Updated: 2022/05/20 17:16:29 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_echo(char *line, int fd)
{
	char	*tmp;

	tmp = line;
	if (!line)
		return ;
	if ((line[0] == '-' || line[1] == 'n') && line[2] == ' ')
		tmp = ft_substr(line, 3, ft_strlen(line) - 3);
	else
		tmp = ft_strjoin(tmp, "\n");
	ft_putstr_fd(tmp, fd);
	free(tmp);
}
