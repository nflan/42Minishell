/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 17:57:08 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/26 18:22:49 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	divide_by_pipe(t_big_token **b_tokens, t_info *info)
{
	t_token *tmp_s;
	t_big_token *tmp_b;
	int btok_info[2];
//	int length_piped;
//	int start;
	int st_par;
	int end_par;
	int i;
	int j;

	tmp_s = info->tokens;
	tmp_b = *b_tokens;
	btok_info[0] = tmp_b->ind_tok_start;
	btok_info[1] = 0;
//	length_piped = 0;
//	start = tmp_b->ind_tok_start;
	i = 0;
	j = tmp_b->ind_tok_start;
	while (i < tmp_b->length)
	{

		move_tok_2_ind(&tmp_s, j);
		btok_info[1]++;
	//	length_piped++;
		if (tmp_s->token == TOK_EXPANDER_OP)
		{
			st_par = tmp_s->index;
			end_par = cl_par_ind(&tmp_s, tmp_s->index);
			btok_info[1] += (end_par - st_par);
		//	length_piped += (end_par - st_par);
			j += (end_par - st_par) + 1;
			i += (end_par - st_par) + 1;
		}
		else if (tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
		{
			btok_info[1]--;
			if (add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, btok_info, info))
				return (ft_putstr_error("in divide by pipe "));
			btok_info[0] = tmp_s->index + 1;
			btok_info[1] = 0;
	//		start = tmp_s->index + 1;
	//		length_piped = 0;
			i++;
			j = btok_info[0];
		}
		else
		{
			j++;
			i++;
		}
	}
	if (!((tmp_b)->child))
	{
		if (add_b_tok_sib_last(&((tmp_b)->child), TOK_CLEAN, btok_info, info))
				return (ft_putstr_error("in divide by pipe "));
	}
	else
		if (add_b_tok_sib_last(&((tmp_b)->child), TOK_PIPE_LAST, btok_info, info))
				return (ft_putstr_error("in divide by pipe "));
	if (handle_par(&(tmp_b->child), info))
		return (ft_putstr_error("in divie by pipe "));
	return (0);
}
