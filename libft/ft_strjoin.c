/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:14:31 by nflan             #+#    #+#             */
/*   Updated: 2022/05/19 16:10:01 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	len;
	char			*new;

	len = 0;
	new = NULL;
	if (s1 && s2)
	{
		len = ft_strlen(s1) + ft_strlen(s2);
		new = ft_calloc(sizeof(char), len + 1);
		if (!new)
			return (NULL);
		while (*s1)
			*new++ = *s1++;
		while (*s2)
			*new++ = *s2++;
	}
	return (new - len);
}

char	*ft_strjoin_free(char *s1, char *s2, int i)
{
	unsigned int	len;
	unsigned int	j;
	unsigned int	k;
	char			*new;

	len = 0;
	new = NULL;
	j = 0;
	k = 0;
	if (s1 && s2)
	{
		len = ft_strlen(s1) + ft_strlen(s2);
		new = ft_calloc(sizeof(char), len + 1);
		if (!new)
			return (NULL);
		while (s1[j])
			*new++ = s1[j++];
		while (s2[k])
			*new++ = s2[k++];
	}
	if (s1 && i != 2)
		free(s1);
	if (s2 && i != 1)
		free(s2);
	return (new - len);
}
