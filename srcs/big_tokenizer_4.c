/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:45:15 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/01 03:12:38 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
/*
int	handle_piped(t_big_token **tmp_b, t_info *info)
{
	int		i;
	t_token	*tmp_s;

	i = 0;
	tmp_s = info->tokens;
	(*tmp_b)->cmd_args_num = 1;
	(*tmp_b)->cmd_args = ft_calloc(2, sizeof(char *));
	if (!(*tmp_b)->cmd_args)
		return (ft_putstr_error("Malloc error in ft_calloc in handle piped "));
	while (i < (*tmp_b)->length)
	{
		move_tok_2_ind(&tmp_s, (*tmp_b)->ind_tok_start + i);
		((*tmp_b)->cmd_args)[0] = ft_strjoin_free(((*tmp_b)->cmd_args)[0], tmp_s->value, 1);
		if (!(*tmp_b)->cmd_args[0])
			return (ft_putstr_error("Malloc error in ft_strjoin_free in handle piped "));
		i++;
	}
	return (0);
}

void init_params(int *adv_steps, int *to_reduce)
{
	(*adv_steps) = 0;
	(*to_reduce) = 0;
}

int check_if_piped(t_big_token **tmp_b, int ind, t_info *info, int len)
{
	t_token *tmp;
	int i;

	(void) tmp_b;
	tmp = info->tokens;
	i = 0;
	move_tok_2_ind(&tmp, ind);
	while (tmp && i < len)
	{
		if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1 && !ft_strncmp("|", tmp->value, 1))
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}
*/
