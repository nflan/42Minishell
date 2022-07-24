/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errorinizer_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:59:19 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 19:32:45 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	r_2_op_succeding(t_token **tokens, int op_next, int op_prev)
{
	t_token		*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_OPERATOR)
		{
			if (!tmp->next || (tmp->next && tmp->next->token == TOK_SEP
					&& !tmp->next->next))
				return (2);
			op_prev = tmp->token;
			if (tmp->next)
			{
				if (tmp->next->token == TOK_OPERATOR)
					op_next = tmp->next->token;
				else if (tmp->next->token == TOK_SEP && tmp->next->next)
					op_next = tmp->next->next->token;
				if (op_prev == op_next)
					return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int	ft_test_isdir(t_token *t, int i)
{
	if (i == 1)
	{
		if (t->token == TOK_REDIRECTOR_LEFT || t->token == TOK_REDIRECTOR_RIGHT)
			return (1);
	}
	else if (i == 2)
	{
		if (t && t->token == TOK_SEP && !t->next)
			return (1);
	}
	else if (i == 3)
	{
		if (t->token == TOK_OPERATOR || t->token == TOK_REDIRECTOR_LEFT
			|| t->token == TOK_REDIRECTOR_RIGHT)
			return (1);
	}
	else if (i == 4)
	{
		if (t->token == TOK_SEP && t->next && (t->next->token == TOK_OPERATOR
				|| t->next->token == TOK_REDIRECTOR_LEFT
				|| t->next->token == TOK_REDIRECTOR_RIGHT))
			return (1);
	}
	return (0);
}

int	r_dir_op_succeding(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (ft_test_isdir(tmp, 1))
		{
			if (!tmp->next || ft_test_isdir(tmp->next, 2))
				return (2);
			if (tmp->next)
			{
				if (ft_test_isdir(tmp->next, 3))
					return (1);
				else if (ft_test_isdir(tmp->next, 4))
					return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int	op_cl_par_succeeding(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if ((tmp->token == TOK_EXPANDER_OP && tmp->next))
		{
			if (tmp->next->token == TOK_EXPANDER_CL)
				return (1);
			if ((tmp->next->token == TOK_SEP && tmp->next->next)
				&& (tmp->next->next->token == TOK_EXPANDER_CL))
				return (2);
		}
		if ((tmp->token == TOK_EXPANDER_CL && tmp->next))
		{
			if (tmp->next->token == TOK_EXPANDER_OP)
				return (3);
			if ((tmp->next->token == TOK_SEP && tmp->next->next)
				&& (tmp->next->next->token == TOK_EXPANDER_OP))
				return (4);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	syntax_err_handler(t_token **tokens)
{
	int	nb_opcltok[2];

	if (!*tokens)
		return (-1);
	nb_opcltok[0] = count_op_tok(tokens);
	nb_opcltok[1] = count_cl_tok(tokens);
	if (nb_opcltok[0] != nb_opcltok[1])
		return (1);
	if ((*tokens)->token == TOK_OPERATOR)
		return (2);
	if (check_count_errors(tokens))
		return (3);
	if (op_cl_par_succeeding(tokens))
		return (4);
	if (r_quotes_impair(tokens))
		return (5);
	if (is_last_op(tokens))
		return (6);
	if (r_2_op_succeding(tokens, -1, -1))
		return (7);
	if (r_dir_op_succeding(tokens))
		return (8);
	if (par_not_afore_op(tokens))
		return (9);
	return (0);
}
