/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_close.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:12:44 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 13:12:51 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_close_all_fd(t_fd *fd, int fd_type)
{
	if (fd->next)
		while (fd->next)
			fd = fd->next;
	if (fd)
	{
		if (fd->fd > 2)
			close(fd->fd);
		if (!fd_type && fd->red)
			unlink(fd->file);
	}
}

void	ft_close_fd(t_big_token *b_tokens)
{
	if (b_tokens)
	{
		if (b_tokens->fd_out)
			ft_close_all_fd(b_tokens->fd_out, 1);
		if (b_tokens->fd_in)
			ft_close_all_fd(b_tokens->fd_in, 0);
	}
}
