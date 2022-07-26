/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:47:48 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/25 22:49:56 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_tok_last(t_token **tok_list, t_tok_type tok_type, int length, int i)
{
	t_token	*tmp;
	t_token	*bef_last;

	if (!*tok_list)
	{
		*tok_list = ft_create_token(tok_type, length, i);
		if (!*tok_list)
			return (ft_putstr_error("Malloc error\n"));
		(*tok_list)->prev = NULL;
	}
	else
	{
		tmp = *tok_list;
		while (tmp->next)
			tmp = tmp->next;
		bef_last = tmp;
		bef_last->next = ft_create_token(tok_type, length, i);
		if (!bef_last->next)
			return (ft_putstr_error("Malloc error\n"));
		bef_last = bef_last->next;
		bef_last->prev = tmp;
	}
	return (0);
}

int	add_tok_last_bis(t_token **tok_list, t_tok_type tok_t, int q, char *value)
{
	t_token	*tmp;
	t_token	*bef_last;

	if (!value)
		return (1);
	if (!*tok_list)
	{
		*tok_list = create_tok_bis(tok_t, q, value);
		if (!*tok_list)
			return (free(value), 1);
	}
	else
	{
		tmp = *tok_list;
		while (tmp->next)
			tmp = tmp->next;
		bef_last = tmp;
		bef_last->next = create_tok_bis(tok_t, q, value);
		if (!bef_last->next)
			return (free(value), 1);
		bef_last = bef_last->next;
		bef_last->prev = tmp;
	}
	return (free(value), 0);
}

int	add_tok(t_token **tokens, int st, int len, t_tok_type type)
{
	if (type == TOK_WORD_NULL_S || type == TOK_WORD_NULL_D)
	{
		if (type == TOK_WORD_NULL_S)
		{
			if (add_tok_last(tokens, TOK_WORD_S_QUOTED, 0, st))
				return (1);
			if (add_tok_last(tokens, TOK_S_QUOTER, len, st))
				return (1);
		}
		else if (type == TOK_WORD_NULL_D)
		{
			if (add_tok_last(tokens, TOK_WORD_D_QUOTED, 0, st))
				return (1);
			if (add_tok_last(tokens, TOK_D_QUOTER, len, st))
				return (1);
		}
	}
	else
	{
		if (add_tok_last(tokens, type, len, st))
			return (1);
	}
	return (0);
}

int	fill_tok_value(t_token **tok, char *str)
{
	t_token	*tmp;

	if (!*tok || !str)
		return (1);
	tmp = *tok;
	if (tmp)
	{
		while (tmp)
		{
			tmp->value = ft_strncpy(&(str[tmp->start]), tmp->length);
			if (!tmp->value)
				return (ft_putstr_error("Malloc error\n"));
			tmp = tmp->next;
		}
	}
	return (0);
}

char	*ft_strncpy(char *str, int n)
{
	char	*ret;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	ret = ft_calloc(sizeof(char), n + 1);
	if (!ret)
		return (NULL);
	while (i < n)
	{
		ret[i] = str[i];
		i++;
	}
	return (ret);
}
