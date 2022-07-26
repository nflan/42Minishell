/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_par_dir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 02:51:23 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 23:28:52 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_par_dir_0(t_token **t, t_big_token **b, t_info *i, int (*itctlt)[7])
{
	(*t) = i->tokens;
	(*itctlt)[0] = 0;
	(*itctlt)[1] = 0;
	(*itctlt)[3] = 0;
	(*itctlt)[5] = 0;
	handle_piped(b, i);
	move_tok_2_ind(t, (*b)->ind_tok_start);
	if ((*t)->token == TOK_SEP)
	{
		move_tok_2_ind(t, (*b)->ind_tok_start + 1);
		((*itctlt)[3])++;
		((*itctlt)[5])++;
	}
	if ((*t)->token == TOK_EXPANDER_OP)
	{
		((*itctlt)[5])++;
		(*itctlt)[2] = cl_par_ind(t, (*t)->index);
		(*itctlt)[3] += 2;
		move_tok_2_ind(t, (*itctlt)[2] + 1);
		(*itctlt)[4] = (*b)->length + (*b)->ind_tok_start - 1 - (*itctlt)[2];
		if (check_if_piped((*itctlt)[2] + 1, i, (*itctlt)[4]))
			return (0);
		(*itctlt)[3] += (*itctlt)[4];
	}
	return (-1);
}

int	handle_par_dir_1(t_token **t, t_big_token **b, int (*itctlt)[7], int step)
{
	if (step == 1)
	{
		if (ft_fdnew(*b, &((*b)->fd), t, *itctlt))
			return (1);
		(*itctlt)[0]++;
	}
	if (step == 2)
	{
		(*t) = (*t)->next;
		((*itctlt)[4])--;
	}
	if (step == 3)
	{
		(*b)->ind_tok_start += (*itctlt)[5];
		(*b)->length -= (*itctlt)[3];
		(*b)->par = 1;
	}
	return (0);
}

int	handle_par_dir_2(t_token *tmp, int (*itctlt)[7], int step)
{
	if (step == 1)
	{
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token
				== TOK_REDIRECTOR_RIGHT) && ((*itctlt)[0]) % 2)
			return (1);
	}
	if (step == 2)
	{
		if ((tmp->token == TOK_REDIRECTOR_LEFT || tmp->token
				== TOK_REDIRECTOR_RIGHT) && !((*itctlt)[0] % 2))
			return (1);
	}
	if (step == 3)
	{
		rd_inout_type(tmp->value, &((*itctlt)[1]));
		((*itctlt)[0])++;
	}
	return (0);
}

int	handle_par_dir(t_big_token **tmp_b, t_info *info)
{
	t_token	*tmp;
	int		itctlt[7];

	if (!handle_par_dir_0(&tmp, tmp_b, info, &itctlt))
		return (0);
	while (tmp && itctlt[4])
	{
		if (handle_par_dir_2(tmp, &itctlt, 1))
			return (1);
		if (handle_par_dir_2(tmp, &itctlt, 2))
			itctlt[6] = handle_par_dir_2(tmp, &itctlt, 3);
		else if (tmp->token == TOK_WORD && (!(itctlt[0] % 2) || !itctlt[1]))
			return (1);
		if (tmp->token == TOK_WORD && (itctlt[0] % 2))
		{
			if (handle_par_dir_1(&tmp, tmp_b, &itctlt, 1))
				return (1);
		}
		else
			itctlt[6] = handle_par_dir_1(&tmp, tmp_b, &itctlt, 2);
	}
	itctlt[6] = handle_par_dir_1(&tmp, tmp_b, &itctlt, 3);
	return (0);
}
