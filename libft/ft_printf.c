/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 14:40:58 by nflan             #+#    #+#             */
/*   Updated: 2021/12/07 11:11:00 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar_count(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr_count(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		i = ft_putstr_count("(null)");
		return (i);
	}
	while (s[i])
		write(1, &s[i++], 1);
	return (i);
}

int	ft_choose_function(char c, va_list *argv)
{
	int	i;

	i = 0;
	if (c == 'c')
		i += ft_putchar_count(va_arg(*argv, int));
	if (c == 's')
		i += ft_putstr_count(va_arg(*argv, char *));
	if (c == 'd' || c == 'i')
		i += ft_putnbr_count(va_arg(*argv, int), 0);
	if (c == 'u')
		i += ft_putunbr_count(va_arg(*argv, unsigned int));
	if (c == 'x' || c == 'X')
		i += ft_hexa_count(va_arg(*argv, unsigned int), c, 0);
	if (c == 'p')
		i += ft_hexa_co(va_arg(*argv, unsigned long long), c, 0);
	if (c == '%')
		i += ft_putchar_count(c);
	return (i);
}

int	ft_printf(const char *str, ...)
{
	va_list	argv;
	int		i;

	i = 0;
	va_start(argv, str);
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			i += ft_choose_function(*str, &argv);
		}
		else
			i += ft_putchar_count(*str);
		str++;
	}
	va_end(argv);
	return (i);
}
