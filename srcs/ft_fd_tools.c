/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 02:51:58 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/26 13:20:40 by nflan            ###   ########.fr       */
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
		if (!fd->delimitator)
			return (ft_putstr_error("Malloc error\n"));
		if (ft_create_tmp(fd, *hd))
			return (free(fd->delimitator), ft_putstr_error("Malloc error\n"));
		fd->fd = open(fd->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (fd->fd < 0)
			return (free(fd->file), free(fd->delimitator),
				ft_putstr_error("Open Heredoc error"));
		if (ft_here(fd, itscl[1]))
			return (free(fd->file), free(fd->delimitator), 1);
	}
	else
		fd->file = ft_create_del(tmp, itscl);
	if (!fd->file)
		return (ft_putstr_error("Malloc error\n"));
	return (0);
}

// itscl[0] = i, itscl[1] = type_red, itscl[2] = save_word
// itscl[3] = cmd_args_nums, itscl[4] = b_length
int	ft_fdnew(t_big_token *b_tokens, t_fd **fd, t_token **tmp, int itscl[7])
{
	t_fd	*new;

	new = ft_calloc(sizeof(t_fd), 1);
	if (!new)
		return (ft_putstr_error("Malloc error\n"));
	new->info = b_tokens->info;
	if (itscl[1] == 1 || itscl[1] == 2)
		new->inout = 1;
	else
		new->inout = 2;
	if (ft_fill_fdnew(new, tmp, itscl, &(b_tokens)->nb_hd))
		return (free(new), 1);
	ft_fdadd_back(fd, new);
	return (0);
}

int	ft_create_tmp(t_fd *fd, int hd)
{
	unsigned long long	i;
	char				*tmp;

	i = 0;
	if (hd)
	{
		fd->file = ft_strdup("/tmp/.tmp_hd_0");
		if (!fd->file)
			return (1);
		while (access(fd->file, F_OK) == 0)
		{
			tmp = NULL;
			i++;
			free(fd->file);
			tmp = ft_itoa(i);
			if (!tmp)
				return (1);
			fd->file = ft_strjoin("/tmp/.tmp_hd_", tmp);
			if (!fd->file)
				return (free(tmp), 1);
			free(tmp);
		}
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
		while (tmp[0] && create_del_cond(tmp[0]->token))
		{
			if (tmp[0]->token != TOK_QUOTER && tmp[0]->token != TOK_D_QUOTER
				&& tmp[0]->token != TOK_S_QUOTER)
				del = ft_strjoin_free(del, tmp[0]->value, 1);
			if (!del)
				return (NULL);
			if (itscl[1] != 8)
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
