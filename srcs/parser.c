/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:11:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/03 18:40:50 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

void parse(t_big_token **b_tokens, t_token **tokens, int start, int length)
{
	t_big_token *tmp_b;
	t_big_token *b_child;
	int b_start;
	int b_length;

	divide_by_or_and(b_tokens, tokens, start, length);
	tmp_b = *b_tokens;
	if (!tmp_b || (!tmp_b->par && tmp_b->type == TOK_CLEAN))
	{
		printf("tmp_b = %d\n", tmp_b->length);
		printf("tmp_b = %d\n", tmp_b->par);
		print_s_tokens(tokens, tmp_b->ind_tok_start, tmp_b->length);
		printf("\nallo\n");
		return;
	}
	while (tmp_b)
	{
		b_start = tmp_b->ind_tok_start;
		b_length = tmp_b->length;
		if (tmp_b->par)
		{
		//	if ((tmp_b))
		//		printf("\n\n\nhere is his par_pam1: %d\n", tmp_b->par);
			parse(&tmp_b->child, tokens, b_start, b_length);
		}
		else if (piped(tokens, b_start, b_length))
		{
			divide_by_pipe(&tmp_b, tokens);
			if (tmp_b->child)
			{
				b_child = tmp_b->child;
				while (b_child)
				{
					if (b_child->par)
					{
				//	printf("J'ai des parantheses ! : ");
				//	print_s_tokens(tokens, b_child->ind_tok_start, b_child->length);
				//	printf("\n");
		//				if ((tmp_b))
		//					printf("\n\n\nhere is his par_pam2: %d", tmp_b->par);
						parse(&b_child->child, tokens, b_child->ind_tok_start, b_child->length);
					}
					b_child = b_child->sibling;
				}
			}
		}
		tmp_b = tmp_b->sibling;
	}
}
