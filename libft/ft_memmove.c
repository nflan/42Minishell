/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:55:29 by nflan             #+#    #+#             */
/*   Updated: 2021/11/30 16:47:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*tmp1;
	unsigned char	*tmp2;

	tmp1 = (unsigned char *) dst;
	tmp2 = (unsigned char *) src;
	if (!dst && !src && len != 0)
		return (dst);
	if (tmp2 < tmp1)
		while (len--)
			tmp1[len] = tmp2[len];
	else
		while (len--)
			*tmp1++ = *tmp2++;
	return (dst);
}
