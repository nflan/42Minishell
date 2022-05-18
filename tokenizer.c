/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:30:49 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/18 16:31:05 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"


char	*ft_strncpy(char *str, int n)
{
	int		i;
	char	*ret;

	i = 0;
	if (!str)
		return (NULL);
	ret = malloc(n * sizeof(char));
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

t_token *ft_create_token(t_tok_type tok_type, int length, int i)
{
	t_token *tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->token = tok_type;
	tok->length = length;
	tok->start = i;
	tok->next = NULL;
	return (tok);
}

void add_tok_last(t_token **tok_list, t_tok_type tok_type, int length, int i)
{
	t_token *tmp;
	t_token *bef_last;
	int rank_in_list;

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

int is_quoted(t_token **tok_list, int rank_in_list)
{
	t_token *tmp;
	int res;

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
		return (0);
	}
}

void init_tok_struct(t_token **tok_list, int rank_in_list)
{
	if (is_quoted(tok_list, rank_in_list))
		(*tok_list)->quoted = 1;
	else
		(*tok_list)->quoted = 0;
}

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

unsigned int get_real_tok_type(char c, t_token **tok_list)
{
	int len;

	len = len_ll_list(*tok_list);
	if (len == 0 || len == 1)
		return (get_tok_type[get_char_class[c]]);
	else
	{
		if (is_quoted(tok_list, 1))
			return (TOK_WORD);
		else
			return (get_tok_type[get_char_class[c]]);
	}
	return (-1);
}

void detect_tokens(t_token **tok_list, char *str)
{
	int i;
	unsigned int tok_type;
	int length;
	int start;

	i = 0;
	if (!str)
		return;
	while (str[i])
	{
		length = 1;
		start = i;
		tok_type = get_real_tok_type(str[i], tok_list);
		i++;
		while (str[i] && get_real_tok_type(str[i], tok_list) == tok_type)
		{
			length++;
			i++;
		}
		add_tok_last(tok_list, tok_type, length, start);
	}
}

void fill_tok_value(t_token **tok, char *str)
{
	t_token *tmp;

	if (!*tok || !str)
		return;
	tmp = *tok;
	while (tmp)
	{
		tmp->value = ft_strncpy(&(str[tmp->start]), tmp->length);
		tmp = tmp->next;
	}
}

int main(int argc, char *argv[])
{
	t_token *tokens;

	tokens = NULL;
	detect_tokens(&tokens, argv[1]);
	fill_tok_value(&tokens, argv[1]);
	t_token *tmp;
	tmp = tokens;
	while (tmp)
	{
		printf("%d\n", tmp->token);
		printf("%s\n", tmp->value);
		printf("%d\n", tmp->length);
		printf("%d\n", tmp->start);
		printf("%d\n", tmp->quoted);
		printf("\n\n");
		tmp = tmp->next;
	}
	return 0;
}
