/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 11:34:55 by nflan             #+#    #+#             */
/*   Updated: 2022/05/18 18:40:17 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_pwd(void)
{
	char	*buf;
	char	*tofree;

	buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		return (1);
	tofree = buf;
	buf = ft_strjoin(buf, "\n");
	ft_putstr_fd(buf, 1);
	free(buf);
	free(tofree);
	return (0);
}
