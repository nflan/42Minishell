/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errorinizer_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:59:19 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/27 18:11:07 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_test_pipe(t_token *tmp, int i)
{
	if (i == 1)
	{
		if (tmp && tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1
			&& !ft_strncmp(tmp->value, "|", 1))
			return (1);
	}
	else if (i == 2)
	{
		if (tmp && ((tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1
					&& !ft_strncmp(tmp->value, "|", 1))
				|| ft_test_isdir(tmp, 1)))
			return (1);
	}
	return (0);
}

int	is_pipe_in_st_end(t_big_token *b_tokens, t_token *tokens)
{
	t_big_token	*tmp1;
	t_token		*tmp2;

	tmp1 = b_tokens;
	tmp2 = tokens;
	while (tmp1)
	{
		move_tok_2_ind(&tmp2, tmp1->ind_tok_start);
		if (tmp2 && tmp2->token == TOK_SEP && tmp1->length > 2)
			move_tok_2_ind(&tmp2, tmp1->ind_tok_start + 1);
		if (ft_test_pipe(tmp2, 1))
			return (1);
		move_tok_2_ind(&tmp2, tmp1->ind_tok_start + tmp1->length - 1);
		if (tmp2 && tmp2->token == TOK_SEP && tmp1->length > 2)
		{
			tmp2 = tokens;
			move_tok_2_ind(&tmp2, tmp1->ind_tok_start + tmp1->length - 2);
		}
		if (ft_test_pipe(tmp2, 2))
			return (2);
		tmp1 = tmp1->sibling;
	}
	return (0);
}

int	is_there_par(t_big_token *b_tokens, t_token *tokens)
{
	t_big_token	*tmp_b;
	t_token		*tmp;
	int			cl_par;
	int			st_par;

	tmp_b = b_tokens;
	tmp = tokens;
	move_tok_2_ind(&tmp, tmp_b->ind_tok_start);
	while (tmp)
	{
		if (tmp->token == TOK_EXPANDER_OP)
		{
			st_par = tmp->index;
			cl_par = cl_par_ind(&tmp, st_par);
			if (cl_par < tmp_b->ind_tok_start + tmp_b->length)
				return (st_par);
		}
		tmp = tmp->next;
	}
	return (-1);
}

int	is_red_st_par(t_big_token *tmp_b, t_token *tokens)
{
	t_token		*tmp;
	int			i;
	int			st_par;

	while (tmp_b)
	{
		tmp = tokens;
		st_par = is_there_par(tmp_b, tmp);
		i = tmp_b->ind_tok_start;
		if (st_par == -1)
			return (0);
		if (sophisticated_piped(&tmp, i, tmp_b->length))
			return (0);
		move_tok_2_ind(&tmp, i);
		while (i < st_par)
		{
			if (ft_test_isdir(tmp, 1))
				return (1);
			tmp = tmp->next;
			i++;
		}
		tmp_b = tmp_b->sibling;
	}
	return (0);
}