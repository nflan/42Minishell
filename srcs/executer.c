/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 19:39:38 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/16 16:42:35 by nflan            ###   ########.fr       */
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

int	ft_open_fd(t_big_token *b_tokens)
{
	int	i;

	i = 0;
	while (b_tokens)
	{
		i = 0;
		if (b_tokens->outfile)
		{
			while (b_tokens->outfile[i])
			{
				if (b_tokens->red_out[i])
					b_tokens->fdout[i] = open(b_tokens->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else
					b_tokens->fdout[i] = open(b_tokens->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		//		printf("ligne %d --> rd_inouthd[1] = %d && fdout[i] %d && outfile[i] = %s && red_out[i] = %d\n", i, b_tokens->rd_inouthd[1], b_tokens->fdout[i], b_tokens->outfile[i], b_tokens->red_out[i]);
				if (b_tokens->fdout[i] < 0)
					return (1);
				i++;
			}
		}
		if (!b_tokens->fdout)
			b_tokens->fdout = ft_calloc(sizeof(int), 1);
		if (!i)
			b_tokens->fdout[i] = 1;
		i = 0;
		if (b_tokens->infile)
		{
			while (b_tokens->infile[i])
			{
				b_tokens->fdin[i] = open(b_tokens->infile[i], O_RDONLY);
				if (b_tokens->fdin[i] < 0)
					return (1);
				i++;
			}
		}
		if (!b_tokens->fdin)
			b_tokens->fdin = ft_calloc(sizeof(int), 1);
		if (!i)
			b_tokens->fdin[i] = 0;
		b_tokens = b_tokens->sibling;
	}
	return (0);
}

int	ft_close_fd(t_big_token *b_tokens)
{
	int	i;

	i = 0;
	if (b_tokens->outfile)
	{
		while (b_tokens->fdout[i])
		{
			close(b_tokens->fdout[i]);
			i++;
		}
	}
	i = 0;
	if (b_tokens->infile)
	{
		while (b_tokens->fdin[i])
		{
			close(b_tokens->fdin[i]);
			i++;
		}
	}
//	if (!i)
//		close(b_tokens->fdin[0]);
	return (0);
}

int	ft_exec_pipex(t_info *info, t_big_token *b_tokens, int *pid)
{
	int	i;

	i = 0;
	while (b_tokens)
	{
		if (ft_wash_btoken(info, b_tokens))
			return (2147483647);
		else if (b_tokens->sc == -1)
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

int	ft_exec_simple(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	if (ft_wash_btoken(info, tmp_b))
		return (2147483647);
	if (tmp_b->sc == -1)
	{
		ft_launch_cmd(info, tmp_b);
		tmp_b->sc = info->status;
	}
	return (0);
}

int exec_the_bulk(t_info *info, int sib_child, t_big_token *b_tokens)
{
	info->nb_cmd = 0;
	if (ft_open_fd(b_tokens))
		return (ft_error(6, info, NULL));
	if (sib_child >= 1 && sib_child <= 3)
		ft_exec_simple(info, b_tokens);
	else if (sib_child == 4)
		ft_init_pipex(info, b_tokens);
	ft_close_fd(b_tokens);
	return (0);
}

void	give_parent_sc(t_big_token **child, t_big_token **parent)
{
	(*parent)->sc = (*child)->sc;
}

int	rec_exec(t_info *info, t_big_token **b_tokens, int and_or)
{
	t_big_token *tmp_b;
	t_token *tmp_s;
	int i;
	int fc;

	fc = 0;
	i = 0;
	tmp_b = *b_tokens;
	tmp_s = info->tokens;
//	printf("b_tokens->type = %d\n", (*b_tokens)->type);
	while (tmp_b)
	{
//		printf("COUCOU, je suis apres le tmp_b "); print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length); printf("\n"); printf("type = %d\n", tmp_b->type);
//		if ((!tmp_b->parent && !i) || (tmp_b->parent && tmp_b->parent->par))
		if (tmp_b->child && tmp_b->sc == -1)
		{
			if (!tmp_b->par)// || (tmp_b->par && !tmp_b->sibling))
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
		else if (tmp_b->type == TOK_LAST || tmp_b->type == TOK_CLEAN)
			fc = 3;
		if (fc)
			break ;
		tmp_b = tmp_b->sibling;
		i++;
	}
	if (tmp_b && tmp_b->sc == -1)// && !tmp_b->child) //execute le bloc tmp_b tout seul and get the sc;
	{
//		printf("value b_token (commande solo) \n");
//		print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
//		printf("\n");
		if (exec_the_bulk(info, 1, tmp_b))
			return (1);
		if (tmp_b->parent)
			give_parent_sc(&(tmp_b), &(tmp_b->parent));
	}
	if (tmp_b && fc > 0 && fc < 3)
	{
		//printf("value b_token dans le FC (%d)\n", fc);
		//print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
		//printf("\nb_tok->sc = %d\n", tmp_b->sc);
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
