/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:45:04 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/24 15:14:56 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int len_ll_list(t_token *tok_list)
{
	int len;

	len = 0;
	if (!tok_list)
		return (len);
	while (tok_list)
	{
		len++;
		tok_list = tok_list->next;
	}
	return (len);
}

int is_quoted(t_token **tok_list, char c)
{
	t_token *tmp;
	int dq;
	int sq;

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
	if (sq < 0 && dq > 0 && c != '\'')
		return (1);
	else if (dq < 0 && sq > 0 && c != '\"')
		return (2);
	return (0);
}

unsigned int get_real_tok_type(char c, t_token **tok_list)
{
	int len;
	t_token	*last_tok;

	len = len_ll_list(*tok_list);
	if (len == 0)
		return (get_tok_type[get_char_class[(int)c]]);
	last_tok = *tok_list;
	while (last_tok->next)
		last_tok = last_tok->next;
	// printf("for this char: %c, this is the last tok's type %d\n", c, last_tok->token);
	if (is_quoted(&last_tok, c) == 1)
		return (TOK_WORD_S_QUOTED);
	else if (is_quoted(&last_tok, c) == 2)
		return (TOK_WORD_D_QUOTED);
	else
		return (get_tok_type[get_char_class[(int)c]]);
	return (-1);
}

t_token *ft_create_token(t_tok_type tok_type, int length, int i)
{
	t_token *tok;

	tok = ft_calloc(sizeof(t_token), 1);
	if (!tok)
		return (NULL);
	tok->token = tok_type;
	tok->length = length;
	tok->start = i;
	return (tok);
}

t_token *create_tok_bis(t_tok_type tok_type, int quoted, char *value)
{
	t_token *tok;

	tok = ft_calloc(sizeof(t_token), 1);
	if (!tok)
		return (NULL);
	tok->token = tok_type;
	tok->quoted = quoted;
	tok->value = value;
	return (tok);
}

void init_tok_struct(t_token **tok_list, int rank_in_list)
{
	if (is_quoted(tok_list, rank_in_list))
		(*tok_list)->quoted = 1;
	else
		(*tok_list)->quoted = 0;
}
