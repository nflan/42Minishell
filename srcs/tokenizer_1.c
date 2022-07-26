/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:45:04 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 15:51:20 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_quoted(t_token **tok_list, char c)
{
	t_token	*tmp;
	int		dq;
	int		sq;

	dq = 1;
	sq = 1;
	tmp = *tok_list;
	while (tmp)
	{
		if (tmp->token == TOK_S_QUOTER)
			sq *= -1;
		else if (tmp->token == TOK_D_QUOTER)
			dq *= -1;
		tmp = tmp->prev;
	}
	tmp = *tok_list;
	if (sq < 0 && dq > 0 && c == '\'' && tmp && tmp->token == TOK_S_QUOTER)
		return (3);
	if (sq > 0 && dq < 0 && c == '\"' && tmp && tmp->token == TOK_D_QUOTER)
		return (4);
	if (sq < 0 && dq > 0 && c != '\'')
		return (1);
	else if (dq < 0 && sq > 0 && c != '\"')
		return (2);
	return (0);
}

unsigned int	get_real_tok_type(char c, t_token **t, t_tok_type *tok_type_tab)
{
	t_token	*last_tok;
	int		len;
	int		is_qted;

	len = len_ll_list(*t);
	if (len == 0)
		return (tok_type_tab[(int)c]);
	last_tok = *t;
	while (last_tok->next)
		last_tok = last_tok->next;
	is_qted = is_quoted(&last_tok, c);
	if (is_qted == 1)
		return (TOK_WORD_S_QUOTED);
	else if (is_qted == 2)
		return (TOK_WORD_D_QUOTED);
	else if (is_qted == 3)
		return (TOK_WORD_NULL_S);
	else if (is_qted == 4)
		return (TOK_WORD_NULL_D);
	else
		return (tok_type_tab[(int)c]);
	return (-1);
}

void	init_tok_struct(t_token **tok_list, int rank_in_list)
{
	if (is_quoted(tok_list, rank_in_list))
		(*tok_list)->quoted = 1;
	else
		(*tok_list)->quoted = 0;
}
