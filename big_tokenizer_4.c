/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/10 19:38:12 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static void handle_par_1(t_token **tmp_s, t_big_token *tmp_b, int *adv_steps, int *to_reduce)
{
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + 1);
	(*adv_steps)++;
	(*to_reduce)++;
}

static void handle_par_2(t_token **tmp_s, t_big_token *tmp_b, int *to_reduce, t_token *tokens)
{
	*tmp_s = tokens;
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + tmp_b->length - 2);
	(*to_reduce)++;
}

static void handle_par_3(t_big_token **tmp_b, int to_reduce, int adv_steps)
{
	(*tmp_b)->par = 1;
	(*tmp_b)->length -= (2 + to_reduce);
	(*tmp_b)->ind_tok_start += (1 + adv_steps);
}

static void init_params(int *adv_steps, int *to_reduce)
{
	(*adv_steps) = 0;
	(*to_reduce) = 0;
}

// void handle_par(t_big_token **b_tokens, t_token **tokens)
// {
// 	t_big_token *tmp_b;
// 	t_token		*tmp_s;
// 	int			params[2];
// 	int			st_par;
// 	int			end_par;

// 	tmp_b = *b_tokens;
// 	tmp_s = *tokens;
// 	while (tmp_b)
// 	{
// 	init_params(&(params[0]), &(params[1]));
// 		tmp_s = *tokens;
// 		move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
// 		if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
// 		{
// 			handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
// 		}
// 		if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
// 		{
// 			st_par = tmp_s->index;
// 			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
// 			if (tmp_s->token == TOK_SEP)
// 				handle_par_2(&tmp_s, tmp_b, &(params[1]), *tokens);
// 			if (tmp_s->token == TOK_EXPANDER_CL)
// 			{
// 				end_par = tmp_s->index;
// 				if (end_par ==  cl_par_ind(tokens, st_par))
// 					handle_par_3(&tmp_b, params[1], params[0]);
// 			}
// 		}
// 		else
// 			tmp_b->par = 0;
// 		tmp_b = tmp_b->sibling;
// 	}
// }

void handle_par_dir(t_big_token **b_tokens, t_token **tokens, int st_par)
{
	t_big_token *tmp_b;
	t_token *tmp_s;
	int end_par;
	int advancement;
	int to_reduce;

	tmp_b = *b_tokens;
	tmp_s = *tokens;
	advancement = st_par - tmp_b->ind_tok_start;
	to_reduce = advancement;
	end_par = cl_par_ind(&tmp_s, st_par);
	to_reduce++;
	move_tok_2_ind(&tmp_s, end_par);
	advancement += (end_par - st_par) + 1;
	while (advancement < tmp_b->length)
	{
		tmp_s = tmp_s->next;
		if (tmp_s->token == TOK_SEP)
		{
			advancement++;
			if (advancement < tmp_b->length)
				tmp_s = tmp_s->next;
		}
		if (tmp_s->token == TOK_REDIRECTOR_LEFT && ft_strlen(tmp_s->value) == 1)
			//init red fd end extract shit + check for next while incrementing adv
		else if (tmp_s->token == TOK_REDIRECTOR_LEFT && ft_strlen(tmp_s->value) == 2)
		else if (tmp_s->token == TOK_REDIRECTOR_RIGHT && ft_strlen(tmp_s->value) == 1)
		else if (tmp_s->token == TOK_REDIRECTOR_RIGHT && ft_strlen(tmp_s->value) == 2)
	}
}
	void handle_par(t_big_token * *b_tokens, t_token * *tokens)
	{
		t_big_token *tmp_b;
		t_token *tmp_s;
		int params[2];
		int st_par;
		int end_par;

		tmp_b = *b_tokens;
		tmp_s = *tokens;
		while (tmp_b)
		{
			init_params(&(params[0]), &(params[1]));
			tmp_s = *tokens;
			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
			if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
			{
				handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
			}
			if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
			{
				st_par = tmp_s->index;
				move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
				if (tmp_s->token == TOK_SEP)
					handle_par_2(&tmp_s, tmp_b, &(params[1]), *tokens);
				if (tmp_s->token == TOK_EXPANDER_CL)
				{
					end_par = tmp_s->index;
					if (end_par == cl_par_ind(tokens, st_par))
						handle_par_3(&tmp_b, params[1], params[0]);
				}
				else if (tmp_s->token == TOK_WORD)
				{
					handle_par_dir(&tmp_b, tokens);
				}
			}
			else
				tmp_b->par = 0;
			tmp_b = tmp_b->sibling;
		}
	}
