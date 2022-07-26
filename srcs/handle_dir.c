/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 02:56:02 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 23:47:39 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_dir_0(t_token **t, t_big_token **b, t_info *in, int (*itscl)[6])
{
	*t = in->tokens;
	(*itscl)[0] = 0;
	(*itscl)[1] = 0;
	(*itscl)[2] = 0;
	(*itscl)[4] = (*b)->length;
	move_tok_2_ind(t, (*b)->ind_tok_start);
	count_cmd_args(b, (*b)->ind_tok_start, (&(in)->tokens), (*b)->length);
	(*itscl)[3] = (*b)->cmd_args_num;
}

int	handle_dir_1(t_token **t, int (*itscl)[6], int step, t_big_token **b)
{
	if (step == 1)
	{
		rd_inout_type((*t)->value, &((*itscl)[1]));
		((*itscl)[0])++;
		(*itscl)[2] = 1;
		*t = (*t)->next;
		((*itscl)[4])--;
	}
	if (step == 2)
	{
		while ((*t) && (*itscl)[4] && (*t)->token != TOK_SEP && (*t)->token
			!= TOK_REDIRECTOR_LEFT && (*t)->token != TOK_REDIRECTOR_RIGHT)
		{
			(*b)->cmd_args[(*b)->cmd_args_num - (*itscl)[3]]
				= ft_strjoin_free((*b)->cmd_args[(*b)->cmd_args_num
					- (*itscl)[3]], (*t)->value, 1);
			if (!(*b)->cmd_args[(*b)->cmd_args_num - (*itscl)[3]])
				return (ft_putstr_error("Malloc error\n"));
			(*t) = (*t)->next;
			((*itscl)[4])--;
		}
		((*itscl)[3])--;
	}
	return (0);
}

int	handle_dir_2(t_token **t, int (*itscl)[6], int step, t_big_token **b)
{
	if (!step)
		if (((*t)->token == TOK_REDIRECTOR_LEFT || (*t)->token
				== TOK_REDIRECTOR_RIGHT) && !((*itscl)[0] % 2))
			return (1);
	if (step == 1)
	{
		if (ft_fdnew(*b, &((*b)->fd), t, *itscl))
			return (1);
		((*itscl)[2]) = 0;
		((*itscl)[0])++;
	}
	if (step == 2)
	{
		(*t) = (*t)->next;
		((*itscl)[4])--;
	}
	return (0);
}

int	handle_dir(t_big_token **tmp_b, t_info *info)
{
	t_token	*tmp;
	int		itscl[6];

	handle_dir_0(&tmp, tmp_b, info, &itscl);
	while (tmp && itscl[4])
	{
		if (handle_dir_2(&tmp, &itscl, 0, tmp_b))
			itscl[5] = handle_dir_1(&tmp, &itscl, 1, tmp_b);
		else if (tmp->token != TOK_SEP && !itscl[2])
		{
			if (handle_dir_1(&tmp, &itscl, 2, tmp_b))
				return (1);
		}
		else if ((tmp->token != TOK_SEP && (itscl[0] % 2) && itscl[2]))
		{
			if (handle_dir_2(&tmp, &itscl, 1, tmp_b))
				return (1);
		}
		else
			itscl[5] = handle_dir_2(&tmp, &itscl, 2, tmp_b);
	}
	(*tmp_b)->par = 0;
	return (0);
}
