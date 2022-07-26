/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 12:46:54 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 12:11:38 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (47 < c && c < 58)
		return (1);
	return (0);
}

int	ft_digital(char *c)
{
	if (!c)
		return (1);
	if (c)
	{
		if (*c == '-' || *c == '+')
			c++;
		if (!*c)
			return (1);
		while (*c)
		{
			if (!ft_isdigit(*c))
				return (1);
			c++;
		}
	}
	return (0);
}
