/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:14:31 by nflan             #+#    #+#             */
/*   Updated: 2022/07/21 15:49:19 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_3_str(char *s1, char *s2, char *s3, int i)
{
	if (s1 && i != 2 && i != 3)
	{
		free(s1);
		s1 = NULL;
	}
	if (s2 && i != 1 && i != 3)
	{
		free(s2);
		s2 = NULL;
	}
	if (s3 && i != 1 && i != 2)
	{
		free(s3);
		s3 = NULL;
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	len;
	char			*new;

	len = 0;
	new = NULL;
	len = ft_strlen(s1) + ft_strlen(s2);
	new = ft_calloc(sizeof(char), len + 1);
	if (!new)
		return (NULL);
	if (s1)
		while (s1 && *s1)
			*new++ = *s1++;
	if (s2)
		while (s2 && *s2)
			*new++ = *s2++;
	return (new - len);
}

char	*ft_strjoin_free(char *s1, char *s2, int i)
{
	unsigned int	len;
	unsigned int	j;
	char			*new;

	len = 0;
	new = NULL;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2);
	new = ft_calloc(sizeof(char), len + 1);
	if (!new)
		return (ft_putstr_error("Malloc error in ft_strjoin_free in "),
			ft_free_3_str(s1, s2, NULL, i), NULL);
	if (s1)
		while (s1[j])
			*new++ = s1[j++];
	j = 0;
	if (s2)
		while (s2[j])
			*new++ = s2[j++];
	ft_free_3_str(s1, s2, NULL, i);
	return (new - len);
}

char	*ft_strjoiiin(char const *s1, char const *s2, char const *s3)
{
	unsigned int	len;
	char			*new;

	len = 0;
	new = NULL;
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	new = ft_calloc(sizeof(char), len + 1);
	if (!new)
		return (NULL);
	if (s1)
		while (*s1)
			*new++ = *s1++;
	if (s2)
		while (*s2)
			*new++ = *s2++;
	if (s3)
		while (*s3)
			*new++ = *s3++;
	return (new - len);
}

char	*ft_strjoiiin_free(char *s1, char *s2, char *s3, int i)
{
	unsigned int	len;
	unsigned int	j;
	char			*new;

	len = 0;
	new = NULL;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	new = ft_calloc(sizeof(char), len + 1);
	if (!new)
		return (ft_free_3_str(s1, s2, s3, i), NULL);
	if (s1)
		while (s1[j])
			*new++ = s1[j++];
	j = 0;
	if (s2)
		while (s2[j])
			*new++ = s2[j++];
	j = 0;
	if (s3)
		while (s3[j])
			*new++ = s3[j++];
	ft_free_3_str(s1, s2, s3, i);
	return (new - len);
}
