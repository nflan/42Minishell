/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 19:39:38 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/10 15:13:1t 1 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

int no_sib_has_child(t_big_token **b_tokens)
{
	t_big_token *tmp_b;

	tmp_b = *b_tokens;
	while (tmp_b)
	{
		if (tmp_b->child && tmp_b->sc == -1)
			return (0);
		tmp_b = tmp_b->sibling;
	}
	tmp_b = *b_tokens;
	if (tmp_b->type == TOK_CLEAN)
		return (1);
	if (tmp_b->type == TOK_LEFT_OR)
		return (2);
	if (tmp_b->type == TOK_LEFT_AND)
		return (3);
	if (tmp_b->type == TOK_LEFT_PIPE)
		return (4);
}

void exec_the_bulk(int sib_child, t_big_token **b_tokens)
{
	if (sib_child == 1)
		exec_simple(b_tokens);
	if (sib_child == 2)
		exec_or(b_tokens);
	if (sib_child == 3)
		exec_and(b_tokens);
	if (sib_child == 4)
		exec_pipex(b_tokens);
}

void	give_parent_sc(t_big_token **child, t_big_token **parent)
{

}

void rec_exec(t_big_token **b_tokens, t_token **tokens, int and_or)
{
	t_big_token *tmp_b;
	t_big_token *tmp_b_2;
	t_token *tmp_s;
	int pid;
	int status;
	int i;
	int j;
	int fc;

	fc = 0;
	i = 0;
	tmp_b = *b_tokens;
	tmp_b_2 = *b_tokens;
	tmp_s = *tokens;
	while (1)
	{
	while (tmp_b)
	{
		if (tmp_b->child && tmp_b->sc == -1)
			rec_exec(&(tmp_b->child), tokens, 0);
		else if (!i && no_sib_has_child(tmp_b))
		{
			exec_the_bulk(no_sib_has_child, tmp_b);
			if (tmp_b->parent)
				give_parent_sc(&(tmp_b), &(tmp_b->parent));
			return;
		}
		if (tmp_b->type == TOK_PIPE_LAST)
		{
			exec_the_bulk(4, b_tokens);
			if (tmp_b->parent)
				give_parent_sc(&(tmp_b), &(tmp_b->parent));
			return ;
		}
		else if (tmp_b->type == TOK_LEFT_AND && i == and_or)
		{
			fc = 1;
			break;
		}
		else if (tmp_b->type == TOK_LEFT_OR && i == and_or)
		{
			fc = 2;
			break;
		}
		else if (tmp_b->type == TOK_LAST)
		{
			fc = 3;
			break ;
		}
		tmp_b = tmp_b->sibling;
		i++;
	}
	if (tmp_b->sc != -1)
		//execute le bloc tmp_b tout seul and get the sc;
	if (fc == 1)
			{
				if (tmp_b->sc == 1)
					break ;
				else
				{
					tmp_b = b_tokens;
					i = 0;
					and_or++;
				}
			}
	else if (fc == 2)
		{
				if (tmp_b->sc == 0)
					break ;
				else
				{
					tmp_b = b_tokens;
					i = 0;
					and_or++;
				}
		}
	else if (fc == 3)
		break;
}
}
