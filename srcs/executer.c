/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 19:39:38 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/13 22:25:09 by nflan            ###   ########.fr       */
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

int	ft_exec_pipex(t_info *info, t_big_token *b_tokens, int sib_child)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	//	if (pipe(info->pdes) == -1)
	//		return (ft_error(5, info, NULL));
	while (tmp_b)
	{
		//			printf("value b_token dans pipex et sc = %d && fdin = %d && fdout = %d && done = %d\n", tmp_b->sc, tmp_b->fdin, tmp_b->fdout, tmp_b->done);
		//			print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
		//			printf("\n");
		if (ft_wash_btoken(info, tmp_b))
			return (2147483647);
		//	if (tmp_b->sc == 0)
		//		break;
		else if (tmp_b->sc == -1 && !tmp_b->done)
		{
			ft_launch_cmd(info, tmp_b, sib_child);
			info->nb_cmd++;
			tmp_b->done = 1;
			tmp_b->sc = info->status;
		}
		if (tmp_b->sibling)
			tmp_b = tmp_b->sibling;
		else
			break ;
	}
	return (0);
}

int	ft_exec_and(t_info *info, t_big_token *b_tokens, int sib_child)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	//	if (pipe(info->pdes) == -1)
	//		return (ft_error(5, info, NULL));
	while (tmp_b && info->status == 0)
	{
		if (ft_wash_btoken(info, tmp_b))
			return (2147483647);
		if (tmp_b->sc == -1 || !tmp_b->done)
		{
			ft_launch_cmd(info, tmp_b, sib_child);
			info->nb_cmd++;
			tmp_b->done = 1;
			tmp_b->sc = info->status;
		}
		if (tmp_b->sc == 0 && tmp_b->sibling)
			tmp_b = tmp_b->sibling;
		else
			break ;
	}
	return (0);
}

int	ft_exec_or(t_info *info, t_big_token *b_tokens, int sib_child)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	//		printf("value b_token\n");
	//		print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
	//		printf("\n");
	//	if (pipe(info->pdes) == -1)
	//		return (ft_error(5, info, NULL));
	while (tmp_b)
	{
		if (ft_wash_btoken(info, tmp_b))
			return (2147483647);
		if (tmp_b->sc == -1 || !tmp_b->done)
		{
			ft_launch_cmd(info, tmp_b, sib_child);
			info->nb_cmd++;
			tmp_b->done = 1;
			tmp_b->sc = info->status;
		}
		if (tmp_b->sc != 0 && tmp_b->sibling)
			tmp_b = tmp_b->sibling;
		else
			break ;
	}
	return (0);
}

int	ft_exec_simple(t_info *info, t_big_token *b_tokens, int sib_child)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	//		printf("value b_token\n");
	//		print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
	//		printf("\n");
	//	if (pipe(info->pdes) == -1)
	//		return (ft_error(5, info, NULL));
	if (ft_wash_btoken(info, tmp_b))
		return (2147483647);
	if (tmp_b->sc == -1 || !tmp_b->done)
	{
		ft_launch_cmd(info, tmp_b, sib_child);
		tmp_b->done = 1;
		tmp_b->sc = info->status;
	}
	//		printf("allo\n");
	return (0);
}

void exec_the_bulk(t_info *info, int sib_child, t_big_token *b_tokens)
{
	//	printf("value b_token\n");
	//	print_s_tokens(&info->tokens, b_tokens->ind_tok_start, b_tokens->length);
	//	printf("\n");
	//	printf("b_token fdin = %d && fdout = %d\n", b_tokens->fdin, b_tokens->fdout);
	info->nb_cmd = 0;
	if (sib_child == 1)
	{
		ft_exec_simple(info, b_tokens, sib_child);
	}
	//		exec_simple(b_tokens);
	if (sib_child == 2)
	{
		ft_exec_or(info, b_tokens, sib_child);
	}
	//		exec_or(b_tokens);
	if (sib_child == 3)
	{
		ft_exec_and(info, b_tokens, sib_child);
	}
	//		exec_and(b_tokens);
	if (sib_child == 4)
	{
		ft_exec_pipex(info, b_tokens, sib_child);
	}
	//		exec_pipex(b_tokens);
	if (info->nb_cmd)
	{
		while (info->nb_cmd > 1)
		{
			info->nb_cmd--;
			//	printf("je wait une commande\n");
			wait(NULL);
		}
	}
	b_tokens->sc = info->status;
	//	printf("b_tok->sc = %d\n", b_tokens->sc);
}

void	give_parent_sc(t_big_token **child, t_big_token **parent)
{
	(*parent)->sc = (*child)->sc;
	(*parent)->done = (*child)->done;
	(*parent)->fdout = (*child)->fdout;
}

void	ft_exec_par(t_info *info, t_big_token *tmp_b, int and_or)
{
	pid_t	child_par;
	//	int		tmp[2];

	child_par = -1;
	//	if (pipe(tmp) == -1)
	//		return ;
	//	tmp[1] = info->pdes[1];
	//	if (ft_lead_fd(info, tmp_b, NULL))
	//		return ;
	child_par = fork();
	if ((int) child_par == -1)
		return ;
	else if ((int) child_par == 0)
	{
		//		dup2(tmp_b->fdin, STDIN_FILENO);
		//		dup2(info->pdes[1], STDOUT_FILENO);
		//		close(tmp[0]);
		rec_exec(info, &(tmp_b->child), and_or);
		//		printf("je suis dans par mais je sors pas\n");
		//		close(tmp[1]);
		ft_exit_cmd(info, NULL);
	}
	waitpid(child_par, &child_par, 0);
	//	if (info->pdes[0] != 0)
	//		close(info->pdes[0]);
	//	dup2(info->pdes[0], tmp[0]);
	//	close(tmp[1]);
	//	close(info->pdes[1]);
	//	ft_close_cmd(info, tmp_b, 0);
	tmp_b->sc = info->status;
	if (tmp_b->parent)
		give_parent_sc(&(tmp_b), &(tmp_b->parent));
	//	return ;
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
	//printf("type = %d\n", (*b_tokens)->type);
	while (tmp_b)
	{
		//printf("COUCOU, je suis apres le tmp_b "); print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length); printf("\n"); printf("type = %d\n", (*b_tokens)->type);
		if (!tmp_b->parent && !i)
		{
			printf("hello, j'ai cree un pipe (and_or = %d): ", and_or); print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length); printf("\n");
			if (pipe(info->pdes) == -1)
				return (1);
		}
		if (tmp_b->par)
			ft_exec_par(info, tmp_b, and_or);
		else if (tmp_b->child && tmp_b->sc == -1)
			rec_exec(info, &(tmp_b->child), 0);
		else if (!i && no_sib_has_child(tmp_b) && tmp_b->sc == -1)
		{
			//printf("value b_token si sibling pas child: "); print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length); printf("\n"); printf("je lance sibling sans child !! (start = %d && length = %d)\n", tmp_b->ind_tok_start, tmp_b->length);
			exec_the_bulk(info, no_sib_has_child(tmp_b), tmp_b);
			if (tmp_b->parent)
				give_parent_sc(&(tmp_b), &(tmp_b->parent));
			return (0);
		}
		if (tmp_b->type == TOK_PIPE_LAST)
		{
			//printf("value b_token dans le pipe last\n"); print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length); printf("\nFC == 1 && b_tok->sc = %d\n", tmp_b->sc);
			exec_the_bulk(info, 4, *b_tokens);
			if (tmp_b->parent)
				give_parent_sc(&(tmp_b), &(tmp_b->parent));
			return (0);
		}
		else if (tmp_b->type == TOK_LEFT_AND && i == and_or)
			fc = 1;
		else if (tmp_b->type == TOK_LEFT_OR && i == and_or)
			fc = 2;
		else if (tmp_b->type == TOK_LAST)
			fc = 3;
		if (fc)
			break ;
		tmp_b = tmp_b->sibling;
		i++;
	}
	if (tmp_b && tmp_b->sc == -1 && !tmp_b->child) //execute le bloc tmp_b tout seul and get the sc;
	{
		//printf("value b_token (commande solo) \n");
		//print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
		//printf("\n");
		exec_the_bulk(info, 1, tmp_b);
		if (tmp_b->parent)
			give_parent_sc(&(tmp_b), &(tmp_b->parent));
	}
	if (tmp_b && fc)
	{
		//printf("value b_token dans le FC (%d)\n", fc);
		//print_s_tokens(&info->tokens, tmp_b->ind_tok_start, tmp_b->length);
		//printf("\nb_tok->sc = %d\n", tmp_b->sc);
		if ((fc == 1 && sc == 0) || (fc == 2 && sc))
		{
			i = 0;
			rec_exec(info, b_tokens, and_or + 1);
		}
	}
	return (0);
}
