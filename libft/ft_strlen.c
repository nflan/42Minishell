/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:39:17 by nflan             #+#    #+#             */
/*   Updated: 2022/07/22 23:03:33 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str)
		while (str[i])
			i++;
	return (i);
}

size_t	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (0);
	if (tab[i])
		while (tab[i])
			i++;
	return (i);
}
