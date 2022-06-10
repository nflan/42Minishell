/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 19:39:38 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/10 12:06:10 by omoudni          ###   ########.fr       */
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
		if (tmp_b->child)
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

void rec_exec(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token *tmp_b;
	t_big_token *tmp_b_2;
	t_token *tmp_s;
	int pid;
	int status;
	int i;
	int	abd_or;

	i = 0;
	and_or =0;
	tmp_b = *b_tokens;
	tmp_b_2 = *b_tokens;
	tmp_s = *tokens;
	if (!tmp_b || (!tmp_b->par && tmp_b->type == TOK_CLEAN))
	{
		extract_fds(&tmp_b, tokens);
		// exec(tmp_b);
		return;
	}

	else

		while (tmp_b)
		{
			if (!i && no_sib_has_child(tmp_b))
			{
				exec_the_bulk(no_sib_has_child, tmp_b); // should contain the fd extraction & cmd extraction
				if (tmp_b->parent)
					give_parent_sc(&(tmp_b), &(tmp_b->parent));
					return ;
			}
			// if (!tmp_b->child)
			// {
			// 	extract_fds(&tmp_b, tokens);
			// 	extract_cmds(&tmp_b, tokens);
			// 	// exec(tmp_b);
			// 	// if (tmp_b->parent)
			// 	// give_sc_to_par(&(tmp_b), &(tmp_b->parent));
			// }
			else if (tmp_b->child)
			{
				// if (tmp_b->par)
				// {
				// 	pid = fork();
				// 	if (pid < 0)
				// 		printf("Error while forking!!\n");
				// 	if (!pid)
				// 	{
				// 		rec_exec_1((tmp_b->child)
				// 		give_parent_sc(&(tmp_b->child), tmp_b);
				// 	}
				// 	waitpid(pid, &status, 0);
				// }
				// else
				rec_exec(&(tmp_b->child), tokens);
			}
			tmp_b = tmp_b->sibling;
			if (tmp_b->type == TOK_LEFT_AND && !i)
				break;
			i++;
		}
		if (btok_sc(tmp_b_2))
		{
			exec(btok_sc(tmp_b_2), tmp_b_2);
		}
		//while sc put a pointer to the beginning of the ll and start again
}
