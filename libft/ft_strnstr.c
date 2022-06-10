/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 11:16:00 by nflan             #+#    #+#             */
/*   Updated: 2022/05/19 10:56:30 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	tmp;

	i = 0;
	tmp = len;
	if (!(little[0]))
		return ((char *)big);
	while (len && big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && len)
		{
			j++;
			if ((!(little[j]) && len) || (tmp == 1 && tmp == ft_strlen(little)))
				return ((char *)(big + i));
			len--;
		}
		i++;
		len = len - 1 + j;
	}
	return (NULL);
}
