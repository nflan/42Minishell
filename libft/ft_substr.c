/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 14:09:13 by nflan             #+#    #+#             */
/*   Updated: 2022/06/15 15:30:55 by nflan            ###   ########.fr       */
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
		new = ft_calloc(sizeof(char), len + 1);
		if (!new)
			return (NULL);
		if (start < ft_strlen(s))
			while (i < len && s[start])
				new[i++] = s[start++];
	}
	return (new);
}

char	*ft_substr_free(char *s, unsigned int start, size_t len)
{
	char			*new;
	unsigned int	i;

	new = NULL;
	if (s)
	{
		i = 0;
		if (len > ft_strlen(s))
			len = ft_strlen(s);
		new = ft_calloc(sizeof(char), len + 1);
		if (!new)
			return (NULL);
		if (start < ft_strlen(s))
			while (i < len && s[start])
				new[i++] = s[start++];
		free(s);
	}
	return (new);
}
