/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 02:56:02 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 12:36:38 by omoudni          ###   ########.fr       */
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
	move_tok_2_ind(&(in)->tokens, (*b)->ind_tok_start);
	count_cmd_args(b, (*b)->ind_tok_start, t, (*b)->length);
	(*itscl)[3] = (*b)->cmd_args_num;
}

int	handle_dir_1(t_info **t, int (*itscl)[6], int step, t_big_token **b)
{
	if (step == 1)
	{
		rd_inout_type((*t)->tokens->value, &((*itscl)[1]));
		((*itscl)[0])++;
		(*itscl)[2] = 1;
		(*t)->tokens = (*t)->tokens->next;
		((*itscl)[4])--;
	}
	if (step == 2)
	{
		while ((*t) && (*itscl)[4] && (*t)->tokens->token != TOK_SEP)
		{
			(*b)->cmd_args[(*b)->cmd_args_num - (*itscl)[3]]
				= ft_strjoin_free((*b)->cmd_args[(*b)->cmd_args_num
					- (*itscl)[3]], (*t)->tokens->value, 1);
			if (!(*b)->cmd_args[(*b)->cmd_args_num - (*itscl)[3]])
				return (ft_putstr_error("handle dir "));
			(*t)->tokens = (*t)->tokens->next;
			((*itscl)[4])--;
		}
		((*itscl)[3])--;
	}
	return (0);
}

int	handle_dir_2(t_info **t, int (*itscl)[6], int step, t_big_token **b)
{
	if (!step)
		if (((*t)->tokens->token == TOK_REDIRECTOR_LEFT || (*t)->tokens->token
				== TOK_REDIRECTOR_RIGHT) && !((*itscl)[0] % 2))
			return (1);
	if (step == 1)
	{
		if ((*itscl)[1] == 1 || (*itscl)[1] == 2)
		{
			if (ft_fdnew(*b, &((*b)->fd_in), t, *itscl))
				return (ft_putstr_error("in handle dir "));
		}
		else
			if (ft_fdnew(*b, &((*b)->fd_out), t, *itscl))
				return (ft_putstr_error("in handle dir "));
		((*itscl)[2]) = 0;
		((*itscl)[0])++;
	}
	if (step == 2)
	{
		(*t)->tokens = (*t)->tokens->next;
		((*itscl)[4])--;
	}
	return (0);
}

int	handle_dir(t_big_token **tmp_b, t_info *info)
{
	t_token	*tmp;
	int		itscl[6];

	handle_dir_0(&tmp, tmp_b, info, &itscl);
	while (info->tokens && itscl[4])
	{
		if (handle_dir_2(&info, &itscl, 0, tmp_b))
			itscl[5] = handle_dir_1(&info, &itscl, 1, tmp_b);
		else if (info->tokens->token != TOK_SEP && !itscl[2])
		{
			if (handle_dir_1(&info, &itscl, 2, tmp_b))
				return (1);
		}
		else if ((info->tokens->token != TOK_SEP && (itscl[0] % 2) && itscl[2]))
		{
			if (handle_dir_2(&info, &itscl, 1, tmp_b))
				return (1);
		}
		else
			itscl[5] = handle_dir_2(&info, &itscl, 2, tmp_b);
	}
	(*tmp_b)->par = 0;
	info->tokens = tmp;
	return (0);
}
