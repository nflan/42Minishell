/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:11:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/03 14:59:23 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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
			if ((tmp_b))
				printf("\n\n\nhere is his par_pam1: %d\n", tmp_b->par);
			parse(&tmp_b->child, tokens, b_start, b_length);
		}
		if (piped(tokens, b_start, b_length))
		{
			divide_by_pipe(&tmp_b, tokens);
			if (tmp_b->child)
			{
				b_child = tmp_b->child;
				while (b_child)
				{
					if (b_child->par)
					{
						if ((tmp_b))
							printf("\n\n\nhere is his par_pam2: %d", tmp_b->par);
						parse(&b_child->child, tokens, b_child->ind_tok_start, b_child->length);
					}
					b_child = b_child->sibling;
				}
			}
		}
		tmp_b = tmp_b->sibling;
	}
}
