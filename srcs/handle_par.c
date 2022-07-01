/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_par.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 03:00:36 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 03:01:36 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void handle_par_1(t_token **tmp_s, t_big_token *tmp_b, int *adv_steps, int *to_reduce)
{
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + 1);
	(*adv_steps)++;
	(*to_reduce)++;
}

void handle_par_2(t_token **tmp_s, t_big_token *tmp_b, int *to_reduce, t_token *tokens)
{
	*tmp_s = tokens;
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + tmp_b->length - 2);
	(*to_reduce)++;
}

void handle_par_3(t_big_token **tmp_b, int to_reduce, int adv_steps, t_info *info)
{
	int		i;
	t_token	*tmp_s;

	i = 0;
	tmp_s = info->tokens;
	(*tmp_b)->par = 1;
	(*tmp_b)->length -= (2 + to_reduce);
	(*tmp_b)->ind_tok_start += (1 + adv_steps);
	(*tmp_b)->cmd_args_num = 1;
	(*tmp_b)->cmd_args = ft_calloc(2, sizeof(char *));
	while (i < (*tmp_b)->length)
	{
		move_tok_2_ind(&tmp_s, (*tmp_b)->ind_tok_start + i);
		((*tmp_b)->cmd_args)[0] = ft_strjoin_free(((*tmp_b)->cmd_args)[0], tmp_s->value, 1);
		i++;
	}
}

void	handle_par_4(int (*p)[2], t_token **s, t_big_token *b, t_info *i)
{
	init_params(&((*p)[0]), &((*p)[1]));
	*s = i->tokens;
	move_tok_2_ind(s, b->ind_tok_start);
	if ((*s)->token == TOK_SEP && b->length > 2)
		handle_par_1(s, b, &((*p)[0]), &((*p)[1]));

}


int	handle_par_5(t_token **s, t_big_token **b, t_tab *t, t_info *i)
{
	(t->stend_par)[0] = (*s)->index;
	move_tok_2_ind(s, (*b)->ind_tok_start + (*b)->length - 1);
	if ((*s)->token == TOK_SEP)
		handle_par_2(s, *b, &((t->params)[1]), i->tokens);
	if ((*s)->token == TOK_EXPANDER_CL)
	{
		(t->stend_par)[1] = (*s)->index;
		if ((t->stend_par)[1] == cl_par_ind(&(i->tokens), (t->stend_par)[0]))
			handle_par_3(b, (t->params)[1], (t->params)[0], i);
	}
	else if ((*s)->token == TOK_WORD)
	{
		if (handle_par_dir(b, i))
			return (ft_putstr_error("in handle par "));
	}
	return (0);
}


