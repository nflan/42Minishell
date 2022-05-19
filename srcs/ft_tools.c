/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 16:10:52 by nflan             #+#    #+#             */
/*   Updated: 2022/05/19 16:31:51 by nflan            ###   ########.fr       */
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

int	ft_perror(char *error, char *str)
{
	if (str)
		error = ft_strjoin(error, str);
	perror(error);
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
	return (1);
}
