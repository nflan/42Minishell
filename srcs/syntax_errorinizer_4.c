/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errorinizer_4.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:18:30 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 19:30:49 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	par_not_afore_op_1(t_token *tmp)
{
	t_token	*tmp_prev;

	tmp_prev = NULL;
	if (tmp->token == TOK_EXPANDER_OP && tmp->index > 0)
	{
		if (tmp->prev->token == TOK_SEP && tmp->index > 1)
			tmp_prev = tmp->prev->prev;
		else if (tmp->prev->token != TOK_SEP)
			tmp_prev = tmp->prev;
		if (!(tmp_prev->token == TOK_OPERATOR
				|| tmp_prev->token == TOK_EXPANDER_OP))
			return (1);
	}
	return (0);
}

static int	par_not_afore_op_2(t_token *tmp)
{
	t_token	*tmp_next;

	tmp_next = NULL;
	if (tmp->token == TOK_EXPANDER_CL && tmp->next)
	{
		if (tmp->next->token == TOK_SEP && tmp->next->next)
			tmp_next = tmp->next->next;
		else if (tmp->next->token != TOK_SEP)
			tmp_next = tmp->next;
		if (!(tmp_next->token == TOK_OPERATOR
				|| tmp_next->token == TOK_EXPANDER_CL
				|| tmp_next->token == TOK_REDIRECTOR_RIGHT))
			return (1);
	}
	return (0);
}

int	par_not_afore_op(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (par_not_afore_op_1(tmp))
			return (1);
		if (par_not_afore_op_2(tmp))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
