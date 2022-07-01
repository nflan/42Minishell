/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 02:51:58 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 02:52:00 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// itscl[0] = i, itscl[1] = type_red, itscl[2] = save_word, 
// itscl[3] = cmd_args_nums, itscl[4] = b_length
int	ft_fill_fdnew(t_fd *fd, t_token **tmp, int itscl[5], int *hd)
{
	if (itscl[1] == 1 || itscl[1] == 2)
		fd->red = itscl[1] - 1;
	else
		fd->red = itscl[1] - 3;
	if (itscl[1] == 2)
	{
		*hd += 1;
		fd->delimitator = ft_create_del(tmp, itscl);
		if (ft_create_tmp(fd, *hd))
			return (1);
		fd->fd = open(fd->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		ft_here(fd, itscl[1]);
	}
	else
		fd->file = ft_create_del(tmp, itscl);
	if (!fd->file)
		return (1);
	return (0);
}

// itscl[0] = i, itscl[1] = type_red, itscl[2] = save_word
// itscl[3] = cmd_args_nums, itscl[4] = b_length
int	ft_fdnew(t_big_token *b_tokens, t_fd **fd, t_token **tmp, int itscl[7])
{
	t_fd	*new;

	new = ft_calloc(sizeof(t_fd), 1);
	if (!new)
		return (ft_putstr_error("Malloc error in ft_fdnew "));
	new->info = b_tokens->info;
	if (ft_fill_fdnew(new, tmp, itscl, &(b_tokens)->nb_hd))
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

// itscl[0] = i, itscl[1] = type_red, itscl[2] = save_word,
// itscl[3] = cmd_args_nums, itscl[4] = b_length
char	*ft_create_del(t_token **tmp, int itscl[5])
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
			else if (itscl[1] != 8)
				itscl[1] += 3;
			tmp[0] = tmp[0]->next;
			itscl[4]--;
		}
	}
	if (itscl[1] == 8)
		itscl[1] = 5;
	else
		itscl[1] = 2;
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
