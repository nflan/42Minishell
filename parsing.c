/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 11:11:34 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/02 12:19:11 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	parse(t_big_token **b_tokens, t_token **tokens, int start, int length)
{
	//maybe move tokens and check if we are at the end!!

	t_big_token	*tmp_b;
	t_token		*tmp_s;

	divide_by_or_and(b_tokens, tokens, start, length);
	tmp_b = *b_tokens;
	if (!tmp_b || (!tmp_b->par && tmp_b->type == TOK_CLEAN))
		return ;
	while (tmp_b)
	{
		if (tmp_b->par)
			parse(&tmp_b, tokens, tmp_b->ind_tok_start, tmp_b->length);
		if ()
		tmp_b = tmp_b->next;
	}
}
