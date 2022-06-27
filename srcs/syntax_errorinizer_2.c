/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errorinizer_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:59:19 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/27 10:59:06 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int r_2_op_succeding(t_token **tokens)
{
	t_token *tmp;
	t_tok_type op_prev;
	t_tok_type op_next;

	tmp = *tokens;
	op_next = -1;
	op_prev = -1;
	//	printf("%s\n", tmp->value);
	while (tmp)
	{
		if (tmp->token == TOK_OPERATOR)
		{

			if (!tmp->next || (tmp->next && tmp->next->token == TOK_SEP && !tmp->next->next))
				return (2);
			op_prev = tmp->token;
			if (tmp->next)
			{
				if (tmp->next->token == TOK_OPERATOR)
					op_next = tmp->next->token;
				else if (tmp->next->token == TOK_SEP && tmp->next->next)
					op_next = tmp->next->next->token;
				if (op_next != (t_tok_type)-1 && op_prev == op_next)
					return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int r_dir_op_succeding(t_token **tokens)
{
	t_token *tmp;
	//	t_tok_type	op_prev;
	//	t_tok_type	op_next;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT)
		{
			if (!tmp->next || (tmp->next && tmp->next->token == TOK_SEP && !tmp->next->next))
				return (2);
			if (tmp->next)
			{

				if (tmp->next->token == TOK_OPERATOR || tmp->next->token == TOK_REDIRECTOR_LEFT || tmp->next->token == TOK_REDIRECTOR_RIGHT)
					return (1);
				else if (tmp->next->token == TOK_SEP && tmp->next->next && (tmp->next->next->token == TOK_OPERATOR || tmp->next->next->token == TOK_REDIRECTOR_LEFT || tmp->next->next->token == TOK_REDIRECTOR_RIGHT))
					return (1);
			}
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
			if ((tmp->next->token == TOK_SEP && tmp->next->next) && (tmp->next->next->token == TOK_EXPANDER_CL))
				return (2);
		}
		if ((tmp->token == TOK_EXPANDER_CL && tmp->next))
		{
			if (tmp->next->token == TOK_EXPANDER_OP)
				return (3);
			if ((tmp->next->token == TOK_SEP && tmp->next->next) && (tmp->next->next->token == TOK_EXPANDER_OP))
				return (4);
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
	if (r_dir_op_succeding(tokens))
		return (8);
	return (0);
}

int is_pipe_in_st_end(t_big_token *b_tokens, t_token *tokens)
{
	t_big_token *tmp1;
	t_token *tmp2;

	tmp1 = b_tokens;
	tmp2 = tokens;
	while (tmp1)
	{
		move_tok_2_ind(&tmp2, tmp1->ind_tok_start);
		if (tmp2 && tmp2->token == TOK_SEP && tmp1->length > 2)
			move_tok_2_ind(&tmp2, tmp1->ind_tok_start + 1);
		if (tmp2 && tmp2->token == TOK_OPERATOR && ft_strlen(tmp2->value) == 1 && !ft_strncmp(tmp2->value, "|", 1))
			return (1);
		move_tok_2_ind(&tmp2, tmp1->ind_tok_start + tmp1->length - 1);
		if (tmp2 && tmp2->token == TOK_SEP && tmp1->length > 2)
		{
			tmp2 = tokens;
			move_tok_2_ind(&tmp2, tmp1->ind_tok_start + tmp1->length - 2);
		}
		if (tmp2 && ((tmp2->token == TOK_OPERATOR && ft_strlen(tmp2->value) == 1 && !ft_strncmp(tmp2->value, "|", 1)) || (tmp2->token == TOK_REDIRECTOR_LEFT || tmp2->token == TOK_REDIRECTOR_RIGHT)))
			return (2);
		tmp1 = tmp1->sibling;
	}
	return (0);
}

int is_there_par(t_big_token *b_tokens, t_token *tokens)
{
	t_big_token *tmp_b;
	t_token *tmp;
	int cl_par;
	int st_par;

	tmp_b = b_tokens;
	tmp = tokens;
	move_tok_2_ind(&tmp, tmp_b->ind_tok_start);
	while (tmp)
	{
		if (tmp->token == TOK_EXPANDER_OP)
		{
			st_par = tmp->index;
			cl_par = cl_par_ind(&tmp, st_par);
			if (cl_par < tmp_b->ind_tok_start + tmp_b->length)
				return (st_par);
		}
		tmp = tmp->next;
	}
	return (-1);
}

int is_red_st_par(t_big_token *b_tokens, t_token *tokens)
{
	t_big_token *tmp_b;
	t_token *tmp;
	int i;
	int st_par;

	tmp_b = b_tokens;
	while (tmp_b)
	{
		tmp = tokens;
		st_par = is_there_par(tmp_b, tmp);
		i = tmp_b->ind_tok_start;
		if (st_par == -1)
			return (0);
		if (sophisticated_piped(&tmp, i, tmp_b->length))
			return (0);
		move_tok_2_ind(&tmp, i);
		while (i < st_par)
		{
			if (tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT)
				return (1);
			tmp = tmp->next;
			i++;
		}
		tmp_b = tmp_b->sibling;
	}
	return (0);
}

// int is_red_st_par(t_big_token *b_tokens, t_token *tokens)
// {
// 	t_big_token *tmp_b;
// 	t_token *tmp;
// 	int i;

// 	tmp_b = b_tokens;
// 	tmp = tokens;
// 	i = 0;
// 	while (tmp_b)
// 	{
// 		move_tok_2_ind(&tmp, tmp_b->ind_tok_start + tmp_b->length - 1);
// 		if (tmp->token == TOK_SEP)
// 		{
// 			tmp = tokens;
// 			move_tok_2_ind(&tmp, tmp_b->ind_tok_start + tmp_b->length - 2);
// 		}
// 		if (tmp && tmp->token == TOK_EXPANDER_CL)
// 		{
// 			tmp = tokens;
// 			if (sophisticated_piped(&tmp, tmp_b->ind_tok_start, tmp_b->length))
// 				return (0);
// 			else
// 			{
// 				while (tmp && i < tmp_b->length)
// 				{
// 					if (tmp->token == TOK_EXPANDER_OP)
// 						return (0);
// 					else if (tmp->token == TOK_REDIRECTOR_LEFT || tmp->token == TOK_REDIRECTOR_RIGHT)
// 						return (1);
// 					tmp = tmp->next;
// 					i++;
// 				}
// 			}
// 		}
// 		tmp_b = tmp_b->sibling;
// 	}
// 	return (0);
// }
