/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:11:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/07 20:52:53 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	give_parent(t_big_token **b_child, t_big_token **parent)
{
	(*b_child)->parent = *parent;
}

void	sub_parse_1(t_big_token **tmp_b, t_token **tokens, int b_start, int b_length)
{
	parse(&((*tmp_b)->child), tokens, b_start, b_length);
	if ((*tmp_b)->child)
		give_parent(&((*tmp_b)->child), tmp_b);
}

void	sub_parse_2(t_big_token **tmp, t_big_token **tmp_b, t_token **tokens)
{
	t_big_token *b_child;

	b_child = *tmp;
	give_parent(&b_child, tmp_b);
	while (b_child)
	{
		if ((b_child)->par)
			sub_parse_1(&b_child, tokens, (b_child)->ind_tok_start, (b_child)->length);
		(b_child) = (b_child)->sibling;
	}
}

void	parse(t_big_token **b_tokens, t_token **tokens, int start, int length)
{
	t_big_token	*tmp_b;
	t_big_token	*b_child;

	divide_by_or_and(b_tokens, tokens, start, length);
	tmp_b = *b_tokens;
	if (!tmp_b || (!tmp_b->par && tmp_b->type == TOK_CLEAN))
		return;
	while (tmp_b)
	{
		if (tmp_b->par)
			sub_parse_1(&tmp_b, tokens, tmp_b->ind_tok_start, tmp_b->length);
		else if (piped(tokens, tmp_b->ind_tok_start, tmp_b->length))
		{
			divide_by_pipe(&tmp_b, tokens);
			if (tmp_b->child)
				sub_parse_2(&tmp_b->child, &tmp_b, tokens);
		}
		tmp_b = tmp_b->sibling;
	}
}
