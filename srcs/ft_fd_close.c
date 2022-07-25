/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_close.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:12:44 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 13:10:12 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_close_all_fd(t_fd *fd)
{
	if (fd->next)
		while (fd->next)
			fd = fd->next;
	if (fd)
	{
		if (fd->fd > 2)
			close(fd->fd);
		if (fd->inout == 1 && fd->red)
			unlink(fd->file);
	}
}

void	ft_close_fd(t_big_token *b_tokens)
{
	if (b_tokens)
		if (b_tokens->fd)
			ft_close_all_fd(b_tokens->fd);
}
