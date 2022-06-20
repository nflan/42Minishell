/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 17:57:08 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/20 11:03:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void divide_by_pipe(t_big_token **b_tokens, t_token **tokens)
{
	t_token *tmp_s;
	t_big_token *tmp_b;
	int length_piped;
	int start;
	int st_par;
	int end_par;
	int i;
	int j;

	tmp_s = *tokens;
	tmp_b = *b_tokens;
	length_piped = 0;
	start = tmp_b->ind_tok_start;
	i = 0;
	j = start;
	while (i < tmp_b->length)
	{

		move_tok_2_ind(&tmp_s, j);
		length_piped++;
		if (tmp_s->token == TOK_EXPANDER_OP)
		{
			st_par = tmp_s->index;
			end_par = cl_par_ind(&tmp_s, tmp_s->index);
			length_piped += (end_par - st_par);
			j += (end_par - st_par) + 1;
			i += (end_par - st_par) + 1;
		}
		else if (tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
		{
			add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, start, length_piped - 1);
			start = tmp_s->index + 1;
			length_piped = 0;
			i++;
			j = start;
		}
		else
		{
			j++;
			i++;
		}
	}
	if (!((tmp_b)->child))
		add_b_tok_sib_last(&((tmp_b)->child), TOK_CLEAN, start, length_piped);
	else
		add_b_tok_sib_last(&((tmp_b)->child), TOK_PIPE_LAST, start, length_piped);
	handle_par(&(tmp_b->child), tokens);
}
