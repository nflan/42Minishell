/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 10:33:32 by nflan             #+#    #+#             */
/*   Updated: 2022/06/27 19:48:37 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

int	ft_putstr_fd_3(char *s, char *s1, char *s2, int fd)
{
	if (s)
		while (*s)
			write(fd, (void *)s++, 1);
	if (s1)
		while (*s1)
			write(fd, (void *)s1++, 1);
	if (s2)
		while (*s2)
			write(fd, (void *)s2++, 1);
	return (1);
}

int	ft_putstr_fd(char *s, int fd)
{
	if (s)
		while (*s)
			write(fd, (void *)s++, 1);
	return (1);
}
