/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 09:53:40 by nflan             #+#    #+#             */
/*   Updated: 2021/12/03 18:42:49 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_size_int(int nb)
{
	unsigned int	number;
	unsigned int	base_len;
	int				number_len;

	number_len = 0;
	base_len = 10;
	if (nb < 0)
		number = -nb;
	else
		number = nb;
	while (number >= base_len)
	{
		number = number / base_len;
		number_len++;
	}
	number_len++;
	return (number_len);
}

static char	ft_pos(int i, int nb)
{
	char			pos;
	unsigned int	nombre;
	unsigned int	base_len;

	base_len = 10;
	if (nb < 0)
		nombre = nb * -1;
	else
		nombre = nb;
	while (i > 1)
	{
		nombre = nombre / base_len;
		i--;
	}
	pos = nombre % base_len + '0';
	return (pos);
}

char	*ft_itoa(int nb)
{
	char	*str;
	int		int_len;
	int		i;
	int		len;

	i = 0;
	int_len = ft_size_int(nb);
	len = int_len;
	if (nb < 0)
		len++;
	str = ft_calloc(sizeof(*str), (len + 1));
	if (!str)
		return (NULL);
	if (nb < 0)
	{
		str[0] = '-';
		i++;
	}
	while (int_len)
	{
		str[i] = ft_pos(int_len, nb);
		int_len--;
		i++;
	}
	return (str);
}
