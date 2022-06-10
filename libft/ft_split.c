/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:56:17 by nflan             #+#    #+#             */
/*   Updated: 2022/05/20 12:31:57 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_word_count(const char *s, char c)
{
	unsigned int	i;
	unsigned int	line;

	i = 0;
	line = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i])
		{
			while (s[i] != c && s[i])
				i++;
			line++;
		}
	}
	return (line);
}

static char	*ft_strdup_set(const char *s, char c)
{
	unsigned int	i;
	unsigned int	temp;
	unsigned int	index;
	char			*new;

	index = 0;
	i = 0;
	while (s[i] && s[i] == c)
		i++;
	temp = i;
	while (s[i] && s[i] != c)
		i++;
	new = malloc(sizeof(char) * (i - temp) + 1);
	if (!new)
		return (NULL);
	while (0 < i - temp)
		new[index++] = s[temp++];
	new[index] = '\0';
	return (new);
}

char	**ft_free_split(char **tab)
{
	unsigned int	i;

	i = 0;
	if (!tab)
		return (NULL);
	while (tab[i])
		i++;
	while (i-- > 0)
		if (tab[i])
			free(tab[i]);
	free(tab);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	unsigned int	line;
	char			**tab;

	line = 0;
	tab = NULL;
	if (s)
	{
		tab = ft_calloc(sizeof(char *), (ft_word_count(s, c) + 1));
		if (!tab)
			return (NULL);
		while (*s)
		{
			while (*s && *s == c)
				s++;
			if (*s && *s != c)
			{
				tab[line++] = ft_strdup_set(s, c);
				if (!tab[line - 1])
					return (ft_free_split(tab));
			}
			while (*s && *s != c)
				s++;
		}
	}
	return (tab);
}
