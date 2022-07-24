/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errorinizer_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:30:49 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 14:20:18 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_op_tok(t_token **tokens)
{
	t_token	*tmp;
	int		ret;

	ret = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_EXPANDER_OP)
			ret++;
		tmp = tmp->next;
	}
	return (ret);
}

int	count_cl_tok(t_token **tokens)
{
	t_token	*tmp;
	int		ret;

	ret = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_EXPANDER_CL)
			ret++;
		tmp = tmp->next;
	}
	return (ret);
}

int	check_count_errors(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_OPERATOR || tmp->token == TOK_REDIRECTOR_RIGHT
			|| tmp->token == TOK_REDIRECTOR_LEFT)
		{
			if ((tmp->value)[0] == '&' && (ft_strlen(tmp->value) != 2))
				return (1);
			else if (ft_strlen(tmp->value) > 2)
				return (2);
		}
		else if ((tmp->token == TOK_EXPANDER_OP
				|| tmp->token == TOK_EXPANDER_CL) && ft_strlen(tmp->value) > 1)
			return (3);
		tmp = tmp->next;
	}
	return (0);
}

int	r_quotes_impair(t_token **tokens)
{
	t_token	*tmp;
	int		s_num;
	int		d_num;

	s_num = 0;
	d_num = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_D_QUOTER)
			d_num++;
		else if (tmp->token == TOK_S_QUOTER)
		{
			s_num++;
		}
		tmp = tmp->next;
	}
	if (s_num % 2 || d_num % 2)
		return (1);
	return (0);
}

int	is_last_op(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->token == TOK_OPERATOR || tmp->token == TOK_REDIRECTOR_LEFT
		|| tmp->token == TOK_REDIRECTOR_RIGHT || tmp->token == TOK_EXPANDER_OP)
		return (1);
	return (0);
}
