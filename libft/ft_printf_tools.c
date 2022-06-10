/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 10:43:24 by nflan             #+#    #+#             */
/*   Updated: 2021/12/07 11:10:13 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_size_int(unsigned long long nb, unsigned int base_len)
{
	unsigned long long	number_len;

	number_len = 0;
	while (nb >= base_len)
	{
		nb = nb / base_len;
		number_len++;
	}
	number_len++;
	return (number_len);
}

int	ft_putunbr_count(unsigned int nb)
{
	static unsigned long long	n;
	int							i;

	n = nb;
	i = ft_size_int(n, 10);
	if (nb >= 10)
	{
		ft_putunbr_count(nb / 10);
		ft_putunbr_count(nb % 10);
	}
	else
		ft_putchar_fd(nb + '0', 1);
	return (i);
}

int	ft_putnbr_count(int n, int i)
{
	unsigned long long	nb;

	if (n < 0)
	{
		n = (n + 1) * -1;
		nb = n;
		nb++;
		i += ft_putchar_count('-');
	}
	else
		nb = n;
	if (i == 1 || i == 0)
		i += ft_size_int(nb, 10);
	if (nb >= 10)
	{
		ft_putnbr_count(nb / 10, i);
		ft_putnbr_count(nb % 10, i);
	}
	else
		ft_putchar_fd(nb + '0', 1);
	return (i);
}

int	ft_hexa_co(unsigned long long nb, char c, unsigned long long count)
{
	char	*base;

	if (count == 0)
		count = ft_size_int(nb, 16);
	if (c == 'x')
		base = "0123456789abcdef";
	if (c == 'p')
	{
		base = "0123456789abcdef";
		count += ft_putstr_count("0x");
		c = 'x';
	}
	if (nb >= 16)
	{
		ft_hexa_co(nb / 16, c, count);
		ft_hexa_co(nb % 16, c, count);
	}
	else
		ft_putchar_count(base[nb % 16]);
	return (count);
}

int	ft_hexa_count(unsigned int nb, char c, unsigned long long count)
{
	char	*base;

	if (count == 0)
		count = ft_size_int(nb, 16);
	if (c == 'x')
		base = "0123456789abcdef";
	if (c == 'X')
		base = "0123456789ABCDEF";
	if (nb >= 16)
	{
		ft_hexa_count(nb / 16, c, count);
		ft_hexa_count(nb % 16, c, count);
	}
	else
		ft_putchar_count(base[nb % 16]);
	return (count);
}
