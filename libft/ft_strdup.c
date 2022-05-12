/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 18:20:05 by nflan             #+#    #+#             */
/*   Updated: 2021/11/29 12:31:32 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	unsigned int	len;
	char			*str;

	len = ft_strlen((char *)src);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	while (*src)
		*str++ = *src++;
	*str = '\0';
	return (str - len);
}
