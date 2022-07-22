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

static size_t	ft_word_count2(const char *s, char c, size_t size)
{
	unsigned int	i;
	unsigned int	line;

	i = 0;
	line = 0;
	while (s[i] && i < size)
	{
		while (s[i] == c && s[i] && i < size)
			i++;
		if (s[i] != c && s[i] && i < size)
		{
			while (s[i] != c && s[i])
				i++;
			line++;
		}
	}
	return (line);
}

static char	*ft_strdup_set2(const char *s, char c)
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

char	**ft_free_split2(char **tab)
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

char	**ft_split2(const char *s, char c, size_t size)
{
	unsigned int	line;
	char			**tab;

	line = 0;
	tab = NULL;
	if (s)
	{
		tab = ft_calloc(sizeof(char *), ft_word_count2(s, c, size) + 1);
		if (!tab)
			return (NULL);
		while (s[size])
		{
			while (s[size] && s[size] == c)
				size--;
			if (s[size] && s[size] != c)
			{
				tab[line++] = ft_strdup_set2(s, c);
				if (!tab[line - 1])
					return (ft_free_split2(tab));
			}
			while (s[size] && s[size] != c)
				size--;
		}
	}
	return (tab);
}
