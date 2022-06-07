/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 13:47:48 by omoudni           #+#    #+#             */
/*   Updated: 2022/06/07 18:14:32 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"

void	add_tok_last(t_token **tok_list, t_tok_type tok_type, int length, int i)
{
	t_token	*tmp;
	t_token	*bef_last;
	int		rank_in_list;

	if (!*tok_list)
	{
		*tok_list = ft_create_token(tok_type, length, i);
		(*tok_list)->prev = NULL;
		rank_in_list = 0;
	}
	else
	{
		tmp = *tok_list;
		while (tmp->next)
			tmp = tmp->next;
		bef_last = tmp;
		bef_last->next = ft_create_token(tok_type, length, i);
		bef_last = bef_last->next;
		bef_last->prev = tmp;
		rank_in_list = 1;
	}
	init_tok_struct(tok_list, rank_in_list);
}

void	detect_tokens(t_token **tok_list, char *str)
{
	int				i;
	unsigned int	tok_type;
	int				length;
	int				start;

	i = 0;
	if (!str)
		return;
	while (str[i])
	{
		length = 1;
		start = i;
		tok_type = get_real_tok_type(str[i], tok_list);
		i++;
		while (tok_type != TOK_EXPANDER_OP && tok_type != TOK_EXPANDER_CL && str[i] && get_real_tok_type(str[i], tok_list) == tok_type)
		{
			length++;
			i++;
		}
		add_tok_last(tok_list, tok_type, length, start);
	}
}

void	fill_tok_value(t_token **tok, char *str)
{
	t_token	*tmp;

	if (!*tok || !str)
		return;
	tmp = *tok;
	while (tmp)
	{
		tmp->value = ft_strncpy(&(str[tmp->start]), tmp->length);
		tmp = tmp->next;
	}
}

char	*ft_strncpy(char *str, int n)
{
	int i;
	char *ret;

	i = 0;
	if (!str)
		return (NULL);
	ret = malloc((n + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	while (i < n)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

void	index_toks(t_token **tokens, int start, int length)
{
	int i;
	t_token *tmp;

	tmp = *tokens;
	i = 0;
	while (*tokens)
	{
		(*tokens)->index = i;
		(*tokens) = (*tokens)->next;
		i++;
	}
	*tokens = tmp;
}
