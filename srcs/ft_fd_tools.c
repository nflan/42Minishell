#include "../include/minishell.h"

int	ft_fill_fdnew(t_fd *fd, t_token **tmp, int red, int *hd)
{
	if (red == 1 || red == 2)
		fd->red = red - 1;
	else
		fd->red = red - 3;
	if (red == 2)
	{
		*hd += 1;
		fd->delimitator = ft_create_del(tmp, &red);
		if (ft_create_tmp(fd, *hd))
			return (1);
		fd->fd = open(fd->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		ft_here(fd, red);
	}
	else
		fd->file = ft_create_del(tmp, &red);
	if (!fd->file)
		return (1);
	return (0);
}

int	ft_fdnew(t_big_token *b_tokens, t_fd **fd, t_token **tmp, int red)
{
	t_fd	*new;

	new = ft_calloc(sizeof(t_fd), 1);
	if (!new)
		return (ft_putstr_error("Malloc error in ft_fdnew "));
	new->info = b_tokens->info;
	if (ft_fill_fdnew(new, tmp, red, &(b_tokens)->nb_hd))
		return (ft_putstr_error("Malloc error in ft_fdnew "));
	ft_fdadd_back(fd, new);
	return (0);
}

int	ft_create_tmp(t_fd *fd, int hd)
{
	int	i;

	i = 0;
	if (hd)
	{
		fd->file = ft_strdup("/tmp/.tmp_hd_");
		if (!fd->file)
			return (1);
		while (--hd)
			i++;
		fd->file = ft_strjoin_free(fd->file, ft_itoa(i), 4);
		if (!fd->file)
			return (1);
	}
	return (0);
}

char	*ft_create_del(t_token **tmp, int *red)
{
	char	*del;

	del = NULL;
	if (tmp[0])
	{
		while (tmp[0] && tmp[0]->token != TOK_SEP)
		{
			if (tmp[0]->token != TOK_S_QUOTER && tmp[0]->token != TOK_D_QUOTER)
			{
				del = ft_strjoin_free(del, tmp[0]->value, 1);
				if (!del)
					return (NULL);
			}
			else if (*red != 8)
				*red += 3;
			tmp[0] = tmp[0]->next;
		}
	}
	if (*red == 8)
		*red = 5;
	else
		*red = 2;
	return (del);
}

void	ft_fdadd_back(t_fd **alst, t_fd *new)
{
	t_fd	*tmp;

	tmp = NULL;
	tmp = *alst;
	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}
