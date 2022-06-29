/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 19:39:38 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 12:22:48 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	no_sib_has_child(t_big_token *b_tokens)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	while (tmp_b)
	{
		if (tmp_b->child && tmp_b->sc == -1)
			return (0);
		tmp_b = tmp_b->sibling;
	}
	tmp_b = b_tokens;
	if (tmp_b->type == TOK_CLEAN)
		return (1);
	if (tmp_b->type == TOK_LEFT_OR)
		return (2);
	if (tmp_b->type == TOK_LEFT_AND)
		return (3);
	if (tmp_b->type == TOK_LEFT_PIPE)
		return (4);
	return (0);
}

int	ft_open_all_fdout(t_big_token *b_tokens, t_fd *fd)
{
	char	*err;

	err = NULL;
	if (fd)
	{
		while (fd)
		{
			if (!fd->red)
				fd->fd = open(fd->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd->fd = open(fd->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd->fd < 0)
			{
				b_tokens->sc = 1;
				fd->fd = 1;
				err = ft_strjoin("minishell: ", fd->file);
				if (!err)
					return (1);
				perror(err);
				free(err);
				return (1);
			}
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
	char	*err;

	err = NULL;
	if (tmp_fd)
	{
		while (tmp_fd)
		{
			tmp_fd->fd = open(tmp_fd->file, O_RDONLY);
			if (tmp_fd->fd < 0)
			{
				b_tokens->sc = 1;
				tmp_fd->fd = 0;
				err = ft_strjoin("minishell: ", tmp_fd->file);
				if (!err)
					return (1);
				perror(err);
				free(err);
				return (1);
			}
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
		while (b_tokens)
		{
			if (b_tokens->fd_out)
				ft_close_all_fd(b_tokens->fd_out, 1);
			if (b_tokens->fd_in)
				ft_close_all_fd(b_tokens->fd_in, 0);
			b_tokens = b_tokens->sibling;
		}
	}
}

int	ft_exec_pipex(t_info *info, t_big_token *b_tokens, int *pid)
{
	int	i;

	i = 0;
	while (b_tokens)
	{
		if (ft_wash_btoken(info, b_tokens))
			return (2147483647);
		if (b_tokens->sc == -1 || b_tokens->sc == 1)
		{
			if (ft_add_wildcards(info, b_tokens))
				return (ft_putstr_error("Wildcards error\n"));
			ft_launch_cmd_pipex(info, b_tokens, pid[i]);
			info->nb_cmd++;
			b_tokens->sc = info->status;
			i++;
		}
		b_tokens = b_tokens->sibling;
	}
	return (0);
}

int	ft_init_pipex(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;
	int			i;

	tmp_b = b_tokens;
	i = 0;
	while (tmp_b && ++i)
		tmp_b = tmp_b->sibling;
	info->pid = ft_calloc(sizeof(int), i);
	if (!info->pid)
		return (1);
	tmp_b = b_tokens;
	if (pipe(info->pdes) == -1)
		return (ft_error(5, info, NULL));
	if (ft_exec_pipex(info, b_tokens, info->pid) == 2147483647)
		return (2147483647);
	i = -1;
	while (++i < info->nb_cmd - 1)
		waitpid(info->pid[i], &info->pid[i], 0);
	if (info->pid)
	{
		free(info->pid);
		info->pid = NULL;
	}
	return (0);
}

int	ft_exec_simple(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	if (ft_wash_btoken(info, tmp_b))
		return (2147483647);
	if (tmp_b->sc == -1)
	{
		if (ft_add_wildcards(info, b_tokens))
			return (ft_putstr_error("Wildcards error\n"));
		ft_launch_cmd(info, tmp_b);
		tmp_b->sc = info->status;
	}
	ft_close_fd(b_tokens);
	return (0);
}

int	ft_check_dol(char *str)
{
	int	i;

	i = -1;
	if (str)
		while (str[++i])
			if (str[i] == '$')
				return (1);
	return (0);
}

int	ft_check_expand(t_token *token, int start, int length)
{
	t_token	*tmp;

	tmp = token;
	move_tok_2_ind(&tmp, start);
	if (tmp)
	{
		while (tmp && length--)
		{
			if (tmp->token == TOK_S_QUOTER || tmp->token == TOK_D_QUOTER)
				return (1);
			else if (tmp->token == TOK_WORD)
				if (ft_check_dol(tmp->value))
					return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}

int	exec_the_bulk(t_info *info, int sib_child, t_big_token *b)
{
	info->nb_cmd = 0;
	if (!ft_open_fd(b))
	{
		if (!b->par)
		{
			if (ft_check_expand(info->tokens, b->ind_tok_start, b->length))
			{
				dol_expand(&info->tokens, info, b);
				expanded_toks(&info->tokens, b->ind_tok_start, b->length);
				index_toks(&info->tokens);
				if (ft_noquote_args(b))
					return (1);
			}
			print_tab(b->cmd_args);
		}
		if (sib_child >= 1 && sib_child <= 3)
			ft_exec_simple(info, b);
		else if (sib_child == 4)
			ft_init_pipex(info, b);
	}
	ft_close_fd(b);
	return (0);
}

void	give_parent_sc(t_big_token **child, t_big_token **parent)
{
	(*parent)->sc = (*child)->sc;
	if ((*parent)->parent && (*parent)->par)
		give_parent_sc(parent, &(*parent)->parent);
}

int	rec_exec(t_info *info, t_big_token **b_tokens, int and_or)
{
	t_big_token	*tmp_b;
	int			i;
	int			fc;

	fc = 0;
	i = 0;
	tmp_b = *b_tokens;
	while (tmp_b)
	{
		if (tmp_b->child && tmp_b->sc == -1)
		{
			if (!tmp_b->par)
				rec_exec(info, &(tmp_b->child), 0);
		}
		else if ((!i && no_sib_has_child(tmp_b) && tmp_b->parent
				&& tmp_b->sc == -1 && tmp_b->type == TOK_LEFT_PIPE))
		{
			if (exec_the_bulk(info, no_sib_has_child(tmp_b), tmp_b))
				return (1);
			if (tmp_b->parent)
				give_parent_sc(&(tmp_b), &(tmp_b->parent));
			return (0);
		}
		if (tmp_b->type == TOK_PIPE_LAST)
		{
			if (exec_the_bulk(info, 4, *b_tokens))
				return (1);
			if (tmp_b->parent)
				give_parent_sc(&(tmp_b), &(tmp_b->parent));
			return (0);
		}
		if (tmp_b->type == TOK_LEFT_AND && i == and_or)
			fc = 1;
		else if (tmp_b->type == TOK_LEFT_OR && i == and_or)
			fc = 2;
		else if (tmp_b->type == TOK_LAST || tmp_b->type == TOK_CLEAN
			|| tmp_b->type == TOK_CLEAN_PIPED)
			fc = 3;
		if (fc)
			break ;
		tmp_b = tmp_b->sibling;
		i++;
	}
	if (tmp_b && tmp_b->sc == -1 && (!tmp_b->child || tmp_b->par))
	{
		if (exec_the_bulk(info, 1, tmp_b))
			return (1);
		if (tmp_b->parent)
			give_parent_sc(&(tmp_b), &(tmp_b->parent));
	}
	if (tmp_b && fc > 0 && fc < 3)
	{
		if ((fc == 1 && tmp_b->sc == 0) || (fc == 2 && tmp_b->sc))
		{
			rec_exec(info, b_tokens, and_or + 1);
			return (0);
		}
		else if (fc == 2 && !tmp_b->sc)
		{
			while (tmp_b && tmp_b->type != TOK_LEFT_AND)
			{
				tmp_b = tmp_b->sibling;
				and_or++;
			}
		}
		else if (fc == 1 && tmp_b->sc)
		{
			while (tmp_b && tmp_b->type != TOK_LEFT_OR)
			{
				tmp_b = tmp_b->sibling;
				and_or++;
			}
		}
		if (tmp_b)
			rec_exec(info, &(tmp_b), and_or);
	}
	return (0);
}
