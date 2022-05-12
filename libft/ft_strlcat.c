/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:29:39 by nflan             #+#    #+#             */
/*   Updated: 2021/11/25 15:08:44 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	src_len;

	i = ft_strlen(dest);
	j = 0;
	src_len = ft_strlen((char *)src);
	if (size == 0)
		return (src_len);
	while (src[j] && ((i + j) < (size - 1)))
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	if (i > size)
		return (src_len + size);
	else
		return (i + src_len);
}
