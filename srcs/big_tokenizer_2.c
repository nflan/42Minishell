/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:22:43 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/23 15:19:40 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void init_cl_par_ind(int *nb, t_token **tokens, t_token **tmp)
{
	*nb = 0;
	*tmp = *tokens;
}

int cl_par_ind(t_token **tokens, int ind_tok)
{
	t_token *tmp;
	int nb_op_tok;

	init_cl_par_ind(&nb_op_tok, tokens, &tmp);
	move_tok_2_ind(&tmp, ind_tok);
	while (tmp && tmp->token != TOK_EXPANDER_CL)
	{
		if (tmp->token == TOK_EXPANDER_OP && ft_strlen(tmp->value) == 1 && !ft_strncmp("(", tmp->value, 1))
			nb_op_tok++;
		tmp = tmp->next;
	}
	while (tmp && nb_op_tok)
	{
		if (tmp->token == TOK_EXPANDER_CL && ft_strlen(tmp->value) == 1 && !ft_strncmp(")", tmp->value, 1))
			nb_op_tok--;
		if (!nb_op_tok)
			return (tmp->index);
		tmp = tmp->next;
	}
	return (-1);
}

static void divide_by_or_and_1(t_token **tmp, t_token **tokens, int *length, int *i)
{
	int st_par;
	int end_par;

	st_par = (*tmp)->index;

	end_par = cl_par_ind(tokens, (*tmp)->index);
	*length += (end_par - st_par);
	*i += (end_par - st_par);
	// printf("end par = %d\n", end_par);
	move_tok_2_ind(tmp, end_par);
}

static void divide_by_or_and_2(t_token *tmp, t_big_token **b_tokens, int *start, int *length)
{
	if (!ft_strncmp(tmp->value, "&&", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_AND, (*start), (*length) - 1);
	else if (!ft_strncmp(tmp->value, "||", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_OR, (*start), (*length) - 1);
	// move_tok_2_ind(&tmp, tmp->index + 1);
	(*start) = tmp->index + 1;
	(*length) = 0;
}

int piped(t_token **tokens, int start, int length)
{
	t_token *tmp;
	int i;

	tmp = *tokens;
	i = 0;
	move_tok_2_ind(&tmp, start);
	while (i < length && tmp)
	{
		if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1 && !ft_strncmp(tmp->value, "|", 1))
			return (1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int sophisticated_piped(t_token **tokens, int start, int length)
{
	t_token *tmp;
	int i;
	int st_par;
	int end_par;

	tmp = *tokens;
	i = 0;
	move_tok_2_ind(&tmp, start);
	while (i < length && tmp)
	{
		if (tmp->token == TOK_EXPANDER_OP)
		{
			st_par = tmp->index;
			end_par = cl_par_ind(&tmp, tmp->index);
			move_tok_2_ind(&tmp, end_par);
			i += (end_par - st_par);
		}
		else if (tmp->token == TOK_OPERATOR && ft_strlen(tmp->value) == 1 && !ft_strncmp(tmp->value, "|", 1))
			return (1);
		else
		{
			tmp = tmp->next;
			i++;
		}
	}
	return (0);
}

int divide_by_or_and(t_big_token **b_tokens, t_token **tokens, int start_tok, int length)
{
	t_token *tmp;
	int b_length;
	int i;
	i = 0;
	b_length = 0;
//	printf("\nI entered here with this token()\n");
//	print_s_tokens(tokens, start_tok, length);

	tmp = *tokens;
	move_tok_2_ind(&tmp, start_tok);
	while (i < length)
	{
		b_length++;
		if (tmp->token == TOK_EXPANDER_OP)
			divide_by_or_and_1(&tmp, tokens, &b_length, &i);
		else if (tmp->token == TOK_OPERATOR && check_divider_type(tmp->value))
			divide_by_or_and_2(tmp, b_tokens, &start_tok, &b_length);
		tmp = tmp->next;
		i++;
	}
	if (!*b_tokens && !piped(tokens, start_tok, length))
		i = add_b_tok_sib_last(b_tokens, TOK_CLEAN, start_tok, length);
	else if (!*b_tokens && piped(tokens, start_tok, length))
		i = add_b_tok_sib_last(b_tokens, TOK_CLEAN_PIPED, start_tok, length);
	else
		i = add_b_tok_sib_last(b_tokens, TOK_LAST, start_tok, b_length);
	if (i)
		return (ft_putstr_error("in divide by or and "));
	if (handle_par(b_tokens, tokens) == 1)
		return (ft_putstr_error("in divide by or and "));
	return (0);
}
