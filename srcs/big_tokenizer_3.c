/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 17:57:08 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/29 00:38:05 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	dbp_1(t_big_token **tmp_b, int b_info[2], t_info *info)
{
	if (!(*tmp_b)->child)
	{
		if (add_b_tok_sib_last(&((*tmp_b)->child), TOK_CLEAN, b_info, info))
			return (ft_putstr_error("in divide by pipe "));
	}
	else
	{
		if (add_b_tok_sib_last(&((*tmp_b)->child), TOK_PIPE_LAST, b_info, info))
			return (ft_putstr_error("in divide by pipe "));
	}
	if (handle_par(&((*tmp_b)->child), info))
		return (ft_putstr_error("in divie by pipe "));
	return (0);
}

static void	dbp_2(t_big_token *tmp_b, int (*btok_info)[2], int (*ij)[2], int i)
{
	if (i == 1)
	{
		(*btok_info)[0] = tmp_b->ind_tok_start;
		(*btok_info)[1] = 0;
		(*ij)[0] = 0;
		(*ij)[1] = tmp_b->ind_tok_start;
	}
	else if (i == 2)
	{
		((*ij)[1])++;
		((*ij)[0])++;
	}
}

static void	dbp_3(t_token *tmp_s, int (*btok_info)[2], int (*ij)[2], int (*stend_par)[2])
{
	(*stend_par)[0] = tmp_s->index;
	(*stend_par)[1] = cl_par_ind(&tmp_s, tmp_s->index);
	((*btok_info)[1]) += ((*stend_par)[1] - (*stend_par)[0]);
	((*ij)[1]) += (((*stend_par)[1]) - ((*stend_par)[0])) + 1;
	((*ij)[0]) += (((*stend_par)[1]) - ((*stend_par)[0])) + 1;
}

static int	dbp_4(int (*btok_info)[2], int (*ij)[2], t_info *info, t_big_token **tmp_b)
{
	((*btok_info)[1])--;
	if (add_b_tok_sib_last(&((*tmp_b)->child), TOK_LEFT_PIPE, *btok_info, info))
		return (ft_putstr_error("in divide by pipe "));
	(*btok_info)[1] = 0;
	(*ij)[0]++;
	(*ij)[1] = (*btok_info)[0];
	return (0);
}

int	divide_by_pipe(t_big_token **b_tokens, t_info *info)
{
	t_token *tmp_s;
	t_big_token *tmp_b;
	int btok_info[2];
	int stend_par[2];
	int ij[2];

	tmp_s = info->tokens;
	tmp_b = *b_tokens;
//	printf("start tmp_b: %d, length tmp_b: %d\n", tmp_b->ind_tok_start, tmp_b->length);
	dbp_2(tmp_b, &btok_info, &ij, 1);
	printf("start: %d, len: %d", btok_info[0], btok_info[1]);
	print_s_tokens(&tmp_s, btok_info[0], tmp_b->length);
	exit (0);
	while (ij[0] < tmp_b->length)
	{
//		printf("j: %d\n", ij[1]);
		move_tok_2_ind(&tmp_s, ij[1]);
		btok_info[1]++;
		printf("i: %d\n", ij[0]);
		printf("this is the value of the tok i'm checking: %s, here is its type: %d\n", tmp_s->value, tmp_s->token);
		if (is_pipe(tmp_s))
		{
			printf("I entered here hamdoullah, here is my value: %s\n", tmp_s->value);
			if (dbp_4(&btok_info, &ij, info, &tmp_b))
				return (ft_putstr_error("in divide by pipe "));
			btok_info[0] = tmp_s->index + 1;
		}
		else if (tmp_s->token == TOK_EXPANDER_OP)
			dbp_3(tmp_s, &btok_info, &ij, &stend_par);
		else
			dbp_2(tmp_b, &btok_info, &ij, 2);
	}
	return (dbp_1(&tmp_b, btok_info, info));
}

