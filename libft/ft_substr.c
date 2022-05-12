/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 14:09:13 by nflan             #+#    #+#             */
/*   Updated: 2021/12/07 08:25:11 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*new;
	unsigned int	i;

	new = NULL;
	if (s)
	{
		i = 0;
		if (len > ft_strlen(s))
			len = ft_strlen(s);
		new = malloc(sizeof(char) * len + 1);
		if (!new)
			return (NULL);
		if (start < ft_strlen(s))
		{
			while (i < len && s[start])
				new[i++] = s[start++];
		}
		new[i] = '\0';
	}
	return (new);
}
