/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:22:43 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/26 19:56:14 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void init_cl_par_ind(int *nb, t_token **tokens, t_token **tmp)
{
	*nb = 0;
	*tmp = *tokens;
}

int cl_par_ind(t_token **tmp_s, int ind_tok)
{
	t_token *tmp;
	int nb_op_tok;

	tmp = *tmp_s;
	init_cl_par_ind(&nb_op_tok, tmp_s, &tmp);
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

static void divide_by_or_and_1(t_token **tmp, t_info *info, int *length, int *i)
{
	int st_par;
	int end_par;

	st_par = (*tmp)->index;

	end_par = cl_par_ind(&(info->tokens), (*tmp)->index);
	*length += (end_par - st_par);
	*i += (end_par - st_par);
	// printf("end par = %d\n", end_par);
	move_tok_2_ind(tmp, end_par);
}

static void divide_by_or_and_2(t_token *tmp, t_big_token **b_tokens, int btok_info[2], t_info *info)
{
	btok_info[1] -= 1;
	if (!ft_strncmp(tmp->value, "&&", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_AND, btok_info, info);
	else if (!ft_strncmp(tmp->value, "||", 2))
		add_b_tok_sib_last(b_tokens, TOK_LEFT_OR, btok_info, info);
	btok_info[0] = tmp->index + 1;
	btok_info[1] = 0;
}

int piped(t_info *info, int start, int length)
{
	t_token *tmp;
	int i;

	tmp = info->tokens;
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

int divide_by_or_and(t_big_token **b_tokens, t_info *info, int btok_info[2])
{
	t_token *tmp;
	int i;
	int	b_tokinfo[2];

	i = 0;
//	printf("\nI entered here with this token()\n");
//	print_s_tokens(tokens, start_tok, length);

	tmp = info->tokens;
	b_tokinfo[0] = btok_info[0];
	b_tokinfo[1] = 0;
	move_tok_2_ind(&tmp, btok_info[0]);
	while (i < btok_info[1])
	{
		b_tokinfo[1]++;
		if (tmp->token == TOK_EXPANDER_OP)
			divide_by_or_and_1(&tmp, info, &b_tokinfo[1], &i);
		else if (tmp->token == TOK_OPERATOR && check_divider_type(tmp->value))
			divide_by_or_and_2(tmp, b_tokens, b_tokinfo, info);
		tmp = tmp->next;
		i++;
	}
	if (!*b_tokens && !piped(info, btok_info[0], btok_info[1]))
		i = add_b_tok_sib_last(b_tokens, TOK_CLEAN, btok_info, info);
	else if (!*b_tokens && piped(info, btok_info[0], btok_info[1]))
		i = add_b_tok_sib_last(b_tokens, TOK_CLEAN_PIPED, btok_info, info);
	else
		i = add_b_tok_sib_last(b_tokens, TOK_LAST, b_tokinfo, info);
	if (i)
		return (ft_putstr_error("in divide by or and "));
	if (handle_par(b_tokens, info) == 1)
		return (ft_putstr_error("in divide by or and "));
	return (0);
}
