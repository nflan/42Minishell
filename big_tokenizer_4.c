/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/01 18:20:31 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static void handle_par_1(t_token **tmp_s, t_big_token *tmp_b, int *adv_steps, int *to_reduce)
{
	// printf("I'm moving the tok here: %d\n", tmp_b->ind_tok_start + 1);
	// printf("tmp_s's new index: %d\n",(*tmp_s)->index);
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + 1);
	// printf("tmp_s's new index after moving to 1: %d\n",(*tmp_s)->index);
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

void handle_par(t_big_token **b_tokens, t_token **tokens)
{
	t_big_token *tmp_b;
	t_token		*tmp_s;
	int			params[2];

	tmp_b = *b_tokens;
	tmp_s = *tokens;
	while (tmp_b)
	{
	init_params(&(params[0]), &(params[1]));
		tmp_s = *tokens;
		// printf("tok_start (handle part): %d\n", tmp_b->ind_tok_start);
		move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start);
		// printf("check that start of tmp_b: %d\n", tmp_b->ind_tok_start);
		if (tmp_s->token == TOK_SEP && tmp_b->length > 2)
		{
		//	printf("YEs I'm in that case\n");
			handle_par_1(&tmp_s, tmp_b, &(params[0]), &(params[1]));
		}
		// if (!tmp_s)
			// printf("It's null and problematic!!\n");
		if (tmp_s->token == TOK_EXPANDER_OP && tmp_b->length > 2)
		{
			move_tok_2_ind(&tmp_s, tmp_b->ind_tok_start + tmp_b->length - 1);
			if (tmp_s->token == TOK_SEP)
				handle_par_2(&tmp_s, tmp_b, &(params[1]), *tokens);
			if (tmp_s->token == TOK_EXPANDER_CL)
			{
			//	printf("token expander\n");
				handle_par_3(&tmp_b, params[1], params[0]);
			}
		// printf("\n%d %d\n", params[0], params[1]);
		}
		else
			tmp_b->par = 0;
		tmp_b = tmp_b->sibling;
	}
}
