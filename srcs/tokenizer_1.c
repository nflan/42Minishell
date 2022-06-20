/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:45:04 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/20 14:26:03 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	len_ll_list(t_token *tok_list)
{
	int	len;

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

int	is_quoted(t_token **tok_list, char c)
{
	t_token	*tmp;
	int		dq;
	int		sq;
	int		i;

	dq = 1;
	sq = 1;
	tmp = *tok_list;
		while (tmp->prev)
		{
			tmp = tmp->prev;
			if (tmp->token == TOK_QUOTER && ft_strlen(tmp->value) && !ft_strncmp('\'', tmp->value, 1))
				sq *= -1;
			else if (tmp->token == TOK_QUOTER && ft_strlen(tmp->value) && !ft_strncmp('\"', tmp->value, 1))
				dq *= -1;
		}
		if (sq < 0 && dq > 0 && c != '\'')
			return (1);
		else if (dq < 0 && sq > 0 && c != '\"')
			return (2);
	return (0);
}

unsigned int	get_real_tok_type(char c, t_token **tok_list)
{
	int		len;
	t_token	*last_tok;

	len = len_ll_list(*tok_list);
	while (last_tok)
		last_tok = last_tok->next;
	if (len == 0)
		return (get_tok_type[get_char_class[(int)c]]);
	else
	{
		if (is_quoted(&last_tok, c) == 1)
			return (TOK_WORD_S_QUOTED);
		else if (is_quoted(&last_tok, c) == 2)
			return (TOK_WORD_D_QUOTED);
		else
			return (get_tok_type[get_char_class[(int)c]]);
	}
	return (-1);
}

t_token	*ft_create_token(t_tok_type tok_type, int length, int i)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->token = tok_type;
	tok->length = length;
	tok->start = i;
	tok->next = NULL;
	return (tok);
}

void	init_tok_struct(t_token **tok_list, int rank_in_list)
{
	if (is_quoted(tok_list, rank_in_list))
		(*tok_list)->quoted = 1;
	else
		(*tok_list)->quoted = 0;
}
