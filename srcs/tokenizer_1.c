/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 15:45:04 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/14 20:21:09 by nflan            ###   ########.fr       */
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

int	is_quoted(t_token **tok_list, int rank_in_list)
{
	t_token	*tmp;
	int		res;

	res = 1;
	tmp = *tok_list;
	if (!rank_in_list)
		return (0);
	else
	{
		while (tmp->prev)
		{
			tmp = tmp->prev;
			if (tmp->token == TOK_QUOTER)
				res *= -1;
		}
		if (res < 0)
			return (1);
	}
	return (0);
}

unsigned int	get_real_tok_type(char c, t_token **tok_list)
{
	int	len;

	len = len_ll_list(*tok_list);
	//if (len == 0 || len == 1)
	if (len == 0)
		return (get_tok_type[get_char_class[(int)c]]);
	else
	{
		if (is_quoted(tok_list, 1))
			return (TOK_WORD);
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
