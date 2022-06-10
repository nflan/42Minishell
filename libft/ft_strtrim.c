/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:32:54 by nflan             #+#    #+#             */
/*   Updated: 2022/05/13 16:17:00 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_trim_start(char const *s1, char const *set, unsigned int i)
{
	int	j;
	int	len;

	len = 0;
	j = -1;
	while (set[++j])
	{
		if (s1[i] == set[j])
		{
			j = -1;
			i++;
			len++;
		}
	}
	return (len);
}

static size_t	ft_trim_end(char const *s1, char const *set, unsigned int i)
{
	int	j;
	int	len;

	len = 0;
	j = -1;
	while (set[++j] && i > 1)
	{
		if (s1[i] == set[j])
		{
			j = -1;
			i--;
			len++;
		}
	}
	return (len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	len;
	unsigned int	s1_len;
	char			*new;
	unsigned int	trim_start;

	s1_len = 0;
	len = 0;
	new = NULL;
	trim_start = 0;
	if (s1 && !*s1)
	{
		new = malloc(1);
		if (!new)
			return (NULL);
		new[0] = '\0';
	}
	if (s1 && *s1)
	{
		trim_start = ft_trim_start(s1, set, 0);
		s1_len = ft_strlen(s1);
		len = s1_len - trim_start - ft_trim_end(s1, set, s1_len - 1);
		new = ft_substr(s1, ft_trim_start(s1, set, 0), len);
	}
	return (new);
}
