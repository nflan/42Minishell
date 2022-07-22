/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:13:07 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/21 11:48:47 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_open_err(t_big_token *b_tok, t_fd *fd, int i, t_info *info)
{
	char	*err;

	err = NULL;
	b_tok->sc = -1;
	info->nb_cmd++;
	if (!i)
		b_tok->fdin = -1;
	if (i == 1)
		b_tok->fdout = -1;
	err = ft_strjoin("minishell: ", fd->file);
	if (!err)
		return (1);
	perror(err);
	free(err);
	return (1);
}

int	ft_open_all_fdout(t_big_token *b_tokens, t_fd *fd, t_info *info)
{
	if (fd)
	{
		while (fd)
		{
			if (!fd->red)
				fd->fd = open(fd->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd->fd = open(fd->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd->fd < 0)
				return (ft_open_err(b_tokens, fd, 1, info));
			else
				b_tokens->fdout = fd->fd;
			if (fd->next)
			{
				close(fd->fd);
				fd->fd = 0;
			}
			fd = fd->next;
		}
	}
	return (0);
}

int	ft_open_all_fdin(t_big_token *b_tokens, t_fd *tmp_fd, t_info *info)
{
	if (tmp_fd)
	{
		while (tmp_fd)
		{
			tmp_fd->fd = open(tmp_fd->file, O_RDONLY);
			if (tmp_fd->fd < 0)
				return (ft_open_err(b_tokens, tmp_fd, 0, info));
			else
				b_tokens->fdin = tmp_fd->fd;
			if (tmp_fd->next)
			{
				close(tmp_fd->fd);
				tmp_fd->fd = 1; //pk le 1 ici?
				if (tmp_fd->red)
					unlink(tmp_fd->file);
			}
			tmp_fd = tmp_fd->next;
		}
	}
	return (0);
}

int	ft_open_fd(t_big_token *b_tokens, t_info *info)
{
	int	err;

	err = 0;
	if (b_tokens)
	{
		if (b_tokens->fd_in)
			err += ft_open_all_fdin(b_tokens, b_tokens->fd_in, info);
		if (err)
			return (err);
		if (b_tokens->fd_out)
			err += ft_open_all_fdout(b_tokens, b_tokens->fd_out, info);
	}
	return (err);
}
