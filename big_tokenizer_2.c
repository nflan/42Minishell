/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:22:43 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/30 20:49:41 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

static void init_cl_par_ind(int *nb, t_token *tokens, t_token **tmp)
{
	*nb = 0;
	*tmp = tokens;
}

int cl_par_ind(t_token **tokens, t_tok_type tok, int ind_tok, char *value)
{
	t_token *tmp;
	int nb_op_tok;

	init_cl_par_ind(&nb_op_tok, *tokens, &tmp);
	while (tmp)
	{
		if (tmp->index == ind_tok)
			break;
		tmp = tmp->next;
	}
	while (tmp && tmp->token != TOK_EXPANDER_CL)
	{
		if (tmp->token == TOK_EXPANDER_OP && !ft_strncmp(value, tmp->value, ft_strlen(value)))
			nb_op_tok++;
		tmp = tmp->next;
	}
	while (tmp && nb_op_tok)
	{
		if (tmp->token == TOK_EXPANDER_CL && is_cl_2_op(value, tmp->value))
			nb_op_tok--;
		if (!nb_op_tok)
			return (tmp->index);
		tmp = tmp->next;
	}
	return (-1);
}

static void divide_by_or_and_1(t_token **tmp, t_token **tokens, int *length)
{
	int st_par;
	int end_par;

	st_par = (*tmp)->index;
	end_par = cl_par_ind(tokens, (*tmp)->token, (*tmp)->index, (*tmp)->value);
	*length += (end_par - st_par);
	move_tok_2_ind(tmp, end_par);
}

static void divide_by_or_and_2(t_token *tmp, t_big_token **b_tokens, int *start, int *length)
{
	if (!ft_strncmp(tmp->value, "&&", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_AND, (*start), (*length) - 1);
	else if (!ft_strncmp(tmp->value, "||", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_OR, (*start), (*length) - 1);
	(*start) = tmp->index + 1;
	(*length) = 0;
}

void divide_by_or_and(t_big_token **b_tokens, t_token **tokens, int start_tok, int length)
{
	t_token *tmp;
	int b_length;
	int start;

	start = 0;
	b_length = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->index == start)
			break;
		tmp = tmp->next;
	}
	while (tmp)
	{
		b_length++;
		if (tmp->token == TOK_EXPANDER_OP)
			divide_by_or_and_1(&tmp, tokens, &b_length);
		else if (tmp->token == TOK_OPERATOR && check_divider_type(tmp->value))
			divide_by_or_and_2(tmp, b_tokens, &start, &b_length);
		if (tmp->index == length - 1)
			break;
		tmp = tmp->next;
	}
	if (!*b_tokens)
		add_b_tok_sib_last(b_tokens, TOK_CLEAN, 0, len_ll_list(*tokens));
	else
		add_b_tok_sib_last(b_tokens, TOK_LAST, start, b_length);
	handle_par(b_tokens, tokens);
}

void parse(t_big_token **b_tokens, t_token **tokens, int start, int length)
{
	t_big_token *tmp_b;

	tmp_b = *b_tokens;
	divide_by_or_and(&tmp_b, tokens, start, length);
	if (tmp_b && tmp_b->type == TOK_CLEAN)
		return;
	while (tmp_b)
	{
		if (tmp_b->par)
			parse(&(tmp_b->child), tokens, tmp_b->ind_tok_start, tmp_b->length);
		else
		{
			divide_by_pipe(&tmp_b, tokens);
			if (tmp_b->child)
				parse(&(tmp_b->child->child), tokens, tmp_b->child->ind_tok_start, tmp_b->child->length);
		}
		tmp_b = tmp_b->sibling;
	}
}
