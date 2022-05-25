/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_tokenizer_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 17:18:51 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/25 20:55:11 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

t_big_token *ft_create_btoken(t_big_tok_type type, int ind_tok_start, int length)
{
	t_big_token *tok;

	tok = (t_big_token *)malloc(sizeof(t_big_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->ind_tok_start = ind_tok_start;
	tok->length = length;
	// tok->more_info =
	tok->child = NULL;
	tok->sibling = NULL;
	return (tok);
}

void add_b_tok_sib_last(t_big_token **b_tok_list, t_big_tok_type type, int start, int length)
{
	t_big_token *tmp;
	t_big_token *bef_last;
	int rank_in_list;

	if (!*b_tok_list)
	{
		*b_tok_list = ft_create_btoken(type, start, length);
		rank_in_list = 0;
	}
	else
	{
		tmp = *b_tok_list;
		while (tmp->sibling)
			tmp = tmp->sibling;
		bef_last = tmp;
		bef_last->sibling = ft_create_btoken(type, start, length);
		rank_in_list = 1;
	}
	// init_tok_struct(tok_list, rank_in_list);
}

// void add_b_tok_child_last(t_big_token **b_tok_list, t_big_tok_type type, int start, int length)
// {
// 	t_big_token *tmp;
// 	t_big_token *bef_last;
// 	int rank_in_list;

// 	if (!*b_tok_list)
// 	{
// 		*b_tok_list = ft_create_btoken(type, start, length);
// 		rank_in_list = 0;
// 	}
// 	else
// 	{
// 		tmp = *b_tok_list;
// 		while (tmp->sibling)
// 			tmp = tmp->sibling;
// 		bef_last = tmp;
// 		bef_last->child = ft_create_btoken(type, start, length);
// 		rank_in_list = 1;
// 	}
// init_tok_struct(tok_list, rank_in_list);
// }

int check_divider_type(char *tmp_value)
{
	if (!ft_strncmp(tmp_value, "&&", 2))
		return (1);
	if (!ft_strncmp(tmp_value, "||", 2))
		return (2);
	return (0);
}

int is_cl_2_op(char *value_tok_op, char *value_tok_cl)
{
	if (!ft_strncmp(value_tok_op, "(", 1) && !ft_strncmp(value_tok_cl, ")", 1))
		return (1);
	if (!ft_strncmp(value_tok_op, "{", 1) && !ft_strncmp(value_tok_cl, "}", 1))
		return (2);
	return (0);
}

int cl_par_ind(t_token **tokens, t_tok_type tok, int ind_tok, char *value)
{
	t_token *tmp;
	int nb_op_tok;

	nb_op_tok = 0;
	tmp = *tokens;
	if (!tmp)
		return (-1);
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

void move_tok_2_ind(t_token **tokens, int ind)
{
	while (*tokens)
	{
		if ((*tokens)->index == ind)
			return;
		*tokens = (*tokens)->next;
	}
}

void divide_by_or_and(t_big_token **b_tokens, t_token **tokens)
{
	t_token *tmp;
	int length;
	int start;
	int st_par;
	int end_par;

	start = 0;
	length = 0;
	tmp = *tokens;
	while (tmp)
	{
		length++;
		if (tmp->token == TOK_EXPANDER_OP)
		{
			st_par = tmp->index;
			printf("\n\nthis is the start of the pars: %d\n\n", st_par);
			end_par = cl_par_ind(tokens, tmp->token, tmp->index, tmp->value);
			printf("\n\nthis is the end of the pars: %d\n\n", end_par);
			length += (end_par - st_par);
			move_tok_2_ind(&tmp, end_par);
			printf("\n\nThis is the value you are looking for: %s\n\n", tmp->value);
		}
		else if (tmp->token == TOK_OPERATOR && check_divider_type(tmp->value))
		{
			if (!ft_strncmp(tmp->value, "&&", 2))
				add_b_tok_sib_last(b_tokens, TOK_LEFT_AND, start, length - 1);
			if (!ft_strncmp(tmp->value, "||", 2))
				add_b_tok_sib_last(b_tokens, TOK_LEFT_OR, start, length - 1);
			start = tmp->index + 1;
			length = 0;
		}
		tmp = tmp->next;
	}
	if (!*b_tokens)
		add_b_tok_sib_last(b_tokens, TOK_CLEAN, 0, len_ll_list(*tokens));
	else
		add_b_tok_sib_last(b_tokens, TOK_LAST, start, length);
}

void divide_by_pipe(t_big_token **b_tokens, t_token **tokens)
{
	t_token *tmp_s;
	t_big_token *tmp_b;
	int length_piped;
	int start;
	int st_par;
	int end_par;
	int i;
	int j;

	tmp_s = *tokens;
	tmp_b = *b_tokens;
	start = 0;
	length_piped = 0;

	while (tmp_b)
	{
		i = 0;
		j = tmp_b->ind_tok_start;
		length_piped = 0;
		while (i < tmp_b->length)
		{
			length_piped++;
			move_tok_2_ind(&tmp_s, j);
			if (tmp_s->token == TOK_EXPANDER_OP)
			{
				st_par = tmp_s->index;
				end_par = cl_par_ind(&tmp_s, tmp_s->token, tmp_s->index, tmp_s->value);
				length_piped += (end_par - st_par);
			}
			else if (tmp_s->token == TOK_OPERATOR && ft_strlen(tmp_s->value) == 1 && !ft_strncmp(tmp_s->value, "|", 1))
			{
				add_b_tok_sib_last(&((tmp_b)->child), TOK_LEFT_PIPE, start, length_piped - 1);
				start = tmp_s->index + 1;
				length_piped = 0;
			}
			j++;
			i++;
		}
		if (!((*b_tokens)->child))
			add_b_tok_sib_last(&((tmp_b)->child), TOK_CLEAN, start, length_piped);
		else
			add_b_tok_sib_last(&((tmp_b)->child), TOK_PIPE_LAST, start, length_piped);
		tmp_b = tmp_b->sibling;
	}
}
