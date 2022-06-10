/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 13:42:42 by nflan             #+#    #+#             */
/*   Updated: 2021/11/25 12:43:16 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*us1;
	const unsigned char	*us2;

	us1 = s1;
	us2 = s2;
	if (n == 0)
		return (0);
	while (*us1 == *us2 && --n)
	{
		us1++;
		us2++;
	}
	return (*us1 - *us2);
}
