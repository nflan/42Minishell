/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 17:57:08 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 22:49:47 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	dbp_1(t_big_token **tmp_b, int b_info[2], t_info *info)
{
	if (!(*tmp_b)->child)
		if (add_b_tok_sib_last(&((*tmp_b)->child), TOK_CLEAN, b_info, info))
				return (ft_putstr_error("in divide by pipe "));
	else
		if (add_b_tok_sib_last(&((*tmp_b)->child), TOK_PIPE_LAST, b_info, info))
				return (ft_putstr_error("in divide by pipe "));
	if (handle_par(&((*tmp_b)->child), info))
		return (ft_putstr_error("in divie by pipe "));
	return (0);
}

static void	dbp_2(t_big_token *tmp_b, int btok)

int	divide_by_pipe(t_big_token **b_tokens, t_info *info)
{
	t_token *tmp_s;
	t_big_token *tmp_b;
	int btok_info[2];
	int stend_par[2];
	int ij[2];

	tmp_s = info->tokens;
	tmp_b = *b_tokens;
	btok_info[0] = tmp_b->ind_tok_start;
	btok_info[1] = 0;
	ij[0] = 0;
	ij[1] = tmp_b->ind_tok_start;
	while (ij[0] < tmp_b->length)
	{

		move_tok_2_ind(&tmp_s, ij[1]);
		btok_info[1]++;
		if (tmp_s->token == TOK_EXPANDER_OP)
		{
			stend_par[0] = tmp_s->index;
			stend_par[1] = cl_par_ind(&tmp_s, tmp_s->index);
			btok_info[1] += (stend_par[1] - stend_par[0]);
			ij[1] += (stend_par[1] - stend_par[0]) + 1;
			ij[0] += (stend_par[1] - stend_par[0]) + 1;
		}
		else if (tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
		{
			btok_info[1]--;
			if (add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, btok_info, info))
				return (ft_putstr_error("in divide by pipe "));
			btok_info[0] = tmp_s->index + 1;
			btok_info[1] = 0;
			ij[0]++;
			ij[1] = btok_info[0];
		}
		else
		{
			ij[1]++;
			ij[0]++;
		}
	}
	return (dbp_1(&tmp_b, btok_info, info));
}

