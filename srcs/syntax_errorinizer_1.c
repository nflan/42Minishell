/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errorinizer_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:30:49 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/02 14:35:29 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

int count_op_tok(t_token **tokens)
{
	t_token *tmp;
	int ret;

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

int count_cl_tok(t_token **tokens)
{
	t_token *tmp;
	int ret;

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

int check_count_errors(t_token **tokens)
{
	t_token *tmp;

	tmp = *tokens;
	while (tmp)
	{
		if ((tmp->token == TOK_OPERATOR || tmp->token == TOK_REDIRECTOR_RIGHT || tmp->token == TOK_REDIRECTOR_LEFT) && ft_strlen(tmp->value) > 2)
			return (1);
		else if ((tmp->token == TOK_EXPANDER_OP || tmp->token == TOK_EXPANDER_CL) && ft_strlen(tmp->value) > 1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int r_quotes_impair(t_token **tokens)
{
	t_token *tmp;
	int q_num;

	q_num = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_QUOTER)
		{
			printf("tmp's index: %d\n", tmp->index);
			q_num++;
		}
		tmp = tmp->next;
	}
	printf("q_num: %d\n", q_num);
	if (q_num % 2)
		return (1);
	return (0);
}

int is_last_op(t_token **tokens)
{
	t_token *tmp;

	tmp = *tokens;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	if (tmp->token == TOK_OPERATOR || tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT)
		return (1);
	return (0);
}
