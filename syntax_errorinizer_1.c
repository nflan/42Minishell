/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errorinizer_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:30:49 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/25 21:31:52 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

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

int r_2_op_succeding(t_token **tokens)
{
	t_token *tmp;
	t_tok_type op_prev;
	t_tok_type op_next;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_OPERATOR)
			op_prev = tmp->token;
		if (tmp->next && tmp->next->token == TOK_OPERATOR)
		{
			op_next = tmp->next->token;
			if (op_prev == op_next)
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int op_cl_par_succeeding(t_token **tokens)
{
	t_token *tmp;

	tmp = *tokens;
	while (tmp)
	{
		if ((tmp->token == TOK_EXPANDER_OP && tmp->next))
		{
			if (tmp->next->token == TOK_EXPANDER_CL)
				return (1);
			else if (tmp->next->token == TOK_SEP && tmp->next->next)
			{
				if (tmp->next->next->token == TOK_EXPANDER_CL);
					return (2);
			}
		}
		if ((tmp->token == TOK_EXPANDER_CL && tmp->next))
		{
			if (tmp->next->token == TOK_EXPANDER_OP)
				return (3);
			else if (tmp->next->token == TOK_SEP && tmp->next->next)
			{
				if (tmp->next->next->token == TOK_EXPANDER_OP);
					return (4);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int syntax_err_handler(t_token **tokens)
{
	int nb_optok;
	int nb_cltok;

	if (!*tokens)
		return (-1);
	nb_optok = count_op_tok(tokens);
	nb_cltok = count_cl_tok(tokens);
	if (nb_optok != nb_cltok)
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
	if (r_2_op_succeding(tokens))
		return (7);
	return (0);
}

// here where to do my weird experiment

int is_pipe_in_st_end(t_big_token *b_tokens, t_token *tokens)
{
	t_big_token *tmp1;
	t_token *tmp2;

	tmp1 = b_tokens;
	tmp2 = tokens;
	while (tmp1)
	{
		move_tok_2_ind(&tmp2, tmp1->ind_tok_start);
		if (tmp2->token == TOK_SEP && tmp1->length > 2)
			move_tok_2_ind(&tmp2, tmp1->ind_tok_start + 1);
		if (tmp2->token == TOK_OPERATOR && ft_strlen(tmp2->value) == 1 && !ft_strncmp(tmp2->value, "|", 1))
			return (1);
		move_tok_2_ind(&tmp2, tmp1->ind_tok_start + tmp1->length - 1);
		if (tmp2->token == TOK_SEP && tmp1->length > 2)
		{
			tmp2 = tokens;
			move_tok_2_ind(&tmp2, tmp1->ind_tok_start + tmp1->length - 2);
		}
		if (tmp2->token == TOK_OPERATOR && ft_strlen(tmp2->value) == 1 && !ft_strncmp(tmp2->value, "|", 1))
			return (2);
		tmp1 = tmp1->sibling;
	}
	return (0);
}
