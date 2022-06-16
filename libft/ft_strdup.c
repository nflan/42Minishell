/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 18:20:05 by nflan             #+#    #+#             */
/*   Updated: 2022/06/16 15:13:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	unsigned int	len;
	char			*str;

	len = ft_strlen((char *)src);
	str = ft_calloc(sizeof(char), (len + 1));
	if (!str)
		return (0);
	while (*src)
		*str++ = *src++;
	return (str - len);
}

char	*ft_strdup_free(char *src)
{
	unsigned int	len;
	char			*str;

	len = ft_strlen(src);
	str = ft_calloc(sizeof(char), len + 1);
	if (!str)
		return (0);
	while (*src)
		*str++ = *src++;
	free(src - len);
	return (str - len);
}
