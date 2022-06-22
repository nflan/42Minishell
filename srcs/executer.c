/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 19:39:38 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/22 12:15:55 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	sc;

int no_sib_has_child(t_big_token *b_tokens)
{
	t_big_token *tmp_b;

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

int	ft_open_all_fdout(t_big_token *b_tokens, t_fd *tmp_fd)
{
	char	*err;

	err = NULL;
	if (tmp_fd)
	{
		while (tmp_fd)
		{
		//	printf("file = %s\n", tmp_fd->file);
		//	printf("red = %d\n", tmp_fd->red);
			if (!tmp_fd->red)
				tmp_fd->fd = open(tmp_fd->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				tmp_fd->fd = open(tmp_fd->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (tmp_fd->fd < 0)
			{
				b_tokens->sc = 1;
				tmp_fd->fd = 1;
				sc = 1;
				err = ft_strjoin("minishell: ", tmp_fd->file);
				if (!err)
					return (1);
				perror(err);
				free(err);
				return (1);
			}
			else
				b_tokens->fdout = tmp_fd->fd;
			if (tmp_fd->next)
			{
				close(tmp_fd->fd);
				tmp_fd->fd = 0;
			}
			tmp_fd = tmp_fd->next;
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
				sc = 1;
				err = ft_strjoin("minishell: ", tmp_fd->file);
				if (err)
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
		else if (b_tokens->sc == -1 || b_tokens->sc == 1)
		{
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
	int	*pid;
	int	i;

	tmp_b = b_tokens;
	i = 0;
	while (tmp_b && ++i)
		tmp_b = tmp_b->sibling;
	pid = ft_calloc(sizeof(int), i);
		if (!pid)
			return (1);
	tmp_b = b_tokens;
	if (pipe(info->pdes) == -1)
		return (ft_error(5, info, NULL));
	if (ft_exec_pipex(info, b_tokens, pid) == 2147483647)
		return (2147483647);
	i = -1;
	while (++i < info->nb_cmd - 1)
		waitpid(pid[i], &pid[i], 0);
	free(pid);
	return (0);
}

void	ft_wdadd_back(t_wildcards **alst, t_wildcards *new)
{
	t_wildcards	*tmp;

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

int	ft_wildcardsnew(t_wildcards **wd, struct dirent *dir, DIR *fd)
{
	t_wildcards	*new;

	new = ft_calloc(sizeof(t_wildcards), 1);
	if (!new)
		return (ft_putstr_error("Malloc error in get_wildcards\n"));
	new->fd = fd;
	new->dir = dir;
	ft_wdadd_back(wd, new);
	return (0);
}

void	ft_print_wildcards(t_wildcards *wd)
{
	int	i;

	i = 0;
	if (wd)
	{
		while (wd)
		{
			printf("Fichier/Dossier %d: ", i++);
			printf("%s (type = %u)\n", wd->dir->d_name, wd->dir->d_type);
			wd = wd->next;
		}
	}
}

void	ft_free_wildcards(t_wildcards *wd)
{
	if (!wd)
		return ;
	else if (wd->next)
		ft_free_wildcards(wd->next);
	else
		closedir(wd->fd);
	if (wd)
		free(wd);
}

int	ft_get_wildcards(t_wildcards **wd)
{
	DIR				*dir;
	struct dirent	*send;
	char			*tofree;

	dir = NULL;
	tofree = NULL;
	tofree = getcwd(tofree, 0);
	if (!tofree)
		return (1);
	dir = opendir(tofree);
	if (!dir)
		return (free(tofree), perror("minishell"), 1);
	send = readdir(dir);
	while (send)
	{
		if (send)
			if (ft_wildcardsnew(wd, send, dir))
				return (closedir(dir), 1);
		send = readdir(dir);
	}
	free(tofree);
	return (0);
}

int	ft_keep(char *str, char *dir, int *i)
{
	if (!str || !dir)
		return (1);
	while (dir[*i] && *str != dir[*i])
		*i += 1;
	if (!dir[*i])
		return (1);
	return (0);
}

int	ft_do_keep(char *str, char *dir)
{
	int		i;

	i = 0;
	if (!str || !dir || (str[0] == '.' && dir[0] != '.') || (str[0] != '.' && dir[0] == '.'))
		return (1);
	if (str && dir)
	{
		while (*str && dir[i])
		{
			if (!ft_keep(str, dir, &i) || *str == '*' || *str == '/')
			{
				str++;
				if (*str == '*' || *str == '/')
					while (*str == '*' || *str == '/')
						str++;
			}
			else
				return (1);
		}
		if (*str && !dir[i])
			return (1);
	}
//	printf("str %s && dir = %s\n", str, dir);
	return (0);
}

int	ft_wd_nb_args(t_wildcards *wd, t_big_token *b_tokens, int i, int type)
{
	int			count;

	count = 0;
	if (wd)
	{
		while (wd)
		{
			if (wd->dir->d_type == type || (ft_strlen(b_tokens->cmd_args[i]) == 1 && b_tokens->cmd_args[i][0] == '*'))
				if (!ft_do_keep(b_tokens->cmd_args[i], wd->dir->d_name))
					count++;
			wd = wd->next;
		}
	}
	return (count);
}

char	**ft_fill_old_args(t_big_token *b_tokens, char **tmp, int j, int list)
{
	static	int	i = 0;

	if (!list)
	{
		i = 0;
		while (i < j)
		{
			tmp[i] = ft_strdup(b_tokens->cmd_args[i]);
			if (!*tmp[i])
				return (NULL);
			i++;
		}
	}
	else
	{
		while (j < b_tokens->cmd_args_num - 1)
		{
			i++;
			tmp[j] = ft_strdup(b_tokens->cmd_args[i]);
			if (!tmp[j])
				return (NULL);
			j++;
		}
	}
	return (tmp);
}

int	ft_realloc_args(t_wildcards *wd, t_big_token *b_tokens, int i, int type)
{
	char	**tmp;
	int		count;
	int		j;

	j = 0;
	count = ft_wd_nb_args(wd, b_tokens, i, type);
	if (!count)
		return (0);
//	printf("count = %d && type = %d\n", count, type);
//	printf("cmd_args_num au debut = %d\n", b_tokens->cmd_args_num);
	tmp = ft_calloc(sizeof(char *), b_tokens->cmd_args_num + count);
	if (!tmp)
		return (1);
	tmp = ft_fill_old_args(b_tokens, tmp, i, 0);
	if (!tmp)
		return (1);
	while (wd && j < count)
	{
		while (wd->next && wd->dir->d_type != type && ft_strlen(b_tokens->cmd_args[i]) != 1 && b_tokens->cmd_args[i][0] != '*')
			wd = wd->next;
		if (wd && !ft_do_keep(b_tokens->cmd_args[i], wd->dir->d_name) && wd->dir->d_type == type)
		{
			if (wd->dir->d_type == 4)
				tmp[i + j] = ft_strjoin(wd->dir->d_name, "/");
			else
				tmp[i + j] = ft_strdup(wd->dir->d_name);
			if (!tmp[i + j])
				return (1);
			j++;
		}
		wd = wd->next;
	}
	b_tokens->cmd_args_num += count;
//	print_tab(tmp);
	tmp = ft_fill_old_args(b_tokens, tmp, j + i, 1);
	if (!tmp)
		return (1);
	ft_free_split(b_tokens->cmd_args);
//	print_tab(tmp);
	b_tokens->cmd_args = tmp;
	return (0);
}

int	ft_do_wildcards(t_big_token *b_tokens, int i)
{
	t_wildcards	*wd;
	int			type;

	type = 8;
	wd = NULL;
	if (ft_get_wildcards(&wd))
		return (ft_free_wildcards(wd), 1);
//	ft_print_wildcards(wd);
	if (b_tokens->cmd_args[i][ft_strlen(b_tokens->cmd_args[i]) - 1] == '/')
		type = 4;
	if (ft_realloc_args(wd, b_tokens, i, type))
		return (ft_free_wildcards(wd), 1);
	ft_free_wildcards(wd);
	return (0);
}

int	ft_check_wildcards(t_info *info, t_big_token *b_tokens, int i)
{
	t_token	*tmp_s;

	tmp_s = info->tokens;
	if (!info || !b_tokens || !tmp_s)
		return (1);
	move_tok_2_ind(&tmp_s, b_tokens->ind_tok_start + (i * 2));
	if (tmp_s && ft_strchr(b_tokens->cmd_args[i], '*') && tmp_s->token != TOK_QUOTER && tmp_s->token != TOK_D_QUOTER && tmp_s->token != TOK_WORD_S_QUOTED && tmp_s->token != TOK_WORD_D_QUOTED)
		return (0);
	return (1);
}

int	ft_add_wildcards(t_info *info, t_big_token *b_tokens)
{
	int	i;

	i = 0;
	if (b_tokens->cmd_args)
	{
		while (b_tokens->cmd_args[i])
		{
			if (!ft_check_wildcards(info, b_tokens, i))
				if (ft_do_wildcards(b_tokens, i))
					return (1);
			i++;
		}
	}
	print_tab(b_tokens->cmd_args);
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
			return (1);
		ft_launch_cmd(info, tmp_b);
		tmp_b->sc = info->status;
	}
	return (0);
}

int exec_the_bulk(t_info *info, int sib_child, t_big_token *b_tokens)
{
	info->nb_cmd = 0;
	if (!ft_open_fd(b_tokens))
	{
		if (sib_child >= 1 && sib_child <= 3)
			ft_exec_simple(info, b_tokens);
		else if (sib_child == 4)
			ft_init_pipex(info, b_tokens);
	}
	ft_close_fd(b_tokens);
	return (0);
}

void	give_parent_sc(t_big_token **child, t_big_token **parent)
{
//	printf("tmpb parent 1 [0] = %s\n", child[0]->parent->cmd_args[0]);
	(*parent)->sc = (*child)->sc;
//	printf("tmpb parent 1 SC = %d\n", child[0]->parent->sc);
	if ((*parent)->parent && (*parent)->par)
		give_parent_sc(parent, &(*parent)->parent);
}

int	rec_exec(t_info *info, t_big_token **b_tokens, int and_or)
{
	t_big_token *tmp_b;
	int i;
	int fc;

	fc = 0;
	i = 0;
	tmp_b = *b_tokens;
//	printf("b_tokens->type = %d\n", (*b_tokens)->type);
	while (tmp_b)
	{
	//	printf("tmpb [0] = %s\n",tmp_b->cmd_args[0]);
		if (tmp_b->child && tmp_b->sc == -1)
		{
			if (!tmp_b->par)
				rec_exec(info, &(tmp_b->child), 0);
		}
		else if ((!i && no_sib_has_child(tmp_b) && tmp_b->parent && tmp_b->sc == -1 && tmp_b->type == TOK_LEFT_PIPE))
		{
//			printf("value b_token si sibling pas child: "); print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length); printf("\n"); printf("je lance sibling sans child !! (start = %d && length = %d)\n", tmp_b->ind_tok_start, tmp_b->length);
			if (exec_the_bulk(info, no_sib_has_child(tmp_b), tmp_b))
				return (1);
			if (tmp_b->parent)
				give_parent_sc(&(tmp_b), &(tmp_b->parent));
			return (0);
		}
		if (tmp_b->type == TOK_PIPE_LAST)
		{
//				printf("value b_token dans le pipe last\n"); print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length); printf("\nFC == 1 && b_tok->sc = %d\n", tmp_b->sc);
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
		else if (tmp_b->type == TOK_LAST || tmp_b->type == TOK_CLEAN || tmp_b->type == TOK_CLEAN_PIPED)
			fc = 3;
		if (fc)
			break ;
		tmp_b = tmp_b->sibling;
		i++;
	}
	if (tmp_b && tmp_b->sc == -1 && (!tmp_b->child || tmp_b->par)) //execute le bloc tmp_b tout seul and get the sc;
	{
	//	printf("value b_token (commande solo) && tmp_b->sc = %d\n", tmp_b->sc);
	//	print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
	//	printf("\n");
		if (exec_the_bulk(info, 1, tmp_b))
			return (1);
		if (tmp_b->parent)
			give_parent_sc(&(tmp_b), &(tmp_b->parent));
	}
	if (tmp_b && fc > 0 && fc < 3)
	{
	//	printf("value b_token dans le FC (%d)\n", fc);
	//	print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
	//	printf("\nb_tok->sc = %d\n", tmp_b->sc);
		if ((fc == 1 && sc == 0) || (fc == 2 && sc))
		{
			rec_exec(info, b_tokens, and_or + 1);
			return (0);
		}
		else if (fc == 2 && !sc)
		{
			while (tmp_b && tmp_b->type != TOK_LEFT_AND)
			{
				tmp_b = tmp_b->sibling;
				and_or++;
			}
		}
		else if (fc == 1 && sc)
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
//	printf("je pars des parentheses\n");
	return (0);
}
