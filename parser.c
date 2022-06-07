/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:11:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/07 14:19:43 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	give_parent(t_big_token **b_child, t_big_token **parent)
{
	(*b_child)->parent = *parent;
}

void parse(t_big_token **b_tokens, t_token **tokens, int start, int length)
{
	t_big_token *tmp_b;
	t_big_token *b_child;
	t_token *tmp_s;
	int b_start;
	int b_length;

	divide_by_or_and(b_tokens, tokens, start, length);
	tmp_b = *b_tokens;
	if (!tmp_b || (!tmp_b->par && tmp_b->type == TOK_CLEAN))
		return;
	while (tmp_b)
	{
		b_start = tmp_b->ind_tok_start;
		b_length = tmp_b->length;
		if (tmp_b->par)
		{
			parse(&tmp_b->child, tokens, b_start, b_length);
			if (tmp_b->child)
				give_parent(&(tmp_b->child), &tmp_b);
		}
		else if (piped(tokens, b_start, b_length))
		{
			divide_by_pipe(&tmp_b, tokens);
			if (tmp_b->child)
			{
				b_child = tmp_b->child;
				give_parent(&b_child, &tmp_b);
				while (b_child)
				{
					if (b_child->par)
					{
						parse(&b_child->child, tokens, b_child->ind_tok_start, b_child->length);
						if (b_child->child)
							give_parent(&(b_child->child), &b_child);
					}
					b_child = b_child->sibling;
				}
			}
		}
		tmp_b = tmp_b->sibling;
		if (tmp_b)
		{
		printf("sibling while: ");
		print_s_tokens(tokens, tmp_b->ind_tok_start, tmp_b->length);
		printf("\n");
		}
	}
}
