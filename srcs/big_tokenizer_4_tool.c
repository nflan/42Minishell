/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/28 19:29:39 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_par_1(t_token **tok, t_big_token *tmp_b, int *adv_steps, int *tr)
{
	move_tok_2_ind(tok, tmp_b->ind_tok_start + 1);
	(*adv_steps)++;
	(*tr)++;
}

void	handle_par_2(t_token **tmp_s, t_big_token *tmp_b, int *tr, t_token *tok)
{
	*tmp_s = tok;
	move_tok_2_ind(tmp_s, tmp_b->ind_tok_start + tmp_b->length - 2);
	(*tr)++;
}

void	handle_par_3(t_big_token **tmp_b, int tr, int adv_steps, t_info *info)
{
	t_token	*tmp_s;
	int		i;

	i = 0;
	tmp_s = info->tokens;
	(*tmp_b)->par = 1;
	(*tmp_b)->length -= (2 + tr);
	(*tmp_b)->ind_tok_start += (1 + adv_steps);
	(*tmp_b)->cmd_args_num = 1;
	(*tmp_b)->cmd_args = ft_calloc(2, sizeof(char *));
	while (i < (*tmp_b)->length)
	{
		move_tok_2_ind(&tmp_s, (*tmp_b)->ind_tok_start + i);
		((*tmp_b)->cmd_args)[0] = ft_strjoin_free(((*tmp_b)->cmd_args)[0],
				tmp_s->value, 1);
		i++;
	}
}

void	rd_inout_type(char *str, int *type_red)
{
	if (ft_strlen(str) == 1 && !ft_strncmp(str, "<", 1))
		(*type_red) = 1;
	if (ft_strlen(str) == 2 && !ft_strncmp(str, "<<", 2))
		(*type_red) = 2;
	if (ft_strlen(str) == 1 && !ft_strncmp(str, ">", 1))
		(*type_red) = 3;
	if (ft_strlen(str) == 2 && !ft_strncmp(str, ">>", 2))
		(*type_red) = 4;
}

int	check_if_piped(t_big_token **tmp_b, int ind, t_info *info, int len)
{
	t_token	*tmp;
	int		i;

	(void) tmp_b;
	tmp = info->tokens;
	i = 0;
	move_tok_2_ind(&tmp, ind);
	while (tmp && i < len)
	{
		if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1
			&& !ft_strncmp("|", tmp->value, 1))
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}
