#include "../include/minishell.h"

int	ft_open_err(t_big_token *b_tok, t_fd *fd, int i)
{
	char	*err;

	err = NULL;
	b_tok->sc = 1;
	if (!i)
		fd->fd = 0;
	if (i == 1)
		fd->fd = 0;
	err = ft_strjoin("minishell: ", fd->file);
	if (!err)
		return (1);
	perror(err);
	free(err);
	return (1);
}

int	ft_open_all_fdout(t_big_token *b_tokens, t_fd *fd)
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
				return (ft_open_err(b_tokens, fd, 1));
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

int	ft_open_all_fdin(t_big_token *b_tokens, t_fd *tmp_fd)
{
	if (tmp_fd)
	{
		while (tmp_fd)
		{
			tmp_fd->fd = open(tmp_fd->file, O_RDONLY);
			if (tmp_fd->fd < 0)
				return (ft_open_err(b_tokens, tmp_fd, 0));
			else
				b_tokens->fdin = tmp_fd->fd;
			if (tmp_fd->next)
			{	
				close(tmp_fd->fd);
				tmp_fd->fd = 1;
				if (tmp_fd->red)
					unlink(tmp_fd->file);
			}
			tmp_fd = tmp_fd->next;
		}
	}
	return (0);
}

int	ft_open_fd(t_big_token *b_tokens)
{
	int	err;

	err = 0;
	if (b_tokens)
	{
		while (b_tokens)
		{
			if (b_tokens->fd_out)
				err += ft_open_all_fdout(b_tokens, b_tokens->fd_out);
			if (b_tokens->fd_in)
				err += ft_open_all_fdin(b_tokens, b_tokens->fd_in);
			b_tokens = b_tokens->sibling;
		}
	}
	return (err);
}
